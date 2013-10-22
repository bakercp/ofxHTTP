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


#include "Poco/Net/WebSocket.h"
#include "ofFileUtils.h"


using Poco::Net::WebSocket;

// TODO:
// - replace ofBuffer with Poco::FIFOBuffer or ofx::IO::ByteBuffer

namespace ofx {
namespace HTTP {


class WebSocketFrame: public ofBuffer
{
public:
    WebSocketFrame(const ofBuffer& buffer,
                   int flags = WebSocket::FRAME_TEXT);

    WebSocketFrame(const std::string& text,
                   int flags = WebSocket::FRAME_TEXT);

    WebSocketFrame(const char* buffer,
                   std::size_t size,
                   int flags = WebSocket::FRAME_TEXT);

    WebSocketFrame(const unsigned char* buffer,
                   std::size_t size,
                   int flags = WebSocket::FRAME_TEXT);
    
    virtual ~WebSocketFrame();
    
    int getFlags() const;
    
    bool isContinuation() const;
    bool isText() const;
    bool isBinary() const;
    bool isClose() const;
    bool isPing() const;
    bool isPong() const;
    
    bool isFinal() const;
    bool isRSV1() const;
    bool isRSV2() const;
    bool isRSV3() const;
    
    std::string toString() const;

protected:
    int _flags;
    
};

    
} } // namespace ofx::HTTP
