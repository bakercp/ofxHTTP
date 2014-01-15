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
#include "ofx/HTTP/Types/AbstractTypes.h"


namespace ofx {
namespace HTTP {


class BaseRouteSettings
    /// \brief A base implementation of route settings.
    /// \details These settings define the basic information needed
    ///         to implement BaseRoute::canHandleRequest() method.
{
public:
    BaseRouteSettings(const std::string& routePathPattern = BaseRouteSettings::DEFAULT_ROUTE_PATH_PATTERN);
        ///< \brief Create the BaseRouteSettings with the given route path.
        ///< \param routePathPattern The regex pattern that this route
        ///<        will handle.

    virtual ~BaseRouteSettings();
        ///< \brief Destroy the BaseRoutSettings.

    void setRoutePathPattern(const std::string& routePathPattern);
        ///< \brief Set the route path regex pattern.
        ///< \param routePathPattern The regex pattern that this route
        ///<        will handle.

    std::string getRoutePathPattern() const;
        ///< \returns The regex pattern that this route handles.

    void setRequireSecurePort(bool requireSecurePort);
        ///< \brief Set the secure port requirement.
        ///< \param requireSecurePort Set to true if this route
        ///<        can only handle requests submitted on an SSL
        ///<        encrypted port.

    bool getRequireSecurePort() const;
        ///< \returns true iff this route requires communication on
        ///<        an SSL encrypted port.

    static const std::string DEFAULT_ROUTE_PATH_PATTERN;
        ///< \brief The default route path regex pattern.
        ///< \details By default, this pattern matches all requests.

private:
    std::string _routePathPattern;
        ///< \brief the route's regex route pattern.

    bool _requireSecurePort;
        ///< \brief true if this route can only handle requests submitted on
        ///<        an SSL encrypted port.

};


} } // namespace ofx::HTTP
