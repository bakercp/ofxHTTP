// =============================================================================
//
// Copyright (c) 2012-2015 Christopher Baker <http://christopherbaker.net>
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


class AbstractSession
{
public:
    /// \brief Destroy the AbstractSession.
    virtual ~AbstractSession()
    {
    }

    /// \brief Get the session id.
    /// \returns the session id.
    virtual std::string getId() const = 0;

    /// \brief Check to see if the provided key exists in the session data.
    /// \param key The data key to check.
    /// \returns true iff the key exists in the session data.
    virtual bool has(const std::string& key) const = 0;

    /// \brief Add (or replace) a key in the sesion data with the given value.
    /// \param key The data key to add.
    /// \param value The data value to add for the given key.
    virtual void put(const std::string& key, const std::string& value) = 0;

    /// \brief Get the session data for a given key, or a default value.
    /// \param key The data key to get.
    /// \param defaultValue The data value to return if the key does not exist.
    /// \returns the data value for the key or the defaultValue if the key does
    ///          not exist.
    virtual std::string get(const std::string& key,
                            const std::string& defaultValue) const = 0;

    /// \brief Get the session data for a given key, or throw an exception.
    /// \param key The data key to get.
    /// \throws Poco::InvalidAccessException if key does not exist.
    /// \returns the data value for the key.
    virtual std::string get(const std::string& key) const = 0;

    /// \brief Remove a key / value pair.
    /// \param key The key to remove.
    virtual void remove(const std::string& key) = 0;

    /// \brief Remove all data from the session.
    virtual void clear() = 0;

};


class ServerEventArgs;
class AbstractSessionStore;


/// \brief Represents an abstract server interface.
///
/// Provides a server event callback and access to the session store.
class AbstractServer: public Poco::Net::HTTPRequestHandlerFactory
{
public:
    /// \brief Destroy the AbstractServer.
    virtual ~AbstractServer()
    {
    }

    virtual void onHTTPServerEvent(const void* pSender, ServerEventArgs& evt) = 0;

    /// \brief Return a reference to the session cache.
    virtual AbstractSessionStore& getSessionStore() = 0;

};


/// \brief An abstract Server Event request handler.
class AbstractServerEventRequestHandler
{
public:
    /// \brief Destroy the AbstractServerEventRequestHandler.
    virtual ~AbstractServerEventRequestHandler()
    {
    }

    /// \brief Handle the ServerEventArgs.
    /// \param evt The server request event.
    virtual void handleRequest(ServerEventArgs& evt) = 0;

};


/// \brief An AbstractHTTPRequestHandler.
/// \note This layer of abstraction is to make documentation simpler.
class AbstractHTTPRequestHandler:
    public Poco::Net::HTTPRequestHandler,
    public AbstractServerEventRequestHandler
{
public:
    /// \brief Destroy the AbstractHTTPRequestHandler.
    virtual ~AbstractHTTPRequestHandler()
    {
    }

};


/// \brief An abstract class representing a session store.
///
/// A session store is responsible for establishing browsing sessions.  This
/// includes but is not limited to sessions created using HTTP cookies.
class AbstractSessionStore
{
public:
    /// \brief Destroy the AbstractSessionStore.
    virtual ~AbstractSessionStore()
    {
    }

    /// \brief Get a valid session for the given request.
    ///
    /// This method is guaranteed to provide a valid session the session created
    /// will expire when teh browser window is closed. Other method will be able
    /// to update the cookie in the response headers if more speific cookie
    /// parameters are desired.
    ///
    /// This method will also ensure that only one session id is availble at a
    /// given time
    ///
    /// \param request The HTTP request.
    /// \param response The HTTP response.
    /// \returns A reference to an AbstractSession.
    virtual AbstractSession& getSession(Poco::Net::HTTPServerRequest& request,
                                        Poco::Net::HTTPServerResponse& response) = 0;

    /// \brief Destroy the session(s) associated with the given exchange.
    ///
    /// This method will invalidate any session cookies in the response header.
    /// If session data does not exist, the method will return quietly.
    virtual void destroySession(Poco::Net::HTTPServerRequest& request,
                                Poco::Net::HTTPServerResponse& response) = 0;

protected:
    /// \brief Query if the store has the the given session
    /// \param sessionId The id of the session to query.
    virtual bool hasSession(const std::string& sessionId) const = 0;

