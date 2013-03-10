#include "ofxHTTPCredentialStore.h"

//------------------------------------------------------------------------------
ofxHTTPCredentialStore::ofxHTTPCredentialStore() { }

//------------------------------------------------------------------------------
ofxHTTPCredentialStore::ofxHTTPCredentialStore(ofxHTTPCredentialStore& that) {
    ofScopedLock thatLock(that.mutex);
    credentialMap = that.credentialMap;
    // cache is not copied
}

//------------------------------------------------------------------------------
ofxHTTPCredentialStore& ofxHTTPCredentialStore::operator = (ofxHTTPCredentialStore& that) {
    ofScopedLock thisLock(mutex);
    ofScopedLock thatLock(that.mutex);
    credentialMap = that.credentialMap;
    // cache is not copied
}

//------------------------------------------------------------------------------
ofxHTTPCredentialStore::ofxHTTPCredentialStore(const ofxHTTPCredentialStore& c) { }

//------------------------------------------------------------------------------
ofxHTTPCredentialStore& ofxHTTPCredentialStore::operator= (const ofxHTTPCredentialStore& c) { }

//------------------------------------------------------------------------------
ofxHTTPCredentialStore::~ofxHTTPCredentialStore() { }

//------------------------------------------------------------------------------
void ofxHTTPCredentialStore::setCredentials(const ofxHTTPAuthScope& scope, const ofxHTTPCredentials& credentials) {
    if(!credentials.hasCredentials()) {
        ofLogWarning("ofxHTTPCredentialStore::setCredentials") << "Credentials are empty.  Ignoring.";
        return;
    }
        
    ofScopedLock lock(mutex);
    credentialMap[scope] = credentials;
    basicCredentialCacheMap.erase(scope); // invalidate cached credentials with the same scope
    digestCredentialCacheMap.erase(scope);
}

//------------------------------------------------------------------------------
void ofxHTTPCredentialStore::setCredentialsFromURI(const URI& uri) {
    string username;
    string password;
    
    string userInfo = uri.getUserInfo();
    
    const std::string::size_type p = userInfo.find(':');
    
	if (p != string::npos) {
		username.assign(userInfo, 0, p);
		password.assign(userInfo, p + 1, string::npos);
	} else {
		username.assign(userInfo);
		password.clear();
	}

    setCredentials(uri,username,password);
}

//------------------------------------------------------------------------------
void ofxHTTPCredentialStore::setCredentials(const URI& uri, const ofxHTTPCredentials& credentials) {
    setCredentials(ofxHTTPAuthScope(uri),credentials);
}

//------------------------------------------------------------------------------
void ofxHTTPCredentialStore::setCredentials(const URI& uri, const string& username, const string& password) {
    setCredentials(ofxHTTPAuthScope(uri),ofxHTTPCredentials(username,password));
}

//------------------------------------------------------------------------------
bool ofxHTTPCredentialStore::hasCredentials(const ofxHTTPAuthScope& targetScope) {
    ofxHTTPCredentials matchingCredentials;
    ofxHTTPAuthScope   matchingScope;
    return getCredentials(targetScope,matchingScope,matchingCredentials);
}

//------------------------------------------------------------------------------
bool ofxHTTPCredentialStore::getCredentials(const ofxHTTPAuthScope& targetScope, ofxHTTPAuthScope& matchingScope, ofxHTTPCredentials& matchingCredentials) {
    ofScopedLock lock(mutex);
    return getCredentialsWithExistingLock(targetScope,matchingScope,matchingCredentials);
}

