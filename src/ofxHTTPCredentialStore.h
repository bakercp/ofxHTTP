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
 
 ==============================================================================*/

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

#include "ofxHTTPConstants.h"
#include "ofxHTTPAuthScope.h"
#include "ofxHTTPCredentials.h"
#include "ofxHTTPUtils.h"

using std::map;

using Poco::icompare;
using Poco::InvalidArgumentException;
using Poco::NotImplementedException;
using Poco::SyntaxException;
using Poco::URI;
using Poco::Net::HTTPAuthenticationParams;
using Poco::Net::HTTPBasicCredentials;
using Poco::Net::HTTPCredentials;
using Poco::Net::HTTPDigestCredentials;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPClientSession;
using Poco::Net::NotAuthenticatedException;

// TODO: perhaps not ofPtr, but auto_ptr via Poco since we don't share this
typedef ofPtr<HTTPBasicCredentials>  HTTPBasicCredentialsPtr;
typedef ofPtr<HTTPDigestCredentials> HTTPDigestCredentialsPtr;

typedef map<ofxHTTPAuthScope, ofxHTTPCredentials> ofxHTTPCredentialMap;
typedef map<ofxHTTPAuthScope, HTTPBasicCredentialsPtr>  ofxHTTPBasicCredentialCacheMap;
typedef map<ofxHTTPAuthScope, HTTPDigestCredentialsPtr> ofxHTTPDigestCredentialCacheMap;

typedef ofxHTTPCredentialMap::const_iterator      ofxHTTPCredentialMapIter;
typedef ofxHTTPBasicCredentialCacheMap::iterator  ofxHTTPBasicCredentialCacheMapIter;
typedef ofxHTTPDigestCredentialCacheMap::iterator ofxHTTPDigestCredentialCacheMapIter;

// thread-safe credential store
class ofxHTTPCredentialStore {
public:
    ofxHTTPCredentialStore();
    ofxHTTPCredentialStore(ofxHTTPCredentialStore& that);
	ofxHTTPCredentialStore& operator= (ofxHTTPCredentialStore& that);

    virtual ~ofxHTTPCredentialStore();

    bool hasCredentials(const ofxHTTPAuthScope& targetScope);
    
    bool getCredentials(const ofxHTTPAuthScope& targetScope, ofxHTTPAuthScope& matchingScope, ofxHTTPCredentials& matchingCredentials);
    void setCredentials(const ofxHTTPAuthScope& scope, const ofxHTTPCredentials& credentials);
    
    void setCredentialsFromURI(const URI& uri);
    void setCredentials(const URI& uri, const ofxHTTPCredentials& credentials);
    void setCredentials(const URI& uri, const string& username, const string& password);
    
    void clearCredentials(const ofxHTTPAuthScope& scope);
    void clearCredentials(const URI& uri);
    
    bool authenticate(const HTTPClientSession& pSession, HTTPRequest& pRequest);
    bool authenticate(const HTTPClientSession& pSession, HTTPRequest& pRequest, HTTPResponse& pResponse);
    
    void clear();

protected:
    bool authenticateWithCache(const ofxHTTPAuthScope& scope, HTTPRequest& pRequest);
    // get credentials, with mutex already locked
    bool getCredentialsWithExistingLock(const ofxHTTPAuthScope& targetScope, ofxHTTPAuthScope& matchingScope, ofxHTTPCredentials& matchingCredentials);
    
private:
    ofxHTTPCredentialStore(const ofxHTTPCredentialStore& c);
    ofxHTTPCredentialStore& operator = (const ofxHTTPCredentialStore& c);
    
    ofxHTTPCredentialMap            credentialMap;
    ofxHTTPBasicCredentialCacheMap  basicCredentialCacheMap;
    ofxHTTPDigestCredentialCacheMap digestCredentialCacheMap;

    ofMutex mutex;
    
};