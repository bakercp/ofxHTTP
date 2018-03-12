//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


/// \brief An unfortunate compromise until C++11.
#define INIT_SET_WITH_ARRAY(x) x, x + sizeof(x) / sizeof(x[0])


#include <set>
#include <string>

#include "Poco/RegularExpression.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/MediaType.h"

#include "ofLog.h"

#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/RequestHandlerAdapter.h"
#include "ofx/HTTP/ServerEvents.h"


namespace ofx {
namespace HTTP {


/// \brief A base implementation of route settings.
///
/// These settings define the basic information needed to implement
/// BaseRoute::canHandleRequest() method.
class BaseRouteSettings
{
public:
    /// \brief A typedef for HTTPMethodSet.
    typedef std::set<std::string> HTTPMethodSet;

    /// \brief A typedef for a MediaTypeSet.
    typedef std::set<std::string> MediaTypeSet;

    /// \brief Create the BaseRouteSettings with the given route path.
    /// \param routePathPattern The regex pattern that this route will handle.
    /// \param requireSecurePort True if this route requires
    ///        communication on an SSL encrypted port.
    /// \param requireAuthentication Require authenticated requests.
    /// \param validHTTPMethods The valid HTTP Methods that this route
    ///        will handle.
    BaseRouteSettings(const std::string& routePathPattern = BaseRouteSettings::DEFAULT_ROUTE_PATH_PATTERN,
                      bool requireSecurePort = false,
                      bool requireAuthentication = false,
                      const HTTPMethodSet& validHTTPMethods = HTTPMethodSet());

    /// \brief Destroy the BaseRoutSettings.
    virtual ~BaseRouteSettings();

    /// \brief Set the route path regex pattern.
    /// \param routePathPattern The regex pattern that this route will handle.
    void setRoutePathPattern(const std::string& routePathPattern);

    /// \returns The regex pattern that this route handles.
    const std::string& getRoutePathPattern() const;

    /// \brief Set the secure port requirement.
    /// \param requireSecurePort Set to true if this route can only handle
    ///        requests submitted on an SSL encrypted port.
    void setRequireSecurePort(bool requireSecurePort);

    /// \returns true iff route requires communication on an SSL encrypted port.
    bool requireSecurePort() const;

    /// \brief Set the authentication requirement.
    /// \param requireAuthentication Set to true if this route requires
    ///        authentication.
    void setRequireAuthentication(bool requireAuthentication);

    /// \returns true iff route requires server authentication.
    bool requireAuthentication() const;

    /// \brief Set the list of valid HTTPMethods.
    /// \param validHTTPMethods A set of valid HTTPMethods.
    /// \note  An empty set means that any requested HTTPMethod will be
    ///        accepted.  A non-empty set means that the requested
    ///        HTTPMethod MUST be in the set.
    void setValidHTTPMethods(const HTTPMethodSet& validHTTPMethods);

    /// \returns The set of valid HTTP methods.
    /// \note If empty, all requested HTTP methods will be accepted.
    const HTTPMethodSet& getValidHTTPMethods() const;

    /// \brief Set the list of valid Content Types.
    /// \param validContentTypes A set of valid content types.
    /// \note  An empty set means that any requested Content-Type will be
    ///        accepted.  A non-empty set means that the requested
    ///        The Content-Type MUST be in the set.
    void setValidContentTypes(const MediaTypeSet& validContentTypes);

    /// \returns the Set of valid request content types.
    /// \note If Empty, all requested content types will be accepted.
    const MediaTypeSet& getValidContentTypes() const;

    /// \brief The default route path regex pattern.
    /// \details By default, this pattern matches all requests.
    static const std::string DEFAULT_ROUTE_PATH_PATTERN;

private:
    /// \brief the route's regex route pattern.
    std::string _routePathPattern;

    /// \brief true if route requires requests on an SSL encrypted port.
    bool _requireSecurePort;

    /// \brief true if route requires authentication.
    bool _requireAuthentication;

    /// \brief A set of valid HTTP methods.
    HTTPMethodSet _validHTTPMethods;

    /// \brief A set of valid Content-Type variables.
    MediaTypeSet _validContentTypes;

};


/// \brief The base implmentation of a server route.
/// \tparam SettingsType The settings used for this templated route.
template <typename SettingsType>
class BaseRoute_: public AbstractRoute
{
public:
    /// \brief Create a BaseRoute.
    /// \param settings The settings to use for route configuration.
    BaseRoute_(const SettingsType& settings = SettingsType());

