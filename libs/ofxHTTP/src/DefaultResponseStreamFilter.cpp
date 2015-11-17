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


#include "ofx/HTTP/DefaultResponseStreamFilter.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"
#include "Poco/InflatingStream.h"


namespace ofx {
namespace HTTP {


const std::string DefaultResponseStreamFilter::ACCEPT_ENCODING_HEADER = "Accept-Encoding";
const std::string DefaultResponseStreamFilter::CONTENT_ENCODING_HEADER = "Content-Encoding";


DefaultResponseStreamFilter::DefaultResponseStreamFilter()
{
}


DefaultResponseStreamFilter::~DefaultResponseStreamFilter()
{
}


void DefaultResponseStreamFilter::requestFilter(BaseRequest& request,
                                                Context&)
{
    // Set the headers indicating the encodings we can decode.
    request.set(ACCEPT_ENCODING_HEADER, "gzip, deflate");
}


void DefaultResponseStreamFilter::responseFilter(BaseRequest&,
                                                 BaseResponse&,
                                                 Context&)
{
}


bool DefaultResponseStreamFilter::canFilterResponse(BaseRequest&,
                                                    BaseResponse&,
                                                    Context&) const
{
    return true;
}


std::istream& DefaultResponseStreamFilter::responseStreamFilter(std::istream& responseStream,
                                                                const BaseRequest&,
                                                                const BaseResponse& response,
                                                                Context&)
{
    _pResponseStream.reset();

    if (response.has(CONTENT_ENCODING_HEADER))
    {
        std::string contentEncoding = response.get(CONTENT_ENCODING_HEADER);

        if (0 == Poco::UTF8::icompare(contentEncoding, "gzip"))
        {
            _pResponseStream = std::make_shared<Poco::InflatingInputStream>(responseStream,
                                                                            Poco::InflatingStreamBuf::STREAM_GZIP);
            return *_pResponseStream;
        }
        else if (0 == Poco::UTF8::icompare(contentEncoding, "deflate"))
        {
            _pResponseStream = std::make_shared<Poco::InflatingInputStream>(responseStream,
                                                                            Poco::InflatingStreamBuf::STREAM_ZLIB);
            return *_pResponseStream;
        }
        else
        {
            ofLogWarning("DefaultResponseStreamFilter") << "Returning with unknown content encoding: " << contentEncoding;
            return responseStream;
        }
    }
    else
    {
        return responseStream;
    }
}


} } // namespace ofx::HTTP
