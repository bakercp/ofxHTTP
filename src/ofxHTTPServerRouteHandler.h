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

#include <string>

#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/NameValueCollection.h"

#include "ofLog.h"
#include "ofUtils.h"

#include "ofxHTTPBaseTypes.h"
#include "ofxHTTPServerExchange.h"
#include "ofxHttpUtils.h"
#include "ofxMediaTypes.h"

using std::string;

using Poco::URI;
using Poco::FileNotFoundException;
using Poco::OpenFileException;
using Poco::Path;
using Poco::Net::HTTPCookie;
using Poco::Net::HTTPBasicCredentials;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;

//------------------------------------------------------------------------------
class ofxHTTPServerRouteHandler : public ofxBaseHTTPRequestHandler {
public:    
    ofxHTTPServerRouteHandler();
    virtual ~ofxHTTPServerRouteHandler();
    
    // overriden from HTTPRequestHandler
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

protected:

    // authenticate is expected to check authentication and also
    // set any response headers if authentication has failed.
    virtual ofxHTTPAuthStatus authenticate(ofxHTTPServerExchange& exchange) { return OK; }
    virtual void updateSession(ofxHTTPServerExchange& exchange) { };
    
    virtual void handleExchange(ofxHTTPServerExchange& exchange);
    
    virtual void sendErrorResponse(HTTPServerResponse& response);

};
