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


#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
#include "ofTypes.h"
#include "ofx/HTTP/Client/Context.h"
#include "ofx/HTTP/Client/Cookie.h"
#include "ofx/HTTP/Client/BaseRequest.h"
#include "ofx/HTTP/Client/BaseResponse.h"


namespace ofx {
namespace HTTP {


class AbstractRequestProcessor
{
public:
    virtual ~AbstractRequestProcessor()
    {
    }

    virtual void processRequest(Client::BaseRequest& request,
                                Context& context) = 0;

};


class AbstractResponseHandler
{
public:
    virtual ~AbstractResponseHandler()
    {
    }
    
    virtual bool handleResponse(Client::BaseRequest& request,
                                Client::BaseResponse& response,
                                Context& context) = 0;

    virtual std::vector<Poco::Net::HTTPResponse::HTTPStatus> getHandledStatuses() const = 0;

    virtual bool isStatusHandled(Poco::Net::HTTPResponse::HTTPStatus status) const = 0;

};


class AbstractRequestResponseProcessor:
    public AbstractRequestProcessor,
    public AbstractResponseHandler
{
public:
    virtual ~AbstractRequestResponseProcessor()
    {
    }

};


class AbstractSessionProvider
{
public:
    virtual ~AbstractSessionProvider()
    {
    }

    virtual void createSession(const Poco::URI& uri,
                               Context& context) = 0;
};


class AbstractRequestStreamFilter: public AbstractRequestProcessor
{
public:
    virtual ~AbstractRequestStreamFilter()
    {
    }

    virtual std::ostream& filter(std::ostream& requestStream) = 0;
    
};


class AbstractResponseStreamFilter: public AbstractRequestResponseProcessor
{
public:
    virtual ~AbstractResponseStreamFilter()
    {
    }

    virtual std::istream& filter(std::istream& responseStream) = 0;

};
    
    


} } // namespace ofx::HTTP
