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

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

protected:
    /// \breif The the handler to adapt.
    Poco::Net::HTTPRequestHandler& _handler;

};


} } // namespace ofx::HTTP
