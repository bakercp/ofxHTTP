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
