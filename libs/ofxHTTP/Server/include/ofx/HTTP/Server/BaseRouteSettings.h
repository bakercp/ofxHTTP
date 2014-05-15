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


/// \brief An unfortunate compromise until C++11.
#define INIT_SET_WITH_ARRAY(x) x, x + sizeof(x) / sizeof(x[0])


#include <set>
#include <string>
#include "Poco/Net/MediaType.h"
#include "ofx/HTTP/Types/AbstractTypes.h"


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
    typedef std::set<Poco::Net::MediaType> MediaTypeSet;

    /// \brief Create the BaseRouteSettings with the given route path.
    /// \param routePathPattern The regex pattern that this route will handle.
    /// \param requireSecurePorttrue True if this route requires
    ///        communication on an SSL encrypted port.
    /// \param validHTTPMethods The valid HTTP Methods that this route
    ///        will handle.
    BaseRouteSettings(const std::string& routePathPattern = BaseRouteSettings::DEFAULT_ROUTE_PATH_PATTERN,
                      bool requireSecurePort = false,
                      const HTTPMethodSet& validHTTPMethods = BaseRouteSettings::DEFAULT_HTTP_METHODS);

    /// \brief Destroy the BaseRoutSettings.
    virtual ~BaseRouteSettings();

    /// \brief Set the route path regex pattern.
    /// \param routePathPattern The regex pattern that this route will handle.
    void setRoutePathPattern(const std::string& routePathPattern);

    /// \returns The regex pattern that this route handles.
    const std::string& getRoutePathPattern() const;

    /// \brief Set the secure port requirement.
    /// \param requireSecurePort Set to true if this route
    ///        can only handle requests submitted on an SSL
    ///        encrypted port.
    void setRequireSecurePort(bool requireSecurePort);

    /// \returns true iff route requires communication on an SSL encrypted port.
    bool requireSecurePort() const;

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

    /// \brief An unfortunate compromise until C++11.
    /// \note C++ is not able to initialize static collections until
    ///        after C++11.  This is a compromise until then.
    static const std::string DEFAULT_HTTP_METHODS_ARRAY[];

    /// \brief The default HTTP methods for this route.
    static const HTTPMethodSet DEFAULT_HTTP_METHODS;

private:
    /// \brief the route's regex route pattern.
    std::string _routePathPattern;

    /// \brief true if route requires requests on an SSL encrypted port.
    bool _requireSecurePort;

    /// \brief A set of valid HTTP methods.
    HTTPMethodSet _validHTTPMethods;

    /// \brief A set of valid Content-Type variables.
    MediaTypeSet _validContentTypes;

    /// \brief A session key for this route.
    std::string _routeSessionKey;

};


} } // namespace ofx::HTTP

