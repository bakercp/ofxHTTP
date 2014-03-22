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


#include "Poco/Net/MediaType.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "ofEvents.h"


namespace ofx {
namespace HTTP {


class BaseHTTPServerRequestEventArgs: public ofEventArgs
{
public:
    BaseHTTPServerRequestEventArgs(const Poco::Net::HTTPServerRequest& request):
        _request(request)
    {
    }

    virtual ~BaseHTTPServerRequestEventArgs()
    {
    }

    const Poco::Net::HTTPServerRequest& getRequest() const
    {
        return _request;
    }

    std::streamsize getRequestContentLength() const
    {
        return _request.getContentLength();
    }

protected:
    const Poco::Net::HTTPServerRequest& _request;

};



class BaseHTTPServerRequestResponseEventArgs: public BaseHTTPServerRequestEventArgs
{
public:
    BaseHTTPServerRequestResponseEventArgs(const Poco::Net::HTTPServerRequest& request,
                                           Poco::Net::HTTPServerResponse& response):
        BaseHTTPServerRequestEventArgs(request),
        response(response)
    {
    }

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
