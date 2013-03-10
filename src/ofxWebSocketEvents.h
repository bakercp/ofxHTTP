/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ==============================================================================*/

#pragma once

#include "ofEvents.h"
#include "ofEventUtils.h"

class ofxWebSocketFrame;
class ofxWebSocketRouteHandler;


enum ofxWebSocketError {
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

class ofxWebSocketEventArgs {
public:
    ofxWebSocketEventArgs(ofxWebSocketRouteHandler& _connection, ofxWebSocketError _error = WS_ERR_NONE) : connection(_connection), error(_error) { }
    
    bool hasError() const { return error != WS_ERR_NONE; }
    ofxWebSocketError getError() const { return error; }
    void setError(ofxWebSocketError _error) { error = _error; }
    ofxWebSocketRouteHandler& getConnectionRef() { return connection; }
    
    ofxWebSocketError error;
    ofxWebSocketRouteHandler& connection;
};

class ofxWebSocketFrameEventArgs : public ofxWebSocketEventArgs {
public:
    ofxWebSocketFrameEventArgs(ofxWebSocketRouteHandler& _connection, ofxWebSocketFrame& _frame, ofxWebSocketError _error = WS_ERR_NONE)
    : ofxWebSocketEventArgs(_connection,_error), frame(_frame) { }

    ofxWebSocketFrame& getFrameRef() { return frame; }
    
    ofxWebSocketFrame& frame;
};

class ofxWebsocketEvents {
public:
    ofEvent<ofxWebSocketEventArgs>      onOpenEvent;
    ofEvent<ofxWebSocketEventArgs>      onCloseEvent;
    ofEvent<ofxWebSocketFrameEventArgs> onFrameReceivedEvent;
    ofEvent<ofxWebSocketFrameEventArgs> onFrameSentEvent;
    ofEvent<ofxWebSocketEventArgs>      onErrorEvent;
};