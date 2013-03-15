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

#include "ofxHTTPBaseTypes.h"
#include "ofxHTTPServerDefaultRouteHandler.h"

class ofxHTTPServerDefaultRoute : public ofxBaseHTTPServerRoute {
public:
    typedef ofxHTTPServerDefaultRouteSettings Settings;
    typedef ofPtr<ofxHTTPServerDefaultRoute> Ptr;

    ofxHTTPServerDefaultRoute(const Settings& _settings = Settings()) :
    settings(_settings) {
        
        ofDirectory documentRootDirectory(_settings.documentRoot);
        
        if(_settings.bAutoCreateDocumentRoot &&
           !documentRootDirectory.exists()) {
            documentRootDirectory.create();
        }
    }
    
    virtual ~ofxHTTPServerDefaultRoute() { }

    bool canHandleRequest(const HTTPServerRequest& request, bool bIsSecurePort) {
        // require HTTP_GET
        if(request.getMethod() != HTTPRequest::HTTP_GET) return false;
        
        // require a valid path
        URI uri;
        try {
            uri = URI(request.getURI());
        } catch(const SyntaxException& exc) {
            ofLogError("ofxHTTPServerWebSocketRoute::canHandleRequest") << exc.what();
            return false;
        }
        
        // just get the path
        string path = uri.getPath();
        // make paths absolute
        if(path.empty()) { path = "/"; }
        
        return RegularExpression(settings.getRoute()).match(path);
    }

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
        return new ofxHTTPServerDefaultRouteHandler(settings);
    }

    static Ptr Instance(const Settings& settings = Settings()) {
        return Ptr(new ofxHTTPServerDefaultRoute(settings));
    }
    
protected:
    
    Settings settings;
    
};




