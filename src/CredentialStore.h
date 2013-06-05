/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 =============================================================================*/


#pragma once


#include <map>
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPAuthenticationParams.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPDigestCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/NetException.h"
#include "ofLog.h"
#include "ofTypes.h"
#include "AuthScope.h"
#include "Credentials.h"
#include "Utils.h"


namespace ofx {
namespace HTTP {

    
// thread-safe credential store
class CredentialStore {
public:
    typedef ofPtr<Poco::Net::HTTPBasicCredentials>  HTTPBasicCredentialsPtr;
    typedef ofPtr<Poco::Net::HTTPDigestCredentials> HTTPDigestCredentialsPtr;

    CredentialStore();
    CredentialStore(CredentialStore& that);
	CredentialStore& operator= (CredentialStore& that);

    virtual ~CredentialStore();

    bool hasCredentials(const AuthScope& targetScope) const;
    
    bool getCredentials(const AuthScope& targetScope,
                        AuthScope& matchingScope,
                        Credentials& matchingCredentials) const;

    void setCredentials(const AuthScope& scope, const Credentials& credentials);
    
    void setCredentialsFromURI(const Poco::URI& uri);
    void setCredentials(const Poco::URI& uri, const Credentials& credentials);
    void setCredentials(const Poco::URI& uri, const string& username, const string& password);
    
    void clearCredentials(const AuthScope& scope);
    void clearCredentials(const Poco::URI& uri);
    
    bool authenticate(const Poco::Net::HTTPClientSession& pSession,
                      Poco::Net::HTTPRequest& pRequest);
    
    bool authenticate(const Poco::Net::HTTPClientSession& pSession,
                      Poco::Net::HTTPRequest& pRequest,
                      Poco::Net::HTTPResponse& pResponse);
    
    void clear();

protected:
    bool authenticateWithCache(const AuthScope& scope,
                               Poco::Net::HTTPRequest& pRequest);
    // get credentials, with mutex already locked
    bool getCredentialsWithExistingLock(const AuthScope& targetScope,
                                        AuthScope& matchingScope,
                                        Credentials& matchingCredentials) const;
    
private:
    CredentialStore(const CredentialStore& c);
    CredentialStore& operator = (const CredentialStore& c);

    typedef map<AuthScope, Credentials>              CredentialMap;
    typedef map<AuthScope, HTTPBasicCredentialsPtr>  BasicCredentialCacheMap;
    typedef map<AuthScope, HTTPDigestCredentialsPtr> DigestCredentialCacheMap;

    typedef CredentialMap::const_iterator      HTTPCredentialMapIter;
    typedef BasicCredentialCacheMap::iterator  HTTPBasicCredentialCacheMapIter;
    typedef DigestCredentialCacheMap::iterator HTTPDigestCredentialCacheMapIter;

    CredentialMap            credentialMap;
    BasicCredentialCacheMap  basicCredentialCacheMap;
    DigestCredentialCacheMap digestCredentialCacheMap;

    mutable ofMutex mutex;
    
};


} }
