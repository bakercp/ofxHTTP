// =============================================================================
//
// Copyright (c) 2012 Christopher Baker <http://christopherbaker.net>
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
#include "Poco/UUID.h"
#include "Poco/Any.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "ofTypes.h"


namespace ofx {
namespace HTTP {


class SessionCache;
class WebSocketFrame;


class AbstractServer: public Poco::Net::HTTPRequestHandlerFactory
{
public:
    /// \brief Destroy the AbstractHTTPRequestHandler.
    virtual ~AbstractServer()
    {
    }

    /// \brief Return the session cache.
    virtual SessionCache& getSessionCache() = 0;

};


/// \brief An AbstractHTTPRequestHandler.
/// \note This layer of abstraction is to make documentation simpler.
class AbstractHTTPRequestHandler: public Poco::Net::HTTPRequestHandler
{
public:
    /// \brief Destroy the AbstractHTTPRequestHandler.
    virtual ~AbstractHTTPRequestHandler()
    {
    }

    /// \brief Handle an HTTPServerRequest with an HTTPServerResponse.
    /// \param request The HTTPServerRequest to handle.
    /// \param response The HTTPServerResponse to return.
    /// \note Redeclared here for documentation puposes.
    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response) = 0;

};


/// \brief An AbstractHTTPRequestHandlerFactory.
/// \note This layer of abstraction is to make documentation simpler.
class AbstractHTTPRequestHandlerFactory:
    public Poco::Net::HTTPRequestHandlerFactory
{
public:
    /// \brief Destroy the AbstractHTTPRequestHandlerFactory.
    virtual ~AbstractHTTPRequestHandlerFactory()
    {
    }

    /// \brief Creates a new HTTPRequestHandler for the given request.
    ///
    /// Before this is called, it is expected that the calling
    /// server has confirmed that this route is capable of handling
    /// the request by calling canHandleRequest().
    ///
    /// \param request The HTTPServerRequest to be passed to the handler.
    /// \returns An HTTPRequestHandler that will handle the request.
    /// \note Redeclared here for documentation puposes.
    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) = 0;

};


/// \brief Defines an abstract HTTP route handler.
/// Route handlers are invoked in route handling threads
/// created by classes that inherit from AbstractRoute.
class AbstractRouteHandler: public AbstractHTTPRequestHandler
{
public:
    /// \brief A typedef for a shared pointer.
    typedef std::shared_ptr<AbstractRouteHandler> SharedPtr;

    /// \brief A typedef for a weak pointer.
    typedef std::weak_ptr<AbstractRouteHandler> WeakPtr;

    /// \brief Destroy the AbstractRouteHandler instance.
    virtual ~AbstractRouteHandler()
    {
    }

};


/// \brief Defines an interface for handling a websocket connection.
class AbstractWebSocketConnection: public AbstractRouteHandler
{
public:
    /// \brief Destroy the AbstractWebSocketConnection instance.
    virtual ~AbstractWebSocketConnection()
    {
    }

    /// \brief Send a WebSocketFrame using this connection.
    /// \param frame The WebSocketFrame to send.
    /// \returns true iff the sending operation was successful.
    virtual bool sendFrame(const WebSocketFrame& frame) const = 0;

    /// \brief Close the connection.
    virtual void close() = 0;

};


/// \brief Defines an abstract HTTP server route.
class AbstractRoute:
    public AbstractHTTPRequestHandler,
    public AbstractHTTPRequestHandlerFactory
{
public:
    /// \brief A typedef for a shared pointer.
    typedef std::shared_ptr<AbstractRoute> SharedPtr;

    /// \brief A typedef for a weak pointer.
    typedef std::weak_ptr<AbstractRoute> WeakPtr;

    /// \brief Destroy the AbstractRoute instance.
    virtual ~AbstractRoute()
    {
    }

    /// \brief Get the route's regex path pattern.
    ///
    /// The path pattern is used to match incoming server requests and allows
    /// requests to be routed to different routes based on their requested URI.
    ///
    /// \returns the regex path pattern.
    virtual std::string getRoutePathPattern() const = 0;

    /// \brief Determine if this route can handle the given request.
    /// \param request The incoming Poco::Net::HTTPServerRequest to be tested.
    /// \param isSecurePort true iff the connection is SSL encrypted.
    ///        Some implmenetations of this interface may choose to only
    ///        handle requests on secure ports.
    /// \returns true iff the route can handle the given request.
    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const = 0;

    /// \brief Stop any pending activity and close this route.
    /// \details This method may block until the route is fully stopped.
    virtual void stop() = 0;

    virtual AbstractServer* getServer() = 0;

    virtual void setServer(AbstractServer* server) = 0;

};


class AbstractSessionData
{
public:
    virtual ~AbstractSessionData()
    {
    }

    virtual const Poco::UUID& getId() const = 0;

    virtual const Poco::Timestamp getLastModified() const = 0;

    virtual bool has(const std::string& hashKey) const = 0;

    virtual void put(const std::string& hashKey, const Poco::Any& hashValue) = 0;

    virtual Poco::Any get(const std::string& hashKey, const Poco::Any& defaultValue) const = 0;

};


} } // namespace ofx::HTTP