//------------------------------------------------------------------------------
bool ofxHTTPCredentialStore::getCredentialsWithExistingLock(const ofxHTTPAuthScope& targetScope, ofxHTTPAuthScope& matchingScope, ofxHTTPCredentials& matchingCredentials) {
    // calls to this function are expected to be locked!
    ofxHTTPCredentialMapIter iter = credentialMap.find(targetScope);
    
    cout << "TARGET Scope = " << targetScope.toString() << endl;
    cout << "TARGET CREDZ = " << targetScope.toString() << endl;

    
    
    if(iter != credentialMap.end()) {
        cout << "FOUND DIRECT MATCH" << endl;
        
        cout << "scope = " << (*iter).first.toString() << endl;
        cout << "credz = " << (*iter).second.toString() << endl;

        matchingScope       = (*iter).first;
        matchingCredentials = (*iter).second;
        return true;
    } else {
        int bestMatchFactor  = -1;
        ofxHTTPCredentialMapIter bestMatch = credentialMap.end();
        iter = credentialMap.begin();
        while(iter != credentialMap.end()) {
            int factor = (*iter).first.match(targetScope);
            
            if(factor > bestMatchFactor) {
                bestMatch = iter;
                bestMatchFactor = factor;
            }
            ++iter;
        }
        
        if(bestMatch != credentialMap.end()) {
            cout << "best match scope = " << (*bestMatch).first.toString() << endl;
            cout << "best match credz = " << (*bestMatch).second.toString() << endl;
            
            matchingScope       = (*bestMatch).first;
            matchingCredentials = (*bestMatch).second;
            
            cout << "best match factor=" << bestMatchFactor << endl;
            return true;
        } else {
            cout << "best match factor=" << bestMatchFactor << endl;
            return false;
        }
    }

}

//------------------------------------------------------------------------------
void ofxHTTPCredentialStore::clearCredentials(const ofxHTTPAuthScope& scope) {
    ofScopedLock lock(mutex);
    credentialMap.erase(scope);
    basicCredentialCacheMap.erase(scope);
    digestCredentialCacheMap.erase(scope);
}

//------------------------------------------------------------------------------
void ofxHTTPCredentialStore::clearCredentials(const URI& uri) {
    clearCredentials(ofxHTTPAuthScope(uri));
}

//------------------------------------------------------------------------------
bool ofxHTTPCredentialStore::authenticate(const HTTPClientSession& pSession, HTTPRequest& pRequest) {
    
    // first check and see if the request has any authentication headers
    // these could be added via default session headers
    if(pRequest.has(HTTPRequest::AUTHORIZATION)) {
        ofLogVerbose("ofxHTTPCredentialStore::authenticate") << "HTTP Authorization headers already set.  Skipping authentication.";
        return false;
    }

    ofxHTTPAuthScope    targetScope(pSession.getHost(),pSession.getPort());
    ofxHTTPCredentials  matchingCredentials;
    ofxHTTPAuthScope    matchingScope;
    
    ofScopedLock lock(mutex);

    // mutex locking happens in getCredentials() and authenticateWithCache()
    if(getCredentialsWithExistingLock(targetScope,matchingScope,matchingCredentials)) {
        
        // first search our digest credentials to see if we have a matching scope (preferred)
        ofxHTTPDigestCredentialCacheMapIter iterDigest = digestCredentialCacheMap.find(matchingScope);

        if(iterDigest != digestCredentialCacheMap.end()) {
            (*iterDigest).second->updateAuthInfo(pRequest); // successfully updated auth info for matching scope
            return true;
        }
        
        // if there are no digest credentials, search for basic credentials
        
        ofxHTTPBasicCredentialCacheMapIter iterBasic = basicCredentialCacheMap.find(matchingScope);
        
        if(iterBasic != basicCredentialCacheMap.end()) {
            (*iterBasic).second->authenticate(pRequest); // successfully updated auth info for matching scope
            return true;
        }
        
        ofLogVerbose("ofxHTTPCredentialStore::authenticate") << "Had no matching cached credentials for preemptive authentication.";
        return false;
    } else {
        ofLogVerbose("ofxHTTPCredentialStore::authenticate") << "Had no matching credentials for preemptive authentication.";
        return false;
    }    
}

