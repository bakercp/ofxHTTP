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


namespace ofx {
namespace HTTP {


class AbstractRequestProcessor
{
public:
    typedef std::shared_ptr<AbstractRequestProcessor> SharedPtr;

    virtual ~AbstractRequestProcessor()
    {
    }

    virtual void processRequest(Poco::Net::HTTPRequest& request,
                                Context& context) = 0;

};


class AbstractResponseProcessor
{
public:
    typedef std::shared_ptr<AbstractResponseProcessor> SharedPtr;

    
    virtual ~AbstractResponseProcessor()
    {
    }

    
    virtual void processResponse(Poco::Net::HTTPRequest& request,
                                 Poco::Net::HTTPResponse& response,
                                 Context& context) = 0;

};


class AbstractRequestResponseProcessor:
    public AbstractRequestProcessor,
    public AbstractResponseProcessor
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


class AbstractRedirectProcessor: public AbstractResponseProcessor
{
public:
    virtual ~AbstractRedirectProcessor()
    {
    }

};


class AbstractAuthenticationProcessor: public AbstractRequestResponseProcessor
{
public:
    virtual ~AbstractAuthenticationProcessor()
    {
    }
};


class AbstractProxyProcessor: public AbstractRequestResponseProcessor
{
public:
    virtual ~AbstractProxyProcessor()
    {
    }

};


class AbstractCookieProcessor: public AbstractRequestResponseProcessor

{
public:
    virtual ~AbstractCookieProcessor()
    {
    }

};


} } // namespace ofx::HTTP
