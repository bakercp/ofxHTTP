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

#include "ofxHTTPServerBaseRoute.h"
#include "ofxHTTPServerUploadRouteHandler.h"

typedef ofxHTTPServerUploadRouteHandler::Settings Settings; // to keep the lines shorter.

//------------------------------------------------------------------------------
class ofxHTTPServerUploadRoute : public ofxHTTPServerBaseRoute {
public:

    ofxHTTPServerUploadRoute(const Settings& _settings) : settings(_settings) {
        ofDirectory uploadsDirectory(settings.uploadFolder);
        if(!uploadsDirectory.exists()) {
            uploadsDirectory.create();
        }
    }

    virtual ~ofxHTTPServerUploadRoute() { }
    
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
        if(ofxHTTPServerUploadRouteHandler::matchRoute(URI(request.getURI()), settings.route)) {
            return new ofxHTTPServerUploadRouteHandler(settings);
        } else {
            return NULL;
        }
    }
    
    static ofPtr<ofxHTTPServerUploadRoute> Instance(const Settings& settings = Settings::Settings()) {
        return ofPtr<ofxHTTPServerUploadRoute>(new ofxHTTPServerUploadRoute(settings));
    }

protected:
    Settings::Settings settings;
    
};




