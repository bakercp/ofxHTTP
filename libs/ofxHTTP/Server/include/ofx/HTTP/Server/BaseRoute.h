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
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/RegularExpression.h"
#include "Poco/URI.h"
#include "ofLog.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/Server/BaseRouteHandler.h"
#include "ofx/HTTP/Server/BaseRouteSettings.h"
#include "ofx/HTTP/Server/SessionCache.h"
#include "ofx/HTTP/Server/Session.h"


namespace ofx {
namespace HTTP {


/// \brief The base implmentation of a server route.
template<typename SettingsType>
class BaseRoute_: public AbstractRoute
{
public:
    /// \brief Create a BaseRoute.
    BaseRoute_(const SettingsType& settings = SettingsType());

    /// \brief Destroy a BaseRoute.
    virtual ~BaseRoute_();

    virtual std::string getRoutePathPattern() const;

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const;

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response);

    virtual void stop();

    /// \returns the route's Settings.
    const SettingsType& getSettings() const;

    /// \brief Access the SessionCache.
    ///
    /// The session cache is optional and may be `nullptr`.
    /// Any non-null SessionCache may be treated as thread-safe.
    ///
    /// \returns a shared pointer to the SessionCache.
    SessionCache::SharedPtr getSessionCache();

    void setSessionCache(SessionCache::SharedPtr sessionCache);

    Session::SharedPtr getSession(Poco::Net::HTTPServerRequest& request,
                                  Poco::Net::HTTPServerResponse& response);

    Poco::UUID getSessionId(Poco::Net::HTTPServerRequest& request,
                            Poco::Net::HTTPServerResponse& response);


protected:
    const SettingsType _settings;
        ///< \brief The settings.

    SessionCache::SharedPtr _sessionCache;
        ///< \brief a pointer to the session cache;

private:
    BaseRoute_(const BaseRoute_&);
	BaseRoute_& operator = (const BaseRoute_&);

};


/// \brief A standard base route implementation.
typedef BaseRoute_<BaseRouteSettings> BaseRoute;


template<typename SettingsType>
BaseRoute_<SettingsType>::BaseRoute_(const SettingsType& settings):
    _settings(settings)
{
}


template<typename SettingsType>
BaseRoute_<SettingsType>::~BaseRoute_()
{
}


template<typename SettingsType>
std::string BaseRoute_<SettingsType>::getRoutePathPattern() const
{
    return _settings.getRoutePathPattern();
}


template<typename SettingsType>
bool BaseRoute_<SettingsType>::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                                bool isSecurePort) const
{
    // If this isn't a secure pot and we require that, reject it.
    if (_settings.requireSecurePort() && !isSecurePort)
    {
        return false;
    }

    // Check the request method.
    const BaseRouteSettings::HTTPMethodSet& validHTTPMethods = _settings.getValidHTTPMethods();

    // If validHTTPMethods are defined, then the request must match.
    if (!validHTTPMethods.empty() &&
       validHTTPMethods.find(request.getMethod()) == validHTTPMethods.end())
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
            if ((*iter).matchesRange(contentType))
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
        ofLogError("BaseRoute::canHandleRequest") << exc.what();
        return false;
    }

    if(path.empty())
    {
        path = "/";
    }

    try
    {
        // \todo cache this regex
        return Poco::RegularExpression(getRoutePathPattern()).match(path);
    }
    catch (const Poco::RegularExpressionException& exc)
    {
        ofLogError("BaseRoute::canHandleRequest") << exc.what();
        return false;
    }

}


template<typename SettingsType>
Poco::Net::HTTPRequestHandler* BaseRoute_<SettingsType>::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new BaseRouteHandler(*this);
}


template<typename SettingsType>
void BaseRoute_<SettingsType>::handleRequest(Poco::Net::HTTPServerRequest& request,
                                             Poco::Net::HTTPServerResponse& response)
{
    if (response.sent())
    {
        return;
    }

    // We gave the handlers every opportunity to send a response.
    // Now we must conclude that there is a server error.
    try
    {
        // if we got this far and our status is still marked as 200, that constitutes a server error.
        if(response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR,
                                        "No handlers for route.");
        }

        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");

        std::ostream& ostr = response.send(); // get output stream
        ostr << "<html>";
        ostr << "<head><title>" << response.getStatus() << " - " << response.getReason() << "</title></head>";
        ostr << "<body>";
        ostr << "<h1>" << response.getStatus() << " - " << response.getReason() << "</h1>";
        ostr << "</body>";
        ostr << "<html>";
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


template<typename SettingsType>
void BaseRoute_<SettingsType>::stop()
{
    // empty
}


template<typename SettingsType>
const SettingsType& BaseRoute_<SettingsType>::getSettings() const
{
    return _settings;
}


template<typename SettingsType>
SessionCache::SharedPtr BaseRoute_<SettingsType>::getSessionCache()
{
    return _sessionCache;
}


template<typename SettingsType>
void BaseRoute_<SettingsType>::setSessionCache(SessionCache::SharedPtr sessionCache)
{
    _sessionCache = sessionCache;
}


template<typename SettingsType>
Session::SharedPtr BaseRoute_<SettingsType>::getSession(Poco::Net::HTTPServerRequest& request,
                                                        Poco::Net::HTTPServerResponse& response)
{
    if (_sessionCache)
    {
        return _sessionCache->getSession(request, response);
    }
    else
    {
        Session::SharedPtr ptr;
        return ptr;
    }
}


template<typename SettingsType>
Poco::UUID BaseRoute_<SettingsType>::getSessionId(Poco::Net::HTTPServerRequest& request,
                                                  Poco::Net::HTTPServerResponse& response)
{
    if (_sessionCache)
    {
        Session::SharedPtr session = _sessionCache->getSession(request, response);

        if (session)
        {
            return session->getId();
        }
        else
        {
            return Poco::UUID::null();
        }
    }
    else
    {
        return Poco::UUID::null();
    }
}


    Poco::UUID getSessionId(Poco::Net::HTTPServerRequest& request,
                            Poco::Net::HTTPServerResponse& response);



} } // namespace ofx::HTTP
