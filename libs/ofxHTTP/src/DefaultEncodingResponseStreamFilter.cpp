//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/DefaultEncodingResponseStreamFilter.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Response.h"
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
                                                        Request& request) const
{
    // Set the headers indicating the encodings we can decode.
    request.set(ACCEPT_ENCODING_HEADER, "gzip, deflate");
}


void DefaultEncodingResponseStreamFilter::responseFilter(Context&,
                                                         Request&,
                                                         Response&) const
{
}


void DefaultEncodingResponseStreamFilter::responseStreamFilter(Context& context,
                                                               const Request& request,
                                                               const Response& response,
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
