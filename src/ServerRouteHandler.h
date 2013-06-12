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
#include "BaseTypes.h"
#include "ServerExchange.h"
#include "Utils.h"
#include "MediaTypeMap.h"
#include "Authentication.h"


namespace ofx {
namespace HTTP {
        
        
//------------------------------------------------------------------------------
class BaseServerRouteHandler : public Poco::Net::HTTPRequestHandler {
public:
    BaseServerRouteHandler(const BaseRouteSettings& settings);

    virtual ~BaseServerRouteHandler();
    
    // overriden from HTTPRequestHandler
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

protected:
    // authenticate is expected to check authentication and also
    // set any response headers if authentication has failed.
    virtual Authentication::Status authenticate(ServerExchange& exchange)
    {
        return Authentication::OK;
    }

    virtual void updateSession(ServerExchange& exchange)
    {
    }

    virtual void handleExchange(ServerExchange& exchange);

    virtual void sendErrorResponse(Poco::Net::HTTPServerResponse& response);

};


} }
