//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/CredentialStore.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Response.h"


namespace ofx {
namespace HTTP {


DefaultCredentialStore::DefaultCredentialStore()
{
}


DefaultCredentialStore::~DefaultCredentialStore()
{
}


void DefaultCredentialStore::setCredentials(const AuthScope& scope,
                                            const Credentials& credentials)
{
    if (!credentials.hasCredentials())
    {
        ofLogWarning("DefaultCredentialStore::setCredentials") << "Credentials are empty.  Ignoring.";
        return;
    }

    std::unique_lock<std::mutex> lock(_mutex);
    credentialMap[scope] = credentials;
    basicCredentialCacheMap.erase(scope); // invalidate cached credentials with the same scope
    digestCredentialCacheMap.erase(scope);
}


void DefaultCredentialStore::setCredentialsFromURI(const Poco::URI& uri)
{
    std::string username;
    std::string password;

    std::string userInfo = uri.getUserInfo();

    const std::string::size_type p = userInfo.find(':');

    if (p != std::string::npos)
    {
        username.assign(userInfo, 0, p);
        password.assign(userInfo, p + 1, std::string::npos);
    }
    else
    {
        username.assign(userInfo);
        password.clear();
    }

//    std::cout << uri.toString() << " / " << username << " / " << password << std::endl;

    setCredentials(uri, username, password);
}


void DefaultCredentialStore::setCredentials(const Poco::URI& uri,
                                            const Credentials& credentials)
{
    setCredentials(AuthScope(uri), credentials);
}


void DefaultCredentialStore::setCredentials(const Poco::URI& uri,
                                            const std::string& username,
                                            const std::string& password)
{
    setCredentials(AuthScope(uri), Credentials(username, password));
}


bool DefaultCredentialStore::hasCredentials(const AuthScope& targetScope) const
{
    Credentials matchingCredentials;
    AuthScope   matchingScope;
    return getCredentials(targetScope, matchingScope, matchingCredentials);
}


bool DefaultCredentialStore::getCredentials(const AuthScope& targetScope,
                                            AuthScope& matchingScope,
                                            Credentials& matchingCredentials) const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return getCredentialsWithExistingLock(targetScope, matchingScope, matchingCredentials);
}


bool DefaultCredentialStore::getCredentialsWithExistingLock(const AuthScope& targetScope,
                                                            AuthScope& matchingScope,
                                                            Credentials& matchingCredentials) const
{
    // calls to this function are expected to be locked!
    auto iter = credentialMap.find(targetScope);

    // TODO
//    cout << "TARGET Scope = " << targetScope.toString() << endl;
//    cout << "TARGET CREDZ = " << targetScope.toString() << endl;

    if (iter != credentialMap.end())
    {
        matchingScope       = (*iter).first;
        matchingCredentials = (*iter).second;
        return true;
    }
    else
    {
        int bestMatchFactor  = -1;
        auto bestMatch = credentialMap.end();
        iter = credentialMap.begin();
        while (iter != credentialMap.end())
        {
            int factor = (*iter).first.match(targetScope);

            if (factor > bestMatchFactor)
            {
                bestMatch = iter;
                bestMatchFactor = factor;
            }
            ++iter;
        }

        if (bestMatch != credentialMap.end())
        {
            matchingScope       = (*bestMatch).first;
            matchingCredentials = (*bestMatch).second;
            return true;
        }
        else
        {
            return false;
        }
    }
}


void DefaultCredentialStore::clearCredentials(const AuthScope& scope)
{
    std::unique_lock<std::mutex> lock(_mutex);
    credentialMap.erase(scope);
    basicCredentialCacheMap.erase(scope);
    digestCredentialCacheMap.erase(scope);
}


void DefaultCredentialStore::clearCredentials(const Poco::URI& uri)
{
    clearCredentials(AuthScope(uri));
}


void DefaultCredentialStore::requestFilter(Context&,
                                           Request& request) const
{
    // first check and see if the request has any authentication headers
    // these could be added via default session headers
    if (request.has(Poco::Net::HTTPRequest::AUTHORIZATION))
    {
        ofLogVerbose("DefaultCredentialStore::authenticate") << "HTTP Authorization headers already set.  Skipping authentication.";
        return;
    }

    Poco::URI uri(request.getURI());

    AuthScope    targetScope(uri.getHost(), uri.getPort());
    Credentials  matchingCredentials;
    AuthScope    matchingScope;

    std::unique_lock<std::mutex> lock(_mutex);

    // mutex locking happens in getCredentials() and authenticateWithCache()
    if (getCredentialsWithExistingLock(targetScope, matchingScope, matchingCredentials))
    {
        // first search our digest credentials to see if we have a matching scope (preferred)
        auto iterDigest = digestCredentialCacheMap.find(matchingScope);

        if (iterDigest != digestCredentialCacheMap.end())
        {
            (*iterDigest).second->updateAuthInfo(request); // successfully updated auth info for matching scope
            ofLogVerbose("DefaultCredentialStore::updateAuthentication") << "Found and updated digest credentials.";
            return;
        }

        // if there are no digest credentials, search for basic credentials

        auto iterBasic = basicCredentialCacheMap.find(matchingScope);

        if (iterBasic != basicCredentialCacheMap.end())
        {
            (*iterBasic).second->authenticate(request); // successfully updated auth info for matching scope
            ofLogVerbose("DefaultCredentialStore::updateAuthentication") << "Found and updated basic credentials.";
            return;
        }

        ofLogVerbose("DefaultCredentialStore::updateAuthentication") << "Had no matching cached credentials for preemptive authentication.";
        return;
    }
    else
    {
        ofLogVerbose("DefaultCredentialStore::authenticate") << "Had no matching credentials for preemptive authentication.";
        return;
    }
}


void DefaultCredentialStore::responseFilter(Context&,
                                            Request& request,
                                            Response& response) const
{

    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
    {
        Poco::Net::HTTPResponse::ConstIterator iter = response.find("WWW-Authenticate");

        if (iter != response.end())
        {
            Poco::URI uri(request.getURI());

            AuthenticationType requestedAuthType;

            if (Poco::Net::HTTPCredentials::isBasicCredentials(iter->second))
            {
                requestedAuthType = BASIC;
            }
            else if (Poco::Net::HTTPCredentials::isDigestCredentials(iter->second))
            {
                requestedAuthType = DIGEST;
            }
            else
            {
                ofLogError("DefaultCredentialStore::authenticate") << "Incompatible or unknown WWW-Authenticate header.  Basic and digest supported: " << iter->second;
                return;
            }

            // extract the realm
            Poco::Net::HTTPAuthenticationParams params;

            try
            {
                params.fromResponse(response);
                // it is very unlikely that any of the following exceptions will be thrown because we have already checked for most.
            }
            catch (const Poco::Net::NotAuthenticatedException& exc)
            {
                ofLogError("DefaultCredentialStore::authenticate") << "HTTP response has no authentication header." << " : " << exc.displayText();
                return;
            }
            catch (const Poco::InvalidArgumentException& exc)
            {
                ofLogError("DefaultCredentialStore::authenticate") << "Incompatible or unknown WWW-Authenticate header.  Basic and digest supported: " << iter->second << " : " << exc.displayText();
                return;
            }
            catch (const Poco::SyntaxException& exc)
            {
				ofLogError("DefaultCredentialStore::authenticate") << "Error parsing WWW-Authenticate header: " << iter->second << " : " << exc.displayText();
                return;
            }

            AuthScope targetScope(uri.getScheme(),
                                  uri.getHost(),
                                  uri.getPort(),
                                  params.getRealm(),
                                  requestedAuthType);

            Credentials matchingCredentials;

            AuthScope matchingScope;

            std::unique_lock<std::mutex> lock(_mutex);

            if(getCredentialsWithExistingLock(targetScope,
                                              matchingScope,
                                              matchingCredentials))
            {
                if (BASIC == requestedAuthType)
                {
                    // replace any old ones (probably means they failed and were updated somewhere)
                    basicCredentialCacheMap[matchingScope] = std::make_shared<Poco::Net::HTTPBasicCredentials>(matchingCredentials.getUsername(), matchingCredentials.getPassword());
                    basicCredentialCacheMap[matchingScope].get()->authenticate(request);
                    return;
                }
                else if (DIGEST == requestedAuthType)
                {
                    digestCredentialCacheMap[matchingScope] = std::make_shared<Poco::Net::HTTPDigestCredentials>(matchingCredentials.getUsername(),matchingCredentials.getPassword());
                    digestCredentialCacheMap[matchingScope].get()->authenticate(request, response);
                    return;
                }
                else
                {
                    ofLogError("DefaultCredentialStore::authenticate") << "Unknown requestedAuthType: " << requestedAuthType;
                    return;
                }
            }
            else
            {
                ofLogVerbose("DefaultCredentialStore::authenticate") << "Had no matching credentials for authentication.";
                return;
            }
        }

        ofLogVerbose("DefaultCredentialStore::authenticate") << "Response was unauthorized, but could not find WWW-Authenticate header.";
        return;
    }
    else
    {
        ofLogVerbose("DefaultCredentialStore::authenticate") << "Response was not unauthorized.  Nothing to be done.";
        return;
    }

}


//bool DefaultCredentialStore::canFilterResponse(Request&,
//                                               Response&,
//                                               Context&) const
//{
//    // TODO
//    return true;
//}



bool DefaultCredentialStore::authenticateWithCache(const AuthScope&,
                                                   Poco::Net::HTTPRequest&)
{
    // TODO:
    return true;
}


void DefaultCredentialStore::clear()
{
    std::unique_lock<std::mutex> lock(_mutex);
    credentialMap.clear();
    basicCredentialCacheMap.clear();
    digestCredentialCacheMap.clear();
}


/*
void ofxHTTPCredentialStore::dump() {
    ofScopedLock lock(mutex);

    ofxHTTPCredentialMapIter iter = credentialMap.begin();

    if(iter != credentialMap.end()) {
        ofxHTTPAuthScope   scope       = (*iter).first;
        ofxHTTPCredentials credentials = (*iter).second;

        ofLogVerbose("ofxHTTPCredentialStore::dump") << scope;

    }
}
*/


} } // namespace ofx::HTTP
