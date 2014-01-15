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
    /// \brief A WebSocketFrame frame.
{
public:
    WebSocketFrame(const ofBuffer& buffer,
                   int flags = WebSocket::FRAME_TEXT);
        ///< \brief Create a WebSocketFrame.

    WebSocketFrame(const std::string& text,
                   int flags = WebSocket::FRAME_TEXT);
        ///< \brief Create a WebSocketFrame.

    WebSocketFrame(const char* buffer,
                   std::size_t size,
                   int flags = WebSocket::FRAME_TEXT);
        ///< \brief Create a WebSocketFrame.

    WebSocketFrame(const unsigned char* buffer,
                   std::size_t size,
                   int flags = WebSocket::FRAME_TEXT);
        ///< \brief Create a WebSocketFrame.

    virtual ~WebSocketFrame();
        ///< \brief Destroy a WebSocketFrame.

    int getFlags() const;
        ///< \returns the WebSockFrame flags.
    
    bool isContinuation() const;
        ///< \returns true iff a continuation frame.

    bool isText() const;
        ///< \returns true iff a text frame.

    bool isBinary() const;
        ///< \returns true iff a binary frame.

    bool isClose() const;
        ///< \returns true iff a close frame.

    bool isPing() const;
        ///< \returns true iff a ping frame.

    bool isPong() const;
        ///< \returns true iff a pong frame.

    bool isFinal() const;
        ///< \returns true iff a final frame.

    bool isRSV1() const;
        ///< \returns true iff a RSV1 frame.

    bool isRSV2() const;
        ///< \returns true iff a RSV2 frame.

    bool isRSV3() const;
        ///< \returns true iff a RSV3 frame.

    std::string toString() const;
        ///< \returns a string representing the WebSocketFrame.

protected:
    int _flags;
        ///< \brief the websocket flags for this frame.
    
};

    
} } // namespace ofx::HTTP