    /// \brief Destroy a BaseRoute.
    virtual ~BaseRoute_();

    /// \brief Setup the route with settings.
    /// \param settings The seetings to use for setup.
    virtual void setup(const SettingsType& settings);

    virtual std::string routePathPattern() const override;

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const override;

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override;

    virtual void handleRequest(ServerEventArgs& evt) override;

    virtual void stop() override;

    /// \returns the route's Settings.
    const SettingsType& settings() const;

    /// \returns a pointer to the server that owns this route.
    AbstractServer* getServer() override;

    /// \brief Set the server that owns this route.
    /// \param server A pointer to the server that owns this route.
    void setServer(AbstractServer* server) override;

protected:
    /// \brief The settings.
    SettingsType _settings;

    /// \brief A pointer to the server.
    AbstractServer* _server = nullptr;

private:
    BaseRoute_(const BaseRoute_&);
    BaseRoute_& operator = (const BaseRoute_&);

};


typedef BaseRoute_<BaseRouteSettings> DefaultRoute;


template <typename SettingsType>
BaseRoute_<SettingsType>::BaseRoute_(const SettingsType& settings):
    _settings(settings),
    _server(nullptr)
{
}


template <typename SettingsType>
BaseRoute_<SettingsType>::~BaseRoute_()
{
}


template <typename SettingsType>
void BaseRoute_<SettingsType>::setup(const SettingsType& settings)
{
    _settings = settings;
}


template <typename SettingsType>
std::string BaseRoute_<SettingsType>::routePathPattern() const
{
    return _settings.getRoutePathPattern();
}


template <typename SettingsType>
bool BaseRoute_<SettingsType>::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                                bool isSecurePort) const
{
    // If this isn't a secure port and we require that, reject it.
    if (_settings.requireSecurePort() && !isSecurePort)
    {
        return false;
    }

    // Check the request method.
    const BaseRouteSettings::HTTPMethodSet& validHTTPMethods = _settings.getValidHTTPMethods();

    // If validHTTPMethods are defined, then the request must match.
    if (!validHTTPMethods.empty() && validHTTPMethods.find(request.getMethod()) == validHTTPMethods.end())
    {
        return false;
    }

    // Check the request content type.
    const BaseRouteSettings::MediaTypeSet& validContentTypes = _settings.getValidContentTypes();

    if (!validContentTypes.empty())
    {
        BaseRouteSettings::MediaTypeSet::iterator iter = validContentTypes.begin();

        const std::string& contentType = request.getContentType();

        bool foundMatch = false;

        while (iter != validContentTypes.end())
        {
            Poco::Net::MediaType type(*iter);

            if (type.matchesRange(contentType))
            {
                foundMatch = true;
                break;
            }

            ++iter;
        }

        if (!foundMatch)
        {
            return false;
        }
    }

    // require a valid path
    std::string path = "/";

    try
    {
        path = Poco::URI(request.getURI()).getPath();
    }
    catch (const Poco::SyntaxException& exc)
    {
        ofLogError("BaseRoute::canHandleRequest") << exc.displayText();
        return false;
    }

    if (path.empty())
    {
        path = "/";
    }

    try
    {
        // \TODO cache this regex
        return Poco::RegularExpression(routePathPattern()).match(path);
    }
    catch (const Poco::RegularExpressionException& exc)
    {
        ofLogError("BaseRoute::canHandleRequest") << exc.displayText();
        return false;
    }

}


template <typename SettingsType>
Poco::Net::HTTPRequestHandler* BaseRoute_<SettingsType>::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
    // A route handler adapter adapts the factory class (e.g. the BaseRoute_)
    // to act as a reusable instance. The instance passed to the RouteHandler
    // adapter should not modify the internal state of the route itself.
    return new RequestHandlerAdapter(*this);
}


