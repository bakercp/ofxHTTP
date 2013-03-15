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

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "ofxHTTPCookieStore.h"
#include "ofxHTTPCredentialStore.h"
//#include "ofxHTTPCredentialStoreCache.h"
#include "ofxHTTPSessionSettings.h"

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

// a thread-safe shared context
// -- in theory all members are thread safe
// -- and no members are reentrant
// setters are not allowed for thread safety.
// if you want a context with a new store, then you have to make one
// via constructors

class ofxHTTPClientContext;

typedef ofPtr<ofxHTTPClientContext> ofxHTTPClientContextPtr;

class ofxHTTPClientContext {
public:
    
    ofxHTTPClientContext();
    
    ofxHTTPClientContext(ofxHTTPSessionSettings& _settings);

    ofxHTTPClientContext(ofxHTTPSessionSettings& _settings,
                         ofxHTTPCredentialStore& _credentialStore);

    ofxHTTPClientContext(ofxHTTPSessionSettings& _settings,
                         ofxHTTPCredentialStore& _credentialStore,
                         ofxHTTPCookieStore& _cookieStore);
    
    ofxHTTPClientContext(ofxHTTPClientContext& that);
	ofxHTTPClientContext& operator = (ofxHTTPClientContext& that);
    
    ~ofxHTTPClientContext();
   
    ofxHTTPSessionSettings getSessionSettings();
    ofxHTTPSessionSettings& getSessionSettingsRef();
    
    // no consts because all of these objects are synchronized and need access to their mutexes
    ofxHTTPCookieStore  getCookieStore();
    ofxHTTPCookieStore& getCookieStoreRef();
    
    ofxHTTPCredentialStore  getCredentialStore();
    ofxHTTPCredentialStore& getCredentialStoreRef();
    
//    bool canAuthenticate(HTTPRequest& request);
//    bool canAuthenticate(HTTPRequest& request, HTTPResponse& response);
//
//    bool authenticate(HTTPRequest& request);
//    bool authenticate(HTTPRequest& request, HTTPResponse& response);
    
//    void setCredentials(const URI& uri, const ofxHTTPCredentials& credentials);
//    void setCredentials(const URI& uri, const string& username, const string& password);
    
    static ofxHTTPClientContextPtr defaultClientContext();
    
private:
    ofxHTTPClientContext(const ofxHTTPClientContext& that);
	ofxHTTPClientContext& operator = (const ofxHTTPClientContext& that);

    // client settings (i.e. thread pools, # connections, etc).
    // vs. context-settings (num redirects, auth handlers, etc).
    
    ofxHTTPSessionSettings      sessionSettings;
    ofxHTTPCredentialStore      credentialStore;
    
    ofxHTTPCookieStore          cookieStore;

};




