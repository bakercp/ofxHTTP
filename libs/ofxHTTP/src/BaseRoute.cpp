//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/BaseRoute.h"


namespace ofx {
namespace HTTP {


const std::string BaseRouteSettings::DEFAULT_ROUTE_PATH_PATTERN = "/.*";


BaseRouteSettings::BaseRouteSettings(const std::string& routePathPattern,
                                     bool requireSecurePort,
                                     bool requireAuthentication,
                                     const HTTPMethodSet& validHTTPMethods):
    _routePathPattern(routePathPattern),
    _requireSecurePort(requireSecurePort),
    _requireAuthentication(requireAuthentication),
    _validHTTPMethods(validHTTPMethods)
{
}


BaseRouteSettings::~BaseRouteSettings()
{
}


void BaseRouteSettings::setRoutePathPattern(const std::string& routePathPattern)
{
    _routePathPattern = routePathPattern;
}


const std::string& BaseRouteSettings::getRoutePathPattern() const
{
    return _routePathPattern;
}


void BaseRouteSettings::setRequireSecurePort(bool requireSecurePort)
{
    _requireSecurePort = requireSecurePort;
}


bool BaseRouteSettings::requireSecurePort() const
{
    return _requireSecurePort;
}


void BaseRouteSettings::setRequireAuthentication(bool requireAuthentication)
{
    _requireAuthentication = requireAuthentication;
}


bool BaseRouteSettings::requireAuthentication() const
{
    return _requireAuthentication;
}


void BaseRouteSettings::setValidHTTPMethods(const BaseRouteSettings::HTTPMethodSet& validHTTPMethods)
{
    _validHTTPMethods = validHTTPMethods;
}


const BaseRouteSettings::HTTPMethodSet& BaseRouteSettings::getValidHTTPMethods() const
{
    return _validHTTPMethods;
}


void BaseRouteSettings::setValidContentTypes(const MediaTypeSet& validContentTypes)
{
    _validContentTypes = validContentTypes;
}


const BaseRouteSettings::MediaTypeSet& BaseRouteSettings::getValidContentTypes() const
{
    return _validContentTypes;
}


} } // namespace ofx::HTTP
