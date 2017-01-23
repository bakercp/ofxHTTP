//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "ofLog.h"


namespace ofx {
namespace HTTP {


/// \brief A wrapper class to allow Poco::Net::HTTPRequestHandler to be reused.
///
// A route handler adapter adapts the factory class (e.g. the BaseRoute_)
// to act as a reusable instance. The instance passed to the RouteHandler
// adapter should not modify the internal state of the route itself.
class RequestHandlerAdapter: public Poco::Net::HTTPRequestHandler
{
public:
    /// \brief Create a RequestHandlerAdapter with the given handler reference.
    /// \param handler The default RequestHandlerAdapter to use.
    RequestHandlerAdapter(Poco::Net::HTTPRequestHandler& handler);

    /// \brief Destroy the RequestHandlerAdapter.
    virtual ~RequestHandlerAdapter();

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response) override;

protected:
    /// \brief The the handler to adapt.
    Poco::Net::HTTPRequestHandler& _handler;

};


} } // namespace ofx::HTTP
