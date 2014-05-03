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


#include "Poco/TeeStream.h"
#include "ofx/HTTP/Client/ClientProgressStream.h"
#include "ofx/HTTP/Client/AbstractClientTypes.h"
#include "ofx/HTTP/Client/ClientEvents.h"
#include "ofx/HTTP/Client/BaseRequest.h"
#include "ofx/HTTP/Client/BaseResponse.h"


namespace ofx {
namespace HTTP {
namespace Client {


class BaseClient:
    public AbstractRequestStreamListener,
    public AbstractResponseStreamListener
{
public:
    BaseClient();

    virtual ~BaseClient();

    std::istream& execute(BaseRequest& request,
                          BaseResponse& response,
                          Context& context);

    void addRequestFilter(AbstractRequestFilter* filter);
    void addResponseFilter(AbstractResponseFilter* filter);

    void removeRequestFilter(AbstractRequestFilter* filter);
    void removeResponseFilter(AbstractResponseFilter* filter);

    void setRequestStreamFilter(AbstractRequestStreamFilter* filter);
    void setResponseStreamFilter(AbstractResponseStreamFilter* filter);

    void removeRequestStreamFilter();
    void removeResponseStreamFilter();

    ClientEvents events;

private:
    typedef std::vector<AbstractRequestFilter*> RequestFilters;
    typedef std::vector<AbstractResponseFilter*> ResponseFilters;

    RequestFilters _requestFilters;
    ResponseFilters _responseFilters;

    ClientProgressRequestStream* _pClientProgressRequestStream;
    ClientProgressResponseStream* _pClientProgressResponseStream;

    AbstractRequestStreamFilter* _requestStreamFilter;
    AbstractResponseStreamFilter* _responseStreamFilter;

    void progress(const BaseRequest& request,
                  Context& context,
                  std::streamsize total);

    void progress(const BaseRequest& request,
                  const BaseResponse& response,
                  Context& context,
                  std::streamsize total);

};


} } } // namespace ofx::HTTP::Client
