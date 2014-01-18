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


#define INIT_SET_WITH_ARRAY(x) x, x + sizeof(x) / sizeof(x[0])
    ///< \brief An unfortunate compromise until C++11.


#include <set>
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
    typedef std::set<std::string> HTTPMethodSet;
        ///< \brief A typedef for HTTPMethodSet.

    BaseRouteSettings(const std::string& routePathPattern = BaseRouteSettings::DEFAULT_ROUTE_PATH_PATTERN,
                      bool requireSecurePort = false,
                      const HTTPMethodSet& validHTTPMethods = BaseRouteSettings::DEFAULT_HTTP_METHODS);
        ///< \brief Create the BaseRouteSettings with the given route path.
        ///< \param routePathPattern The regex pattern that this route
        ///<        will handle.
        ///< \param requireSecurePorttrue True if this route requires
        ///<        communication on an SSL encrypted port.
        ///< \param validHTTPMethods The valid HTTP Methods that this route
        ///<        will handle.

    virtual ~BaseRouteSettings();
        ///< \brief Destroy the BaseRoutSettings.

    void setRoutePathPattern(const std::string& routePathPattern);
        ///< \brief Set the route path regex pattern.
        ///< \param routePathPattern The regex pattern that this route
        ///<        will handle.

    const std::string& getRoutePathPattern() const;
        ///< \returns The regex pattern that this route handles.

    void setRequireSecurePort(bool requireSecurePort);
        ///< \brief Set the secure port requirement.
        ///< \param requireSecurePort Set to true if this route
        ///<        can only handle requests submitted on an SSL
        ///<        encrypted port.

    bool requireSecurePort() const;
        ///< \returns true iff this route requires communication on
        ///<        an SSL encrypted port.

    void setValidHTTPMethods(const HTTPMethodSet& validHTTPMethods);
        ///< \brief Set the list of valid HTTPMethods.
        ///< \param validHTTPMethods A set of valid HTTPMethods.
        ///< \note  An empty set means that any requested HTTPMethod will be
        ///<        accepted.  A non-empty set means that the requested
        ///<        HTTPMethod MUST be in the set.

    const HTTPMethodSet& getValidHTTPMethods() const;
        ///< \returns The set of valid HTTP methods.
        ///< \note  If empty, all requested HTTP methods will be accepted.

    static const std::string DEFAULT_ROUTE_PATH_PATTERN;
        ///< \brief The default route path regex pattern.
        ///< \details By default, this pattern matches all requests.

    static const std::string DEFAULT_HTTP_METHODS_ARRAY[];
        ///< \brief An unfortunate compromise until C++11.
        ///< \note C++ is not able to initialize static collections until
        ///<        after C++11.  This is a compromise until then.

    static const HTTPMethodSet DEFAULT_HTTP_METHODS;
        ///< \brief The default HTTP methods for this route.

private:
    std::string _routePathPattern;
        ///< \brief the route's regex route pattern.

    bool _requireSecurePort;
        ///< \brief true if this route can only handle requests submitted on
        ///<        an SSL encrypted port.

    HTTPMethodSet _validHTTPMethods;
        ///< \brief A set of valid HTTP methods.

};


} } // namespace ofx::HTTP

