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


#include "ofEvents.h"
#include "ofEventUtils.h"


namespace ofx {
namespace HTTP {


class WebSocketFrame;
class WebSocketRouteHandler;
class WebSocketConnection;


enum WebSocketError
{
    WS_ERR_NONE                           = 0,
    WS_ERR_NO_HANDSHAKE                   = 1,
    /// No Connection: Upgrade or Upgrade: websocket header in handshake request.
    WS_ERR_HANDSHAKE_NO_VERSION           = 2,
    /// No Sec-WebSocket-Version header in handshake request.
    WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION  = 3,
    /// Unsupported WebSocket version requested by client.
    WS_ERR_HANDSHAKE_NO_KEY               = 4,
    /// No Sec-WebSocket-Key header in handshake request.
    WS_ERR_HANDSHAKE_ACCEPT               = 5,
    /// No Sec-WebSocket-Accept header or wrong value.
    WS_ERR_UNAUTHORIZED                   = 6,
    /// The server rejected the username or password for authentication.
    WS_ERR_PAYLOAD_TOO_BIG                = 10,
    /// Payload too big for supplied buffer.
    WS_ERR_INCOMPLETE_FRAME               = 11,
    /// Incomplete frame received.
    WS_ERROR_INCOMPLETE_FRAME_SENT        = 15,
    WS_ERROR_ZERO_BYTE_FRAME_SENT         = 15,
    WS_ERR_TIMEOUT                        = 20,
    WS_ERR_NET_EXCEPTION                  = 30,
    WS_ERR_OTHER                          = 50,
    
};

class WebSocketEventArgs
{
public:
    WebSocketEventArgs(WebSocketConnection& connection,
                       WebSocketError error = WS_ERR_NONE):
        _connection(connection),
        _error(error)
    {
    }
    
    bool hasError() const
    {
        return _error != WS_ERR_NONE;
    }

    WebSocketError getError() const
    {
        return _error;
    }

    void setError(WebSocketError error)
    {
        _error = error;
    }

    const WebSocketConnection& getConnectionRef() const
    {
        return _connection;
    }

private:
    WebSocketConnection& _connection;
    WebSocketError _error;
    
};

class WebSocketFrameEventArgs: public WebSocketEventArgs
{
public:
    WebSocketFrameEventArgs(WebSocketConnection& connection,
                            WebSocketFrame& frame,
                            WebSocketError error = WS_ERR_NONE):
        WebSocketEventArgs(connection,error),
        _frame(frame)
    {
    }

    const WebSocketFrame& getFrameRef() const
    {
        return _frame;
    }



private:
    WebSocketFrame& _frame;

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
