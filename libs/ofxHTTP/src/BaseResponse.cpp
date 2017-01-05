// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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
