// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/WebSocketFrame.h"


namespace ofx {
namespace HTTP {


WebSocketFrame::WebSocketFrame(int flags):
    _flags(flags)
{
}


WebSocketFrame::WebSocketFrame(const IO::ByteBuffer& buffer, int flags):
    IO::ByteBuffer(buffer),
    _flags(flags)
{
}


WebSocketFrame::WebSocketFrame(const ofBuffer& buffer, int flags):
    IO::ByteBuffer(buffer),
    _flags(flags)
{
}


WebSocketFrame::WebSocketFrame(const std::string& text, int flags):
    IO::ByteBuffer(text),
    _flags(flags)
{
}


WebSocketFrame::WebSocketFrame(const unsigned char* buffer,
                               std::size_t size,
                               int flags):
    IO::ByteBuffer(buffer, size),
    _flags(flags)
{
}


WebSocketFrame::WebSocketFrame(const char* buffer,
                               std::size_t size,
                               int flags):
    IO::ByteBuffer(buffer, size),
    _flags(flags)
{
}


WebSocketFrame::~WebSocketFrame()
{
}


int WebSocketFrame::getFlags() const
{
    return _flags;
}


bool WebSocketFrame::isContinuation() const
{
    return (_flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_CONT;
}


bool WebSocketFrame::isText() const
{
    return (_flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_TEXT;
}


bool WebSocketFrame::isBinary() const
{
    return (_flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_BINARY;
}


bool WebSocketFrame::isClose() const
{
    return (_flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_CLOSE;
}


bool WebSocketFrame::isPing() const
{
    return (_flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_PING;
}


bool WebSocketFrame::isPong() const
{
    return (_flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_PONG;
}


bool WebSocketFrame::isFinal() const
{
    return _flags & Poco::Net::WebSocket::FRAME_FLAG_FIN;
}


void WebSocketFrame::setFinal(bool value)
{
    if (value)
    {
        _flags |= Poco::Net::WebSocket::FRAME_FLAG_FIN;
    }
    else
    {
        _flags &= ~(Poco::Net::WebSocket::FRAME_FLAG_FIN);
    }
}


bool WebSocketFrame::isRSV1() const
{
    return _flags & Poco::Net::WebSocket::FRAME_FLAG_RSV1;
}


void WebSocketFrame::setRSV1(bool value)
{
    if (value)
    {
        _flags |= Poco::Net::WebSocket::FRAME_FLAG_RSV1;
    }
    else
    {
        _flags &= ~(Poco::Net::WebSocket::FRAME_FLAG_RSV1);
    }
}


bool WebSocketFrame::isRSV2() const
{
    return _flags & Poco::Net::WebSocket::FRAME_FLAG_RSV2;
}


void WebSocketFrame::setRSV2(bool value)
{
    if (value)
    {
        _flags |= Poco::Net::WebSocket::FRAME_FLAG_RSV2;
    }
    else
    {
        _flags &= ~(Poco::Net::WebSocket::FRAME_FLAG_RSV2);
    }
}


bool WebSocketFrame::isRSV3() const
{
    return _flags & Poco::Net::WebSocket::FRAME_FLAG_RSV3;
}


void WebSocketFrame::setRSV3(bool value)
{
    if (value)
    {
        _flags |= Poco::Net::WebSocket::FRAME_FLAG_RSV3;
    }
    else
    {
        _flags &= ~(Poco::Net::WebSocket::FRAME_FLAG_RSV3);
    }
}

    
std::string WebSocketFrame::toString() const
{
    std::stringstream ss;
    ss << "Flags:";
    ss << " CONT="    << (isContinuation() ? "Y" : "N");
    ss << ", TXT="    << (isText()         ? "Y" : "N");
    ss << ", BIN="    << (isBinary()       ? "Y" : "N");
    ss << ", CLOSE="  << (isClose()        ? "Y" : "N");
    ss << ", PING="   << (isPing()         ? "Y" : "N");
    ss << ", PONG="   << (isPong()         ? "Y" : "N");
    ss << ", FINAL="  << (isFinal()        ? "Y" : "N");
    ss << ", RSV1="   << (isRSV1()         ? "Y" : "N");
    ss << ", RSV2="   << (isRSV2()         ? "Y" : "N");
    ss << ", RSV3="   << (isRSV3()         ? "Y" : "N");
    ss << ", nBytes=" << size();
    ss << ", bytes="  << getText();
    
    return ss.str();
}

    
} } // namespace ofx::HTTP
