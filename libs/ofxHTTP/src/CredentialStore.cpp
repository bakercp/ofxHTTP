// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofx/HTTP/CredentialStore.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"


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
        ofLogWarning("CredentialStore::setCredentials") << "Credentials are empty.  Ignoring.";
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
    
	if (p != string::npos)
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
                                     const string& username,
                                     const string& password)
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
    HTTPCredentialMapIter iter = credentialMap.find(targetScope);

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
        HTTPCredentialMapIter bestMatch = credentialMap.end();
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


void DefaultCredentialStore::requestFilter(BaseRequest& request,
                                           Context&)
{
    // first check and see if the request has any authentication headers
    // these could be added via default session headers
    if (request.has(Poco::Net::HTTPRequest::AUTHORIZATION))
    {
        ofLogVerbose("CredentialStore::authenticate") << "HTTP Authorization headers already set.  Skipping authentication.";
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
        HTTPDigestCredentialCacheMapIter iterDigest = digestCredentialCacheMap.find(matchingScope);

        if (iterDigest != digestCredentialCacheMap.end())
        {
            (*iterDigest).second->updateAuthInfo(request); // successfully updated auth info for matching scope
            ofLogVerbose("CredentialStore::updateAuthentication") << "Found and updated digest credentials.";
            return;
        }

        // if there are no digest credentials, search for basic credentials
        
        HTTPBasicCredentialCacheMapIter iterBasic = basicCredentialCacheMap.find(matchingScope);

        if (iterBasic != basicCredentialCacheMap.end())
        {
            (*iterBasic).second->authenticate(request); // successfully updated auth info for matching scope
            ofLogVerbose("CredentialStore::updateAuthentication") << "Found and updated basic credentials.";
            return;
        }
        
        ofLogVerbose("CredentialStore::updateAuthentication") << "Had no matching cached credentials for preemptive authentication.";
        return;
    }
    else
    {
        ofLogVerbose("CredentialStore::authenticate") << "Had no matching credentials for preemptive authentication.";
        return;
    }    
}


void DefaultCredentialStore::responseFilter(BaseRequest& request,
                                            BaseResponse& response,
                                            Context&)
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
                ofLogError("CredentialStore::authenticate") << "Incompatible or unknown WWW-Authenticate header.  Basic and digest supported: " << iter->second;
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
                ofLogError("CredentialStore::authenticate") << "HTTP response has no authentication header.";
                return;
            }
            catch (const Poco::InvalidArgumentException& exc)
            {
                ofLogError("CredentialStore::authenticate") << "Incompatible or unknown WWW-Authenticate header.  Basic and digest supported: " << iter->second;
                return;
            }
            catch (const Poco::SyntaxException& exc)
            {
                ofLogError("CredentialStore::authenticate") << "Error parsing WWW-Authenticate header: "  << iter->second;
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
                    basicCredentialCacheMap[matchingScope] = HTTPBasicCredentialsSharedPtr(new Poco::Net::HTTPBasicCredentials(matchingCredentials.getUsername(), matchingCredentials.getPassword()));
                    basicCredentialCacheMap[matchingScope].get()->authenticate(request);
                    return;
                }
                else if (DIGEST == requestedAuthType)
                {
                    digestCredentialCacheMap[matchingScope] = HTTPDigestCredentialsSharedPtr(new Poco::Net::HTTPDigestCredentials(matchingCredentials.getUsername(),matchingCredentials.getPassword()));
                    digestCredentialCacheMap[matchingScope].get()->authenticate(request, response);
                    return;
                }
                else
                {
                    ofLogError("CredentialStore::authenticate") << "Unknown requestedAuthType: " << requestedAuthType;
                    return;
                }
            }
            else
            {
                ofLogVerbose("CredentialStore::authenticate") << "Had no matching credentials for authentication.";
                return;
            }
        }
        
        ofLogVerbose("CredentialStore::authenticate") << "Response was unauthorized, but could not find WWW-Authenticate header.";
        return;
    }
    else
    {
        ofLogVerbose("CredentialStore::authenticate") << "Response was not unauthorized.  Nothing to be done.";
        return;
    }
    
}


bool DefaultCredentialStore::canFilterResponse(BaseRequest&,
                                               BaseResponse&,
                                               Context&) const
{
    // TODO
    return true;
}



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
//------------------------------------------------------------------------------
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
