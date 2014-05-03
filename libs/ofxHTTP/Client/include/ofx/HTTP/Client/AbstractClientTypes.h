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
#include "ofx/HTTP/Client/Cookie.h"
//#include "ofx/HTTP/Client/BaseRequest.h"
//#include "ofx/HTTP/Client/BaseResponse.h"
//#include "ofx/HTTP/Client/Context.h"


namespace ofx {
namespace HTTP {
namespace Client {


class BaseRequest;
class BaseResponse;
class Context;


class AbstractRequestFilter
{
public:
    virtual ~AbstractRequestFilter()
    {
    }

    virtual void filter(BaseRequest& request,
                        Context& context) = 0;

};


class AbstractResponseFilter
{
public:
    virtual ~AbstractResponseFilter()
    {
    }
    
    virtual void filter(BaseRequest& request,
                        BaseResponse& response,
                        Context& context) = 0;

};


class AbstractRequestResponseFilter:
    public AbstractRequestFilter,
    public AbstractResponseFilter
{
public:
    virtual ~AbstractRequestResponseFilter()
    {
    }

};


class AbstractRequestStreamFilter: public AbstractRequestFilter
{
public:
    virtual ~AbstractRequestStreamFilter()
    {
    }

    virtual std::ostream& filter(std::ostream& requestStream,
                                 const BaseRequest& request,
                                 Context& context) = 0;
    
};


class AbstractResponseStreamFilter: public AbstractRequestResponseFilter
{
public:
    virtual ~AbstractResponseStreamFilter()
    {
    }

    virtual std::istream& filter(std::istream& responseStream,
                                 const BaseRequest& request,
                                 const BaseResponse& response,
                                 Context& context) = 0;

};


} } } // namespace ofx::HTTP::Client