template <typename SettingsType>
void BaseRoute_<SettingsType>::handleRequest(Poco::Net::HTTPServerRequest& request,
                                             Poco::Net::HTTPServerResponse& response)
{
//    std::cout << "BaseRoute_<SettingsType>::handleRequest" << std::endl;
    // We assert that the server must be set on a route.
    poco_assert(_server);

    // All requests pass through the server's handleRequest method first.
    // The server broadcasts the request / response to any listeners.
    // Generally these listeners should only modify headers, not send a response.
    AbstractSession& session = _server->sessionStore().getSession(request, response);

    ServerEventArgs evt(request, response, session);

    _server->onHTTPServerEvent(this, evt);

    // If the response was sent from the server or its delegates, we finish.
    if (response.sent())
    {
        return;
    }
    else
    {
        // Otherwise, we call the route's sendResponse method.
        // Eventually we may end up back at this base class' sendResponse
        // method.
        handleRequest(evt);
    }

    // A failsafe.
    if (!response.sent())
    {
        BaseRoute_<SettingsType>::handleRequest(evt);
    }
}


template <typename SettingsType>
void BaseRoute_<SettingsType>::handleRequest(ServerEventArgs& evt)
{
//    std::cout << "BaseRoute_<SettingsType>::handleRequest(ServerEventArgs& evt" << std::endl;

    if (evt.response().sent())
    {
        return;
    }

    // We gave the handlers every opportunity to send a response.
    // Now we must conclude that there is a server error.
    try
    {
        // If we got this far and our status is still marked as 200,
        // that constitutes a server error.
        if (Poco::Net::HTTPResponse::HTTP_OK == evt.response().getStatus())
        {
            evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR,
                                              "No handlers for route.");
        }

        evt.response().setChunkedTransferEncoding(true);
        evt.response().setContentType("text/html");

        std::stringstream ss;
        ss << evt.response().getStatus() << " - " << evt.response().getReason();

        std::ostream& ostr = evt.response().send(); // get output stream
        ostr << "<!DOCTYPE html><html><head><meta charset=\"utf-8\"/><title>";
        ostr << ss.str();
        ostr << "</title></head><body><h1>";
        ostr << ss.str();
        ostr << "</h1></body></html>";
        ostr.flush();
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("BaseRoute::handleRequest") << "Exception: " << exc.code() << " " << exc.displayText();
    }
    catch (const std::exception& exc)
    {
        ofLogError("BaseRoute::handleRequest") << "exception: " << exc.what();
    }
    catch ( ... )
    {
        ofLogError("BaseRoute::handleRequest") << "... Unknown exception.";
    }
}


template <typename SettingsType>
void BaseRoute_<SettingsType>::stop()
{
    // empty
}


template <typename SettingsType>
const SettingsType& BaseRoute_<SettingsType>::settings() const
{
    return _settings;
}


template <typename SettingsType>
AbstractServer* BaseRoute_<SettingsType>::getServer()
{
    return _server;
}


template <typename SettingsType>
void BaseRoute_<SettingsType>::setServer(AbstractServer* server)
{
    _server = server;
}


/// \brief The base implmentation of a server route.
template <typename RouteType>
class BaseRouteHandler_: public AbstractRouteHandler
{
public:
    /// \brief Create a BaseRoute.
    BaseRouteHandler_(RouteType& route);

    /// \brief Destroy a BaseRoute.
    virtual ~BaseRouteHandler_();

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override;

    virtual void handleRequest(ServerEventArgs& evt) override = 0;

    virtual void stop() override;

    /// \brief Get the dispatching route.
    RouteType& route();

protected:
    /// \brief The route.
    RouteType& _route;

private:
    BaseRouteHandler_(const BaseRouteHandler_&);
    BaseRouteHandler_& operator = (const BaseRouteHandler_&);

};


template <typename RouteType>
BaseRouteHandler_<RouteType>::BaseRouteHandler_(RouteType& route):
    _route(route)
{
}


template <typename RouteType>
BaseRouteHandler_<RouteType>::~BaseRouteHandler_()
{
}


template <typename RouteType>
void BaseRouteHandler_<RouteType>::handleRequest(Poco::Net::HTTPServerRequest& request,
                                                 Poco::Net::HTTPServerResponse& response)
{
    // We assert that the server must be set on a route.
    poco_assert(route().getServer());

    // All requests pass through the server's handleRequest method first.
    // The server broadcasts the request / response to any listeners.
    // Generally these listeners should only modify headers, not send a response.
    AbstractSession& session = route().getServer()->sessionStore().getSession(request, response);

    ServerEventArgs evt(request, response, session);

    route().getServer()->onHTTPServerEvent(this, evt);

    // If the response was sent from the server or its delegates, we finish.
    if (response.sent())
    {
        return;
    }
    else
    {
        // Otherwise, we call the route's sendResponse method.
        // Eventually we may end up back at this base class' sendResponse
        // method.
        handleRequest(evt);
    }

    // A failsafe.
    if (!response.sent())
    {
        route().handleRequest(evt);
    }
}


