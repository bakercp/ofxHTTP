// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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

    virtual std::string getRoutePathPattern() const override;

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const override;

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override;

    virtual void handleRequest(ServerEventArgs& evt) override;

    virtual void stop() override;

    /// \returns the route's Settings.
    const SettingsType& getSettings() const;

    /// \returns a pointer to the server that owns this route.
    AbstractServer* getServer() override;

    /// \brief Set the server that owns this route.
    /// \param server A pointer to the server that owns this route.
    void setServer(AbstractServer* server) override;

protected:
    /// \brief The settings.
    SettingsType _settings;

    /// \brief A pointer to the server.
    AbstractServer* _server;

private:
    BaseRoute_(const BaseRoute_&);
	BaseRoute_& operator = (const BaseRoute_&);

};


typedef BaseRoute_<BaseRouteSettings> DefaultRoute;


template <typename SettingsType>
BaseRoute_<SettingsType>::BaseRoute_(const SettingsType& settings):
    _settings(settings),
    _server(0)
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
std::string BaseRoute_<SettingsType>::getRoutePathPattern() const
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
        return Poco::RegularExpression(getRoutePathPattern()).match(path);
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
    // We assert that the server must be set on a route.
    poco_assert(_server);

    // All requests pass through the server's handleRequest method first.
    // The server broadcasts the request / response to any listeners.
    // Generally these listeners should only modify headers, not send a response.
    AbstractSession& session = _server->getSessionStore().getSession(request, response);

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
    if (evt.getResponse().sent())
    {
        return;
    }

    // We gave the handlers every opportunity to send a response.
    // Now we must conclude that there is a server error.
    try
    {
        // If we got this far and our status is still marked as 200,
        // that constitutes a server error.
        if (Poco::Net::HTTPResponse::HTTP_OK == evt.getResponse().getStatus())
        {
            evt.getResponse().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR,
                                                 "No handlers for route.");
        }

        evt.getResponse().setChunkedTransferEncoding(true);
        evt.getResponse().setContentType("text/html");

        std::stringstream ss;
        ss << evt.getResponse().getStatus() << " - " << evt.getResponse().getReason();

        std::ostream& ostr = evt.getResponse().send(); // get output stream
        ostr << "<!DOCTYPE html><html><head><meta charset=\"utf-8\"/><title>";
        ostr << ss.str();
        ostr << "</title></head><body><h1>";
        ostr << ss.str();
        ostr << "</h1></body></html>";
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
const SettingsType& BaseRoute_<SettingsType>::getSettings() const
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
    RouteType& getRoute();

private:
    BaseRouteHandler_(const BaseRouteHandler_&);
    BaseRouteHandler_& operator = (const BaseRouteHandler_&);

    /// \brief The route.
    RouteType& _route;

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
    poco_assert(getRoute().getServer());

    // All requests pass through the server's handleRequest method first.
    // The server broadcasts the request / response to any listeners.
    // Generally these listeners should only modify headers, not send a response.
    AbstractSession& session = getRoute().getServer()->getSessionStore().getSession(request, response);

    ServerEventArgs evt(request, response, session);

    getRoute().getServer()->onHTTPServerEvent(this, evt);

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
        getRoute().handleRequest(evt);
    }
}


template <typename RouteType>
void BaseRouteHandler_<RouteType>::stop()
{
}


template <typename RouteType>
RouteType& BaseRouteHandler_<RouteType>::getRoute()
{
    return _route;
}


} } // namespace ofx::HTTP
