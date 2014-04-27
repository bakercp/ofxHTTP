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


#include "Poco/UUID.h"
#include "Poco/Net/WebSocket.h"
#include "ofEvents.h"
#include "ofEventUtils.h"


namespace ofx {
namespace HTTP {


class WebSocketFrame;
class WebSocketRouteHandler;
class WebSocketConnection;


/// \brief A collection of Websocket Errors
enum WebSocketError
{
    WS_ERR_NONE                           = 0,
    WS_ERR_NO_HANDSHAKE                   = Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE,
        ///< \brief No Connection: Upgrade or Upgrade:
        ///<        websocket header in handshake request.
    WS_ERR_HANDSHAKE_NO_VERSION           = Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION,
        ///< \brief No Sec-WebSocket-Version header in handshake request.
    WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION  = Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION,
        ///< \brief Unsupported WebSocket version requested by client.
    WS_ERR_HANDSHAKE_NO_KEY               = Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY,
        ///< \brief No Sec-WebSocket-Key header in handshake request.
    WS_ERR_HANDSHAKE_ACCEPT               = Poco::Net::WebSocket::WS_ERR_HANDSHAKE_ACCEPT,
        ///< \brief No Sec-WebSocket-Accept header or wrong value.
    WS_ERR_UNAUTHORIZED                   = Poco::Net::WebSocket::WS_ERR_UNAUTHORIZED,
        ///< \brief The server rejected the username or password
        ///<        for authentication.
    WS_ERR_PAYLOAD_TOO_BIG                = Poco::Net::WebSocket::WS_ERR_PAYLOAD_TOO_BIG,
        ///< \brief Payload too big for supplied buffer.
    WS_ERR_INCOMPLETE_FRAME               = Poco::Net::WebSocket::WS_ERR_INCOMPLETE_FRAME,
        ///< \brief Incomplete frame received.
    WS_ERROR_INCOMPLETE_FRAME_SENT        = 15,
    WS_ERROR_ZERO_BYTE_FRAME_SENT         = 15,
    WS_ERR_TIMEOUT                        = 20,
    WS_ERR_NET_EXCEPTION                  = 30,
    WS_ERR_OTHER                          = 50,
    
};


/// \brief The base WebSocket event arguments.
class WebSocketEventArgs: public ofEventArgs
{
public:
    /// \brief Create a WebSocketEventArgs object with the provided params.
    /// \param connection A reference to the associated WebSocketConnection.
    /// \param error An error, if any, associated with the event.
    WebSocketEventArgs(const Poco::UUID& sessionId,
                       const WebSocketConnection& connection,
                       WebSocketError error = WS_ERR_NONE):
        _sessionId(sessionId),
        _connection(connection),
        _error(error)
    {
    }
    
    /// \returns true iff _error is not equal to WS_ERR_NONE.
    bool hasError() const
    {
        return _error != WS_ERR_NONE;
    }

    /// \returns the error code.
    WebSocketError getError() const
    {
        return _error;
    }

    /// \brief Get the session id associated with this event.
    ///
    /// The session id is established by the SessionCache.  If no SessionCache
    /// is used, this will always return Poco::UUID::null.
    ///
    /// \returns the session id or Poco::UUID::null if not set.
    const WebSocketConnection& getConnectionRef() const
    {
        return _connection;
    }

private:
    Poco::UUID _sessionId;
        ///< \brief The session id, if available.  Poco::UUID::null if null.

    const WebSocketConnection& _connection;
        ///< \brief A reference to the WebSocketConnection.

    WebSocketError _error;
        ///< \brief The WebSocketError associated with the event, if any.

};

class WebSocketFrameEventArgs: public WebSocketEventArgs
{
public:
    /// \brief Create a WebSocketFrameEventArgs object.
    /// \param connection A reference to the associated WebSocketConnection.
    /// \param error An error, if any, associated with the event.
    WebSocketFrameEventArgs(const WebSocketFrame& frame,
                            const Poco::UUID& sessionId,
                            const WebSocketConnection& connection,
                            WebSocketError error = WS_ERR_NONE):
        WebSocketEventArgs(sessionId, connection, error),
        _frame(frame)
    {
    }

    /// \returns A reference to the WebSocketFrame associated with the event.
    const WebSocketFrame& getFrameRef() const
    {
        return _frame;
    }

private:
    const WebSocketFrame& _frame;
        ///< \brief A reference to the WebSocketFrame associated with the event.

};

class WebSocketEvents
{
public:
    ofEvent<WebSocketEventArgs>      onOpenEvent;
    ofEvent<WebSocketEventArgs>      onCloseEvent;
    ofEvent<WebSocketFrameEventArgs> onFrameReceivedEvent;
    ofEvent<WebSocketFrameEventArgs> onFrameSentEvent;
    ofEvent<WebSocketEventArgs>      onErrorEvent;
    
};

    
} } // namespace ofx::HTTP
