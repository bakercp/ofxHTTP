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
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "ofTypes.h"


namespace ofx {
namespace HTTP {


class AbstractInterruptible
{
public:
    AbstractInterruptible()
    {
    }

    virtual ~AbstractInterruptible()
    {
    }

    virtual void stop() = 0;
    
};


class WebSocketFrame;


class AbstractWebSocketConnection:
    public AbstractInterruptible
{
public:
    AbstractWebSocketConnection()
    {
    }

    virtual ~AbstractWebSocketConnection()
    {
    }

    virtual bool sendFrame(const WebSocketFrame& frame) const = 0;

};


class AbstractRouteHandler:
    public AbstractInterruptible,
    public Poco::Net::HTTPRequestHandler
{
public:
    typedef std::shared_ptr<AbstractRouteHandler> SharedPtr;
    typedef std::weak_ptr<AbstractRouteHandler> WeakPtr;

    
    AbstractRouteHandler()
    {
    }

    virtual ~AbstractRouteHandler()
    {
    }

};


class AbstractRoute:
    public AbstractInterruptible,
    public Poco::Net::HTTPRequestHandler,
    public Poco::Net::HTTPRequestHandlerFactory
{
public:
    typedef std::shared_ptr<AbstractRoute> SharedPtr;
    typedef std::weak_ptr<AbstractRoute> WeakPtr;

    AbstractRoute()
    {
    }

    virtual ~AbstractRoute()
    {
    }

    virtual std::string getRoutePathPattern() const = 0;

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const = 0;


};


} } // namespace ofx::HTTP
