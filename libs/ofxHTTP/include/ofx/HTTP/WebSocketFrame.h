//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Net/WebSocket.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofFileUtils.h"


namespace ofx {
namespace HTTP {


/// \brief A WebSocketFrame frame.
class WebSocketFrame: public IO::ByteBuffer
{
public:
    /// \brief Create a WebSocketFrame.
    WebSocketFrame(int flags = Poco::Net::WebSocket::FRAME_TEXT);

    /// \brief Create a WebSocketFrame.
    WebSocketFrame(const IO::ByteBuffer& buffer,
                   int flags = Poco::Net::WebSocket::FRAME_TEXT);

    /// \brief Create a WebSocketFrame.
    WebSocketFrame(const ofBuffer& buffer,
                   int flags = Poco::Net::WebSocket::FRAME_TEXT);

    /// \brief Create a WebSocketFrame.
    WebSocketFrame(const std::string& text,
                   int flags = Poco::Net::WebSocket::FRAME_TEXT);

    /// \brief Create a WebSocketFrame.
    WebSocketFrame(const char* buffer,
                   std::size_t size,
                   int flags = Poco::Net::WebSocket::FRAME_TEXT);

    /// \brief Create a WebSocketFrame.
    WebSocketFrame(const unsigned char* buffer,
                   std::size_t size,
                   int flags = Poco::Net::WebSocket::FRAME_TEXT);

    /// \brief Destroy a WebSocketFrame.
    virtual ~WebSocketFrame();

    /// \returns the WebSocketFrame flags.
    int flags() const;

    /// \returns true iff a continuation frame.
    bool isContinuation() const;

    /// \returns true iff a text frame.
    bool isText() const;

    /// \returns true iff a binary frame.
    bool isBinary() const;

    /// \returns true iff a close frame.
    bool isClose() const;

    /// \returns true iff a ping frame.
    bool isPing() const;

    /// \returns true iff a pong frame.
    bool isPong() const;

    /// \returns true iff a final frame.
    bool isFinal() const;

    /// \brief Set the FIN bit.
    /// \param value The bit's value.
    void setFinal(bool value);

    /// \returns true iff a RSV1 frame.
    bool isRSV1() const;

    /// \brief Set the RSV1 bit.
    /// \param value The bit's value.
    void setRSV1(bool value);

    /// \returns true iff a RSV2 frame.
    bool isRSV2() const;

    /// \brief Set the RSV2 bit.
    /// \param value The bit's value.
    void setRSV2(bool value);

    /// \returns true iff a RSV3 frame.
    bool isRSV3() const;

    /// \brief Set the RSV3 bit.
    /// \param value The bit's value.
    void setRSV3(bool value);

    /// \returns a string representing the WebSocketFrame.
    std::string toString() const;

protected:
    /// \brief the websocket flags for this frame.
    int _flags;
    
};

    
} } // namespace ofx::HTTP