    /// \brief Get a session by \p sesssionId.
    /// \param sessionId The id of the session to get.
    /// \throws Poco::InvalidAccessException if no session is found.
    virtual AbstractSession& getSession(const std::string& sessionId) = 0;

    /// \brief Create a completely new session.
    ///
    /// Since this returns a reference, the newly created session must be stored
    /// outside of the local method scope so the reference does not become
    /// invalid.
    ///
    /// \returns A reference to the newly created session.
    virtual AbstractSession& createSession() = 0;

    /// \brief Destroy the record of a session by its session id.
    ///
    /// If session data does not exist, the method will return quietly.
    ///
    /// \param sessionId The id of the session to destroy.
    virtual void destroySession(const std::string& sessionId) = 0;

};


//class Credentials;
//
//
//class AbstractSessionAuthenticator
//{
//public:
//    virtual ~AbstractSessionAuthenticator()
//    {
//    }
//
//    virtual bool authenticate(const std::string& sessionId,
//                              const Credentials& credentials) = 0;
//
//    virtual void deauthenticate(const std::string& sessionId) = 0;
//
//    virtual bool isAuthenticated(const std::string& sessionId) = 0;
//
//};


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


class AbstractRoute;


/// \brief Defines an abstract HTTP route handler.
/// Route handlers are invoked in route handling threads
/// created by classes that inherit from AbstractRoute.
class AbstractRouteHandler: public AbstractHTTPRequestHandler
{
public:
    /// \brief Destroy the AbstractRouteHandler instance.
    virtual ~AbstractRouteHandler()
    {
    }

    /// \brief Interrupt the handleRequest method if possible.
    virtual void stop() = 0;

};


/// \brief Defines an abstract HTTP server route.
class AbstractRoute:
    public AbstractHTTPRequestHandler,
    public AbstractHTTPRequestHandlerFactory
{
public:
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
    ///
    /// This method may block until the route is fully stopped.
    virtual void stop() = 0;

    /// \brief Get a pointer to the server.
    ///
    /// \returns nullptr pointer if no server is specified.
    virtual AbstractServer* getServer() = 0;

    /// \brief Set the parent server.
    /// \param server A pointer to the parent server.
    virtual void setServer(AbstractServer* server) = 0;

};


class WebSocketFrame;


/// \brief Represents an abstract WebSocketFrame filter.
///
/// Can modify incoming and outgoing WebSocketFrames.
class AbstractWebSocketFilter
{
public:
    /// \brief Destroy the AbstractWebSocketFilter.
    virtual ~AbstractWebSocketFilter()
    {
    }

    /// \brief Process a frame that was received.
    /// \param frame A reference to the frame that was recieved.
    /// \note The frame contents and flags may be modified.
    virtual void receiveFilter(WebSocketFrame& frame) = 0;

    /// \brief Process a frame before sending.
    /// \param frame A reference to the frame that will be sent.
    /// \note The frame contents and flags may be modified.
    virtual void sendFilter(WebSocketFrame& frame) = 0;

};



/// \brief Represents an abstract WebSocketExtension factory.
///
/// Can process and respond to headers in WebSocket Upgrade request. Returns an
/// approprately configured AbstractWebSocketFilter for use with a
/// WebSocketConnection.
class AbstractWebSocketFilterFactory
{
public:
    /// \brief Destroy the AbstractWebSocketFilterFactory.
    virtual ~AbstractWebSocketFilterFactory()
    {
    }

    /// \brief Create an AbstractWebSocketFilter based on the ServerEventArgs.
    ///
    /// This function will set the appropriate response headers in the
    /// evt.getResponse() if applicable.
    ///
    /// The function will return an AbstractWebSocketFilter or a nullptr if no
    /// matching AbstractWebSocketFilter can be configured.
    ///
    /// \param evt The server request and response.
    /// \returns an AbstractWebSocketFilter or nullptr if none could be created.
    virtual std::unique_ptr<AbstractWebSocketFilter> makeFilterForRequest(ServerEventArgs& evt) const = 0;

};


} } // namespace ofx::HTTP
