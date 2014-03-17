// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/Client/CredentialStore.h"


namespace ofx {
namespace HTTP {
namespace Client {


CredentialStore::CredentialStore()
{
}


CredentialStore::CredentialStore(CredentialStore& that)
{
    Poco::FastMutex::ScopedLock thatLock(that.mutex);
    credentialMap = that.credentialMap;
    // cache is not copied
}


CredentialStore& CredentialStore::operator = (CredentialStore& that)
{
    Poco::FastMutex::ScopedLock thisLock(mutex);
    Poco::FastMutex::ScopedLock thatLock(that.mutex);
    credentialMap = that.credentialMap;
    // cache is not copied
    return *this;
}


CredentialStore::~CredentialStore()
{
}


void CredentialStore::setCredentials(const AuthScope& scope,
                                     const Credentials& credentials)
{
    if(!credentials.hasCredentials())
    {
        ofLogWarning("CredentialStore::setCredentials") << "Credentials are empty.  Ignoring.";
        return;
    }
        
    Poco::FastMutex::ScopedLock lock(mutex);
    credentialMap[scope] = credentials;
    basicCredentialCacheMap.erase(scope); // invalidate cached credentials with the same scope
    digestCredentialCacheMap.erase(scope);
}


void CredentialStore::setCredentialsFromURI(const Poco::URI& uri)
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

