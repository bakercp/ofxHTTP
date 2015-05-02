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


#include "Poco/UUID.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "ofEvents.h"
#include "ofx/HTTP/AbstractServerTypes.h"


namespace ofx {
namespace HTTP {


/// \brief A base class describing 
class BaseHTTPSessionEvent: public AbstractHasSessionId, public ofEventArgs
{
public:
    /// \brief Create a BaseHTTPSessionEvent with a sessionId.
    /// \param sessionId the session id associated with this event.
    BaseHTTPSessionEvent(const Poco::UUID& sessionId):
        _sessionId(sessionId)
    {
    }


    /// \brief Destroy the BaseHTTPSessionEvent.
    virtual ~BaseHTTPSessionEvent()
    {
    }


    /// \brief Get the session id associated with this event.
    ///
    /// The session id is established by the SessionCache.  If no SessionCache
    /// is used, this will always return Poco::UUID::null.
    ///
    /// \returns the session id or Poco::UUID::null if not set.
    Poco::UUID getSessionId() const
    {
        return _sessionId;
    }

protected:
    /// \brief The session id, if available.  Poco::UUID::null if null.
    Poco::UUID _sessionId;

};



/// \brief An event describing a server request.
class BaseHTTPServerRequestEventArgs: public BaseHTTPSessionEvent
{
public:
    /// \brief Construct the BaseHTTPServerRequestEventArgs.
    /// \param sessionId the session id associated with this event.
    /// \param request the Poco::Net::HTTPServerRequest.
    BaseHTTPServerRequestEventArgs(const Poco::UUID& sessionId,
                                   const Poco::Net::HTTPServerRequest& request):
        BaseHTTPSessionEvent(sessionId),
        _request(request)
    {
    }

    /// \brief Destroy the BaseHTTPServerRequestEventArgs.
    virtual ~BaseHTTPServerRequestEventArgs()
    {
    }


    /// \brief Return the Poco::Net::HTTPServerRequest.
    /// \return the Poco::Net::HTTPServerRequest.
    const Poco::Net::HTTPServerRequest& getRequest() const
    {
        return _request;
    }

    /// \brief Get the request's content length.
    /// \returns the Request's content lenght in bytes.
    std::streamsize getRequestContentLength() const
    {
        return _request.getContentLength();
    }


protected:
    /// \brief A const reference to the server request.
    const Poco::Net::HTTPServerRequest& _request;

};


/// \brief An event describing a server response.
class BaseHTTPServerRequestResponseEventArgs: public BaseHTTPServerRequestEventArgs
{
public:
    /// \brief Construct the BaseHTTPServerRequestResponseEventArgs.
    /// \param sessionId the session id associated with this event.
    /// \param request the Poco::Net::HTTPServerRequest.
    /// \param _response the Poco::Net::HTTPServerResponse to be modified.
    BaseHTTPServerRequestResponseEventArgs(const Poco::UUID& sessionId,
                                           const Poco::Net::HTTPServerRequest& request,
                                           Poco::Net::HTTPServerResponse& _response):
        BaseHTTPServerRequestEventArgs(sessionId, request),
        response(_response)
    {
    }

    /// \brief Destroy the BaseHTTPServerRequestResponseEventArgs.
    virtual ~BaseHTTPServerRequestResponseEventArgs()
    {
    }


    /// \brief Callbacks are permitted to set the response.
    ///
    /// \warning Before working with the response, the callback must check the
    /// HTTPServerResponse::sent() method to ensure that the response stream
    /// is still available available.
    Poco::Net::HTTPServerResponse& response;

};


} } // namespace ofx::HTTP
