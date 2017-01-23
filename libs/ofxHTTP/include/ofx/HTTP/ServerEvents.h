//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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


/// \brief An event describing a server request.
class ServerEventArgs: public ofEventArgs
{
public:
    /// \brief Construct the ServerEventArgs.
    /// \param request the Poco::Net::HTTPServerRequest.
    /// \param response the Poco::Net::HTTPServerResponse.
    /// \param session The AbstractSession associated with this event.
    ServerEventArgs(Poco::Net::HTTPServerRequest& request,
                    Poco::Net::HTTPServerResponse& response,
                    AbstractSession& session):
        _request(request),
        _response(response),
        _session(session)
    {
    }

    /// \brief Destroy the ServerEventArgs.
    virtual ~ServerEventArgs()
    {
    }

    /// \brief Get the Poco::Net::HTTPServerRequest.
    /// \return the Poco::Net::HTTPServerRequest.
    Poco::Net::HTTPServerRequest& request()
    {
        return _request;
    }

    /// \brief Get the Poco::Net::HTTPServerResponse.
    /// \return the Poco::Net::HTTPServerResponse.
    Poco::Net::HTTPServerResponse& response()
    {
        return _response;
    }

    /// \brief Get the session associated with this event.
    /// \returns the session associated with this event.
    AbstractSession& session()
    {
        return _session;
    }

protected:
    /// \brief A reference to the server request.
    Poco::Net::HTTPServerRequest& _request;

    /// \brief Callbacks are permitted to set the response.
    ///
    /// \warning Before working with the response, the callback must check the
    /// HTTPServerResponse::sent() method to ensure that the response stream
    /// is still available available.
    Poco::Net::HTTPServerResponse& _response;

    /// \brief The session associated with the event.
    AbstractSession& _session;

};


/// \brief A class describing a set of low level HTTP server events.
class ServerEvents
{
public:
    /// \brief The onHTTPServerEvent event.
    ofEvent<ServerEventArgs> onHTTPServerEvent;

};


} } // namespace ofx::HTTP