//------------------------------------------------------------------------------
bool ofxHTTPCredentialStore::authenticate(const HTTPClientSession& pSession, HTTPRequest& pRequest, HTTPResponse& pResponse) {

    if(pResponse.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED) {
        for (HTTPResponse::ConstIterator iter = pResponse.find("WWW-Authenticate"); iter != pResponse.end(); ++iter) {
            ofxHTTPAuthType requestedAuthType;
            
            if(HTTPCredentials::isBasicCredentials(iter->second)) {
                requestedAuthType = BASIC;
                cout << "AUTHENTICATING BASIC: " << iter->second << endl;
            } else if(HTTPCredentials::isDigestCredentials(iter->second)) {
                requestedAuthType = DIGEST;
                cout << "AUTHENTICATING DIGEST: " << iter->second << endl;
            } else {
                ofLogError("ofxHTTPCredentialStore::authenticate") << "Incompatible or unknown WWW-Authenticate header.  Basic and digest supported: " << iter->second;
                return false;
            }

            // extract the realm
            HTTPAuthenticationParams params;
            
            try {
                params.fromResponse(pResponse);
            
            // it is very unlikely that any of the following exceptions will be thrown because we have already checked for most.
            } catch(const NotAuthenticatedException& exc) {
                ofLogError("ofxHTTPCredentialStore::authenticate") << "HTTP response has no authentication header.";
                return false;
            } catch(const InvalidArgumentException& exc) {
                ofLogError("ofxHTTPCredentialStore::authenticate") << "Incompatible or unknown WWW-Authenticate header.  Basic and digest supported: " << iter->second;
                return false;
            } catch (const SyntaxException& exc) {
                ofLogError("ofxHTTPCredentialStore::authenticate") << "Error parsing WWW-Authenticate header: "  << iter->second;
                return false;
            }
            
            
            string scheme = pSession.secure() ? "https" : "http";
            
            ofxHTTPAuthScope    targetScope(scheme,pSession.getHost(),pSession.getPort(),params.getRealm(),requestedAuthType);
            ofxHTTPCredentials  matchingCredentials;
            ofxHTTPAuthScope    matchingScope;
            
            ofScopedLock lock(mutex);
            if(getCredentialsWithExistingLock(targetScope, matchingScope, matchingCredentials)) {
                cout << "HAD CREDS FOR TARGET SCOPE =" << targetScope.toString() << endl;
                cout << "HAD CREDS FOR MATCHING SCOPE =" << matchingScope.toString() << endl;
                if(requestedAuthType == BASIC) {
                    // replace any old ones (probably means they failed and were updated somewhere)
                    basicCredentialCacheMap[matchingScope] = HTTPBasicCredentialsPtr(new HTTPBasicCredentials(matchingCredentials.getUsername(),matchingCredentials.getPassword()));
                    basicCredentialCacheMap[matchingScope].get()->authenticate(pRequest);
                    return true;
                } else if(requestedAuthType == DIGEST) {
                    digestCredentialCacheMap[matchingScope] = HTTPDigestCredentialsPtr(new HTTPDigestCredentials(matchingCredentials.getUsername(),matchingCredentials.getPassword()));
                    digestCredentialCacheMap[matchingScope].get()->authenticate(pRequest,pResponse);
                    return true;
                } else {
                    ofLogError("ofxHTTPCredentialStore::authenticate") << "Unknown requestedAuthType: " << requestedAuthType;
                    return false;
                }
            } else {
                ofLogVerbose("ofxHTTPCredentialStore::authenticate") << "Had no matching credentials for authentication.";
                return false;
            }
        }
        
        ofLogVerbose("ofxHTTPCredentialStore::authenticate") << "Response was unauthorized, but could not find WWW-Authenticate header.";
        return false;

    } else {
        ofLogVerbose("ofxHTTPCredentialStore::authenticate") << "Response was not unauthorized.  Nothing to be done.";
        return false;
    }
    
}

//------------------------------------------------------------------------------
bool ofxHTTPCredentialStore::authenticateWithCache(const ofxHTTPAuthScope& scope, HTTPRequest& pRequest) {
}


//------------------------------------------------------------------------------
void ofxHTTPCredentialStore::clear() {
    ofScopedLock lock(mutex);
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