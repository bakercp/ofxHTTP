// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
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


class DefaultEncodingResponseStreamFilter: public AbstractResponseStreamFilter
{
public:
    DefaultEncodingResponseStreamFilter();
    virtual ~DefaultEncodingResponseStreamFilter();
    
    void requestFilter(Context& context, BaseRequest& request) const override;

    void responseFilter(Context& context,
                        BaseRequest& request,
                        BaseResponse& response) const override;

    void responseStreamFilter(Context& context,
                              const BaseRequest& request,
                              const BaseResponse& response,
                              IO::FilteredInputStream& responseStream) const override;

    /// \brief The "Accept-Encoding" header key.
    static const std::string ACCEPT_ENCODING_HEADER;

    /// \brief The "Content-Encoding" header key.
    static const std::string CONTENT_ENCODING_HEADER;
    
};


} } // namespace ofx::HTTP
