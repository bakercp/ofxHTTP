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


#include "ofx/HTTP/DefaultEncodingResponseStreamFilter.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"
#include "ofx/IO/StreamFilters.h"


namespace ofx {
namespace HTTP {


const std::string DefaultEncodingResponseStreamFilter::ACCEPT_ENCODING_HEADER = "Accept-Encoding";
const std::string DefaultEncodingResponseStreamFilter::CONTENT_ENCODING_HEADER = "Content-Encoding";


DefaultEncodingResponseStreamFilter::DefaultEncodingResponseStreamFilter()
{
}


DefaultEncodingResponseStreamFilter::~DefaultEncodingResponseStreamFilter()
{
}


void DefaultEncodingResponseStreamFilter::requestFilter(Context&,
                                                        BaseRequest& request) const
{
    // Set the headers indicating the encodings we can decode.
    request.set(ACCEPT_ENCODING_HEADER, "gzip, deflate");
}


void DefaultEncodingResponseStreamFilter::responseFilter(Context&,
                                                         BaseRequest&,
                                                         BaseResponse&) const
{
}


void DefaultEncodingResponseStreamFilter::responseStreamFilter(Context& context,
                                                               const BaseRequest& request,
                                                               const BaseResponse& response,
                                                               IO::FilteredInputStream& responseStream) const
{
    if (response.has(CONTENT_ENCODING_HEADER))
    {
        std::string contentEncoding = response.get(CONTENT_ENCODING_HEADER);

        if (contentEncoding == "gzip")
        {
            responseStream.push(std::make_unique<IO::InflatingInputStreamFilter>(Poco::InflatingStreamBuf::StreamType::STREAM_GZIP));
        }
        else if (contentEncoding == "deflate")
        {
            responseStream.push(std::make_unique<IO::InflatingInputStreamFilter>(Poco::InflatingStreamBuf::StreamType::STREAM_ZLIB));
        }
    }
}


} } // namespace ofx::HTTP
