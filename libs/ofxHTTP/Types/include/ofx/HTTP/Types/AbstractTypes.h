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


class WebSocketFrame;


class AbstractHTTPRequestHandler: public Poco::Net::HTTPRequestHandler
    /// \brief An AbstractHTTPRequestHandler.
    /// \note This layer of abstraction is to make documentation simpler.
{
public:
    virtual ~AbstractHTTPRequestHandler()
        ///< \brief Destroy the AbstractHTTPRequestHandler.
    {
    }

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response) = 0;
        ///< \brief Handle an HTTPServerRequest with an HTTPServerResponse.
        ///< \param request The HTTPServerRequest to handle.
        ///< \param response The HTTPServerResponse to return.
        ///< \note Redeclared here for documentation puposes.

};



class AbstractHTTPRequestHandlerFactory:
    public Poco::Net::HTTPRequestHandlerFactory
    /// \brief An AbstractHTTPRequestHandlerFactory.
    /// \note This layer of abstraction is to make documentation simpler.
{
public:
    virtual ~AbstractHTTPRequestHandlerFactory()
        ///< \brief Destroy the AbstractHTTPRequestHandlerFactory.
    {
    }

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) = 0;
        ///< \brief Creates a new HTTPRequestHandler for the given request.
        ///< \details Before this is called, it is expected that the calling
        ///<        server has confirmed that this route is capable of handling
        ///<        the request by calling canHandleRequest().
        ///< \param request The HTTPServerRequest to be passed to the handler.
        ///< \returns An HTTPRequestHandler that will handle the request.
        ///< \note Redeclared here for documentation puposes.

};


class AbstractWebSocketConnection: public AbstractHTTPRequestHandler
    /// \brief Defines an interface for handling a websocket connection.
{
public:
    virtual ~AbstractWebSocketConnection()
        ///< \brief Destroy the AbstractWebSocketConnection instance.
    {
    }


    virtual bool sendFrame(const WebSocketFrame& frame) const = 0;
        ///< \brief Send a WebSocketFrame using this connection.
        ///< \param frame The WebSocketFrame to send.
        ///< \returns true iff the sending operation was successful.

    virtual void close() = 0;
        ///< \brief Close the connection.

};


class AbstractRouteHandler: public AbstractHTTPRequestHandler
    /// \brief Defines an abstract HTTP route handler.
    /// \details Route handlers are invoked in route handling threads
    ///         created by classes that inherit from AbstractRoute.
{
public:
    typedef std::shared_ptr<AbstractRouteHandler> SharedPtr;
        ///< \brief A typedef for a shared pointer.

    typedef std::weak_ptr<AbstractRouteHandler> WeakPtr;
        ///< \brief A typedef for a weak pointer.

    virtual ~AbstractRouteHandler()
        ///< \brief Destroy the AbstractRouteHandler instance.
    {
    }

};


class AbstractRoute:
    public AbstractHTTPRequestHandler,
    public AbstractHTTPRequestHandlerFactory
    /// \brief Defines an abstract HTTP server route.
{
public:
    typedef std::shared_ptr<AbstractRoute> SharedPtr;
        ///< \brief A typedef for a shared pointer.

    typedef std::weak_ptr<AbstractRoute> WeakPtr;
        ///< \brief A typedef for a weak pointer.

    virtual ~AbstractRoute()
        ///< \brief Destroy the AbstractRoute instance.
    {
    }

    virtual std::string getRoutePathPattern() const = 0;
        ///< \brief Get the route's regex path pattern.
        ///< \details The path pattern is used to match incoming
        ///<        server requests and allows requests to be
        ///<        routed to different routes based on their
        ///<        requested URI.
        ///< \returns the regex path pattern.

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const = 0;
        ///< \brief Determine if this route can handle the given request.
        ///< \param request The incoming Poco::Net::HTTPServerRequest to be
        ///<        tested.
        ///< \param isSecurePort true iff the connection is SSL encrypted.
        ///<        Some implmenetations of this interface may choose to only
        ///<        handle requests on secure ports.
        ///< \returns true iff the route can handle the given request.

    virtual void stop() = 0;
        ///< \brief Stop any pending activity and close this route.
        ///< \details This method may block until the route is fully stopped.

};


} } // namespace ofx::HTTP
