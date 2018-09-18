//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/EventRoute.h"
//#include "ofUtils.h"
//#include "ofx/MediaTypeMap.h"


namespace ofx {
namespace HTTP {


const std::string EventRouteSettings::DEFAULT_GET_HTTP_METHODS_ARRAY[] = { "GET" };
const EventRouteSettings::HTTPMethodSet EventRouteSettings::DEFAULT_GET_HTTP_METHODS(INIT_SET_WITH_ARRAY(DEFAULT_GET_HTTP_METHODS_ARRAY));


EventRouteSettings::EventRouteSettings(const std::string& routePathPattern):
    BaseRouteSettings(routePathPattern,
                      false,
                      false,
                      EventRouteSettings::DEFAULT_GET_HTTP_METHODS)
{
}


EventRouteSettings::~EventRouteSettings()
{
}


EventRoute::EventRoute(const Settings& settings):
    BaseRoute_<EventRouteSettings>(settings)
{
}


EventRoute::~EventRoute()
{
}


void EventRoute::handleRequest(ServerEventArgs& evt)
{
    std::cout << "handling request" << std::endl;
}


Poco::Net::HTTPRequestHandler* EventRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new RequestHandlerAdapter(*this);
}


} } // namespace ofx::HTTP
