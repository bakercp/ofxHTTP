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

#include <set>

#include "Poco/RegularExpression.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/NetException.h"

#include "ofxHTTPServerExchange.h"
#include "ofxHTTPConstants.h"
#include "ofxHTTPCredentials.h"
#include "ofxHTTPResponseStream.h"

using std::set;

using Poco::RegularExpression;
using Poco::Net::HTTPBasicCredentials;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NotAuthenticatedException;

class ofxBaseHTTPServerRoute : public HTTPRequestHandlerFactory {
public:
    typedef ofPtr<ofxBaseHTTPServerRoute> Ptr;
    
    ofxBaseHTTPServerRoute() { }
    virtual ~ofxBaseHTTPServerRoute() { }
    
    virtual bool canHandleRequest(const HTTPServerRequest& request, bool bIsSecurePort) = 0;
    
};

typedef ofPtr<ofxBaseHTTPServerRoute> ofxBaseHTTPServerRoutePtr;

class ofxBaseHTTPRequestHandler : public HTTPRequestHandler {
public:
    ofxBaseHTTPRequestHandler() { }
    virtual ~ofxBaseHTTPRequestHandler() { }
    
    virtual void handleExchange(ofxHTTPServerExchange& exchange) = 0;
    
};

class ofxBaseHTTPServerAuthenticationManager {
public:
    ofxBaseHTTPServerAuthenticationManager() { }
    virtual ~ofxBaseHTTPServerAuthenticationManager() { }
    
    // checkes the request for the correct authentication headers
    virtual ofxHTTPAuthStatus authenticate(HTTPServerRequest& request) = 0;
    
    // writes the required authentication headers to the response
    virtual void setAuthenticationRequiredHeaders(HTTPServerResponse& response)   = 0;
    
};

class ofxBaseHTTPServerBasicAuthenticationManager {
public:
    ofxBaseHTTPServerBasicAuthenticationManager(const string& _realm) : realm(_realm) { }
    virtual ~ofxBaseHTTPServerBasicAuthenticationManager() { }
    
    ofxHTTPAuthStatus authenticate(HTTPServerRequest& request) {
        if(request.hasCredentials()) {
            if(HTTPCredentials::hasBasicCredentials(request)) {
                HTTPBasicCredentials pCredentials(request);
                ofxHTTPCredentials credentials(pCredentials.getUsername(),pCredentials.getPassword());
                if(checkCredentials(credentials)) {
                    return OK;
                } else {
                    return UNAUTHORIZED;
                }
            } else if(HTTPCredentials::hasDigestCredentials(request)) {
                ofLogWarning("ofxBaseHTTPServerBasicAuthenticationManager::authenticate") << "HTTPServerResponse does not currently support DIGEST Auththentication";
                return UNAUTHORIZED;
            } else {
                string scheme;
                string authInfo;

                try {
                    request.getCredentials(scheme, authInfo);
                } catch(const NotAuthenticatedException& exc) {
                    ofLogWarning("ofxBaseHTTPServerBasicAuthenticationManager::authenticate") << "Unrecognizable authentication scheme.";
                    return UNAUTHORIZED;
                }
                
                ofLogWarning("ofxBaseHTTPServerAuthenticationManager::authenticate") << "Unrecognized authentication scheme : " << scheme;
                return UNAUTHORIZED;
            }
        } else {
            return NO_CREDENTIALS;
        }
    }
    
    string getRealm() { return realm; };
    
protected:
    string realm;

    virtual bool checkCredentials(const ofxHTTPCredentials& credentials) = 0;
    
    void setAuthenticationRequiredHeaders(HTTPServerResponse& response) {
        // similar to response.requireAuthentication(realm),
        // but we want to send our own content after the
        // headers have been sent.
        string auth("Basic realm=\"");
        auth.append(realm);
        auth.append("\"");
        response.set("WWW-Authenticate", auth);
    }
};

class ofxBaseHTTPServerSessionManager {
public:
    ofxBaseHTTPServerSessionManager() { }
    virtual ~ofxBaseHTTPServerSessionManager() { }
    virtual bool update(HTTPServerRequest& request, HTTPServerResponse& response) = 0;
};

class ofxBaseHTTPResponseStreamConsumer {
public:
    ofxBaseHTTPResponseStreamConsumer() { }
    virtual ~ofxBaseHTTPResponseStreamConsumer() { }
    // the consumer subclass must free the response stream
    virtual void consume(ofxHTTPResponseStream* reponseStream) = 0;
};