    setCredentials(uri, username, password);
}


void CredentialStore::setCredentials(const Poco::URI& uri,
                                     const Credentials& credentials)
{
    setCredentials(AuthScope(uri), credentials);
}


void CredentialStore::setCredentials(const Poco::URI& uri,
                                     const string& username,
                                     const string& password)
{
    setCredentials(AuthScope(uri), Credentials(username, password));
}


bool CredentialStore::hasCredentials(const AuthScope& targetScope) const
{
    Credentials matchingCredentials;
    AuthScope   matchingScope;
    return getCredentials(targetScope, matchingScope, matchingCredentials);
}


bool CredentialStore::getCredentials(const AuthScope& targetScope,
                                     AuthScope& matchingScope,
                                     Credentials& matchingCredentials) const
{
    Poco::FastMutex::ScopedLock lock(mutex);
    return getCredentialsWithExistingLock(targetScope, matchingScope, matchingCredentials);
}


bool CredentialStore::getCredentialsWithExistingLock(const AuthScope& targetScope,
                                                     AuthScope& matchingScope,
                                                     Credentials& matchingCredentials) const
{
    // calls to this function are expected to be locked!
    HTTPCredentialMapIter iter = credentialMap.find(targetScope);

    // TODO
//    cout << "TARGET Scope = " << targetScope.toString() << endl;
//    cout << "TARGET CREDZ = " << targetScope.toString() << endl;

    

    if(iter != credentialMap.end())
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
        while(iter != credentialMap.end())
        {
            int factor = (*iter).first.match(targetScope);
            
            if(factor > bestMatchFactor)
            {
                bestMatch = iter;
                bestMatchFactor = factor;
            }
            ++iter;
        }
        
        if(bestMatch != credentialMap.end())
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


void CredentialStore::clearCredentials(const AuthScope& scope)
{
    Poco::FastMutex::ScopedLock lock(mutex);
    credentialMap.erase(scope);
    basicCredentialCacheMap.erase(scope);
    digestCredentialCacheMap.erase(scope);
}


void CredentialStore::clearCredentials(const Poco::URI& uri)
{
    clearCredentials(AuthScope(uri));
}


bool CredentialStore::authenticate(const Poco::Net::HTTPClientSession& pSession,
                                   Poco::Net::HTTPRequest& pRequest)
{
    
    // first check and see if the request has any authentication headers
    // these could be added via default session headers
    if(pRequest.has(Poco::Net::HTTPRequest::AUTHORIZATION))
    {
        ofLogVerbose("CredentialStore::authenticate") << "HTTP Authorization headers already set.  Skipping authentication.";
        return false;
    }

    AuthScope    targetScope(pSession.getHost(), pSession.getPort());
    Credentials  matchingCredentials;
    AuthScope    matchingScope;
    
    Poco::FastMutex::ScopedLock lock(mutex);

    // mutex locking happens in getCredentials() and authenticateWithCache()
    if(getCredentialsWithExistingLock(targetScope,matchingScope,matchingCredentials))
    {
        
        // first search our digest credentials to see if we have a matching scope (preferred)
        HTTPDigestCredentialCacheMapIter iterDigest = digestCredentialCacheMap.find(matchingScope);

        if(iterDigest != digestCredentialCacheMap.end())
        {
            (*iterDigest).second->updateAuthInfo(pRequest); // successfully updated auth info for matching scope
            return true;
        }
        
        // if there are no digest credentials, search for basic credentials
        
        HTTPBasicCredentialCacheMapIter iterBasic = basicCredentialCacheMap.find(matchingScope);

        if(iterBasic != basicCredentialCacheMap.end())
        {
            (*iterBasic).second->authenticate(pRequest); // successfully updated auth info for matching scope
            return true;
        }
        
        ofLogVerbose("CredentialStore::authenticate") << "Had no matching cached credentials for preemptive authentication.";
        return false;
    }
    else
    {
        ofLogVerbose("CredentialStore::authenticate") << "Had no matching credentials for preemptive authentication.";
        return false;
    }    
}


bool CredentialStore::authenticate(const Poco::Net::HTTPClientSession& pSession,
                                   Poco::Net::HTTPRequest& pRequest,
                                   Poco::Net::HTTPResponse& pResponse)
{

    if(pResponse.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
    {
        for(Poco::Net::HTTPResponse::ConstIterator iter = pResponse.find("WWW-Authenticate"); iter != pResponse.end(); ++iter)
        {
            AuthenticationType requestedAuthType;
            
            if(Poco::Net::HTTPCredentials::isBasicCredentials(iter->second))
            {
                requestedAuthType = BASIC;
                cout << "AUTHENTICATING BASIC: " << iter->second << endl;
            }
            else if(Poco::Net::HTTPCredentials::isDigestCredentials(iter->second))
            {
                requestedAuthType = DIGEST;
                cout << "AUTHENTICATING DIGEST: " << iter->second << endl;
            }
            else
            {
                ofLogError("CredentialStore::authenticate") << "Incompatible or unknown WWW-Authenticate header.  Basic and digest supported: " << iter->second;
                return false;
            }

            // extract the realm
            Poco::Net::HTTPAuthenticationParams params;
            
            try
            {
                params.fromResponse(pResponse);
                // it is very unlikely that any of the following exceptions will be thrown because we have already checked for most.
            }
            catch (const Poco::Net::NotAuthenticatedException& exc)
            {
                ofLogError("CredentialStore::authenticate") << "HTTP response has no authentication header.";
                return false;
            }
            catch (const Poco::InvalidArgumentException& exc)
            {
                ofLogError("CredentialStore::authenticate") << "Incompatible or unknown WWW-Authenticate header.  Basic and digest supported: " << iter->second;
                return false;
            }
            catch (const Poco::SyntaxException& exc)
            {
                ofLogError("CredentialStore::authenticate") << "Error parsing WWW-Authenticate header: "  << iter->second;
                return false;
            }

            std::string scheme = pSession.secure() ? "https" : "http";
            
            AuthScope targetScope(scheme,
                                  pSession.getHost(),
                                  pSession.getPort(),
                                  params.getRealm(),
                                  requestedAuthType);

            Credentials matchingCredentials;

            AuthScope matchingScope;
            
            Poco::FastMutex::ScopedLock lock(mutex);
            
            if(getCredentialsWithExistingLock(targetScope,
                                              matchingScope,
                                              matchingCredentials))
            {
                cout << "HAD CREDS FOR TARGET SCOPE   = " << targetScope.toString() << endl;
                cout << "HAD CREDS FOR MATCHING SCOPE = " << matchingScope.toString() << endl;
                if(requestedAuthType == BASIC)
                {
                    // replace any old ones (probably means they failed and were updated somewhere)
                    basicCredentialCacheMap[matchingScope] = HTTPBasicCredentialsSharedPtr(new Poco::Net::HTTPBasicCredentials(matchingCredentials.getUsername(),matchingCredentials.getPassword()));
                    basicCredentialCacheMap[matchingScope].get()->authenticate(pRequest);
                    return true;
                }
                else if(requestedAuthType == DIGEST)
                {
                    digestCredentialCacheMap[matchingScope] = HTTPDigestCredentialsSharedPtr(new Poco::Net::HTTPDigestCredentials(matchingCredentials.getUsername(),matchingCredentials.getPassword()));
                    digestCredentialCacheMap[matchingScope].get()->authenticate(pRequest,pResponse);
                    return true;
                }
                else
                {
                    ofLogError("CredentialStore::authenticate") << "Unknown requestedAuthType: " << requestedAuthType;
                    return false;
                }
            }
            else
            {
                ofLogVerbose("CredentialStore::authenticate") << "Had no matching credentials for authentication.";
                return false;
            }
        }
        
        ofLogVerbose("CredentialStore::authenticate") << "Response was unauthorized, but could not find WWW-Authenticate header.";
        return false;

    }
    else
    {
        ofLogVerbose("CredentialStore::authenticate") << "Response was not unauthorized.  Nothing to be done.";
        return false;
    }
    
}


bool CredentialStore::authenticateWithCache(const AuthScope& scope,
                                            Poco::Net::HTTPRequest& pRequest)
{
    // TODO:
    return true;
}


void CredentialStore::clear()
{
    Poco::FastMutex::ScopedLock lock(mutex);
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

} } }