template <typename RouteType>
void BaseRouteHandler_<RouteType>::stop()
{
}


template <typename RouteType>
RouteType& BaseRouteHandler_<RouteType>::route()
{
    return _route;
}



template <typename RouteType, typename FrameType>
class BaseConnection_: public BaseRouteHandler_<RouteType>
{
public:
    /// \brief Create a BaseConnection_.
    /// \param route A reference to the parent route.
    BaseConnection_(RouteType& route);

    /// \brief Destroy the BaseConnection_.
    virtual ~BaseConnection_();

    void handleRequest(ServerEventArgs& evt) override;

    void stop() override;

    /// \brief Queue data to be sent to the client.
    /// \param frame The data to send to the client.
    /// \param cache True if the frame should be cached.
    /// \returns false iff frame was not queued.
    bool send(const FrameType& frame) const;

    /// \returns The original http request headers.
    Poco::Net::NameValueCollection requestHeaders() const;

    /// \returns the client's SocketAddress.
    Poco::Net::SocketAddress clientAddress() const;

    /// \returns true iff this BaseConnection_ is connected to a client.
    bool isConnected() const;

    /// \returns the size of the send queue.
    std::size_t sendQueueSize() const;

    /// \brief Clears the send queue.
    void clearSendQueue();

    /// \brief Get the total bytes sent to the client.
    /// \returns the total bytes sent to the client.
    std::size_t totalBytesSent() const;

protected:
    /// \brief The original request headers for reference.
    Poco::Net::NameValueCollection _requestHeaders;

    /// \brief The client's SocketAddress for reference.
    Poco::Net::SocketAddress _clientAddress;

    /// \brief True iff the BaseConnection_ is connected to a client.
    bool _isConnected = false;

    /// \brief The total number of bytes sent to the client.
    std::size_t _totalBytesSent = 0;

    /// \brief A queue of the SSEFrames scheduled for delivery.
    mutable std::queue<FrameType> _frameQueue;

    /// \brief A mutex for threadsafe access to the frame queue, etc.
    mutable std::mutex _mutex;

};



template <typename RouteType, typename FrameType>
BaseConnection_<RouteType, FrameType>::BaseConnection_(RouteType& _route):
    BaseRouteHandler_<RouteType>(_route)
{
//    BaseRouteHandler_<RouteType>::route().registerConnection(static_cast<this);
}


template <typename RouteType, typename FrameType>
BaseConnection_<RouteType, FrameType>::~BaseConnection_()
{
    // std::cout << "closing the connection " << std::endl;

//    BaseRouteHandler_<RouteType>::route().unregisterConnection(this);

    if (_isConnected)
        stop();
}


template <typename RouteType, typename FrameType>
void BaseConnection_<RouteType, FrameType>::stop()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _isConnected = false;
    //    _condition.notify_all();
}


template <typename RouteType, typename FrameType>
bool BaseConnection_<RouteType, FrameType>::send(const FrameType& frame) const
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (_isConnected)
    {
        _frameQueue.push(frame);
//        _condition.notify_all();
        return true;
    }
    else
    {
        ofLogError("BaseConnection_::send") << "Not connected, frame not sent.";
        return false;
    }
}


template <typename RouteType, typename FrameType>
std::size_t BaseConnection_<RouteType, FrameType>::sendQueueSize() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _frameQueue.size();
}


template <typename RouteType, typename FrameType>
void BaseConnection_<RouteType, FrameType>::clearSendQueue()
{
    std::unique_lock<std::mutex> lock(_mutex);
    std::queue<FrameType> empty; // a way to clear queues.
    std::swap(_frameQueue, empty);
}


template <typename RouteType, typename FrameType>
Poco::Net::NameValueCollection BaseConnection_<RouteType, FrameType>::requestHeaders() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _requestHeaders;
}


template <typename RouteType, typename FrameType>
Poco::Net::SocketAddress BaseConnection_<RouteType, FrameType>::clientAddress() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _clientAddress;
}


template <typename RouteType, typename FrameType>
bool BaseConnection_<RouteType, FrameType>::isConnected() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _isConnected;
}


template <typename RouteType, typename FrameType>
std::size_t BaseConnection_<RouteType, FrameType>::totalBytesSent() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _totalBytesSent;
}


} } // namespace ofx::HTTP
