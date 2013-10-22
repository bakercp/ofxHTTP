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
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/RegularExpression.h"
#include "Poco/URI.h"
#include "ofLog.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/Server/BaseRouteHandler.h"
#include "ofx/HTTP/Server/BaseRouteSettings.h"


namespace ofx {
namespace HTTP {


class BaseRoute: public AbstractRoute
{
public:
    BaseRoute();

    virtual ~BaseRoute();

    virtual std::string getRoutePathPattern() const;

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const;

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response);

    virtual void stop();

private:
    BaseRoute(const BaseRoute&);
	BaseRoute& operator = (const BaseRoute&);

};


} } // namespace ofx::HTTP
