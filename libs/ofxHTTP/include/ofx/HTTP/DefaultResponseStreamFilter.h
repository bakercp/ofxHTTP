// =============================================================================
//
// Copyright (c) 2014-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


class DefaultResponseStreamFilter: public AbstractResponseStreamFilter
{
public:
    DefaultResponseStreamFilter();
    virtual ~DefaultResponseStreamFilter();
    
    void requestFilter(BaseRequest& request, Context& context) override;

    void responseFilter(BaseRequest& request,
                        BaseResponse& response,
                        Context& context) override;

    bool canFilterResponse(BaseRequest& request,
                           BaseResponse& response,
                           Context& context) const;

    std::istream& responseStreamFilter(std::istream& responseStream,
                                       const BaseRequest& request,
                                       const BaseResponse& response,
                                       Context& context) override;

    static const std::string ACCEPT_ENCODING_HEADER;
    static const std::string CONTENT_ENCODING_HEADER;

private:
    std::shared_ptr<std::istream> _pResponseStream;
    
};


} } // namespace ofx::HTTP
