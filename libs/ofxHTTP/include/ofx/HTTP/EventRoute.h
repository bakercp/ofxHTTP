//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/BaseRoute.h"


namespace ofx {
namespace HTTP {


class EventRouteSettings: public BaseRouteSettings
{
public:
    EventRouteSettings(const std::string& routePathPattern = DEFAULT_ROUTE_PATH_PATTERN);

    virtual ~EventRouteSettings();

    /// \brief An unfortunate compromise until C++11.
    /// \note C++ is not able to initialize static collections until
    ///        after C++11.  This is a compromise until then.
    static const std::string DEFAULT_GET_HTTP_METHODS_ARRAY[];

    /// \brief The default HTTP methods for this route.
    static const HTTPMethodSet DEFAULT_GET_HTTP_METHODS;

private:
    
};


/// \brief A route dynamic events.
class EventRoute: public BaseRoute_<EventRouteSettings>
{
public:
    typedef EventRouteSettings Settings;

    EventRoute(const Settings& settings);

    virtual ~EventRoute();

    virtual void handleRequest(ServerEventArgs& evt) override;

    virtual void handleErrorResponse(ServerEventArgs& evt);

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

};

    
} } // namespace ofx::HTTP
