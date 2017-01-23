//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/BaseResponse.h"


namespace ofx {
namespace HTTP {


const std::string BaseResponse::CONTENT_RANGE = "Content-Range";
const std::string BaseResponse::BYTES_UNIT = "bytes";


BaseResponse::~BaseResponse()
{
}


bool BaseResponse::hasResponseStream() const
{
    return _responseStream != nullptr;
}


std::istream& BaseResponse::stream()
{
    return _responseStream->stream();
}


int64_t BaseResponse::estimatedContentLength() const
{
    int64_t contentLength = getContentLength64();

    // If needed, try to calculate it based on the form data.
    if (contentLength == UNKNOWN_CONTENT_LENGTH)
    {
        // We only support a very specific content range in the form:
        // Content-Range: bytes 0-4068/4069
        // This could be expanded using regex, e.g.
        // https://code.google.com/p/googleappengine/source/browse/trunk/java/src/main/com/google/appengine/api/blobstore/ByteRange.java
        auto tokens = ofSplitString(get(CONTENT_RANGE, ""), " ", true, true);

        if (tokens.size() == 2 && tokens[0] == BYTES_UNIT)
        {
            auto _entryTokens = ofSplitString(tokens[1], "/", true, true);

            if (_entryTokens.size() == 2)
            {
                auto _rangeTokens = ofSplitString(tokens[1], "-", true, true);

                if (_rangeTokens.size() == 2)
                {
                    int rangeStart = ofToInt(_rangeTokens[0]);
                    int rangeEnd = ofToInt(_rangeTokens[1]);

                    if (rangeEnd > rangeStart)
                    {
                        contentLength = rangeEnd - rangeStart + 1;
                    }
                }
            }
        }
    }
    
    return contentLength;
}


BufferedResponse::~BufferedResponse()
{

}


std::istream& BufferedResponse::stream()
{
    _bufferResponse();
    return *_bbis;
}


ofBuffer BufferedResponse::buffer() 
{
    _bufferResponse();
    return ofBuffer(_buffer.getCharPtr(), _buffer.size());
}


bool BufferedResponse::isBuffered() const
{
    return _bbis != nullptr;
}


void BufferedResponse::_bufferResponse()
{
    if (_bbis == nullptr)
    {
        _buffer.writeBytes(BaseResponse::stream());
        _bbis = std::make_unique<IO::ByteBufferInputStream>(_buffer);
    }
}


} } // namespace ofx::HTTP
