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
#include "Poco/Net/HTTPServerRequest.h"
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
    /// \brief No Connection: Upgrade or Upgrade:
    ///        websocket header in handshake request.
    WS_ERR_NO_HANDSHAKE                   = Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE,
    /// \brief No Sec-WebSocket-Version header in handshake request.
    WS_ERR_HANDSHAKE_NO_VERSION           = Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION,
    /// \brief Unsupported WebSocket version requested by client.
    WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION  = Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION,
    /// \brief No Sec-WebSocket-Key header in handshake request.
    WS_ERR_HANDSHAKE_NO_KEY               = Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY,
    /// \brief No Sec-WebSocket-Accept header or wrong value.
    WS_ERR_HANDSHAKE_ACCEPT               = Poco::Net::WebSocket::WS_ERR_HANDSHAKE_ACCEPT,
    /// \brief The server rejected the username or password for authentication.
    WS_ERR_UNAUTHORIZED                   = Poco::Net::WebSocket::WS_ERR_UNAUTHORIZED,
    /// \brief Payload too big for supplied buffer.
    WS_ERR_PAYLOAD_TOO_BIG                = Poco::Net::WebSocket::WS_ERR_PAYLOAD_TOO_BIG,
    /// \brief Incomplete frame received.
    WS_ERR_INCOMPLETE_FRAME               = Poco::Net::WebSocket::WS_ERR_INCOMPLETE_FRAME,
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
    /// \param sessionId The session id..
    /// \param connection A reference to the associated WebSocketConnection.
    WebSocketEventArgs(const Poco::UUID& sessionId,
                       const WebSocketConnection& connection):
        _sessionId(sessionId),
        _connection(connection)
    {
    }
    
    /// \brief Get the session id associated with this event.
    ///
    /// The session id is established by the SessionCache.  If no SessionCache
    /// is used, this will always return Poco::UUID::null.
    ///
    /// \returns the session id or Poco::UUID::null if not set.
    const Poco::UUID& getSessionId() const
    {
        return _sessionId;
    }

    const WebSocketConnection& getConnectionRef() const
    {
        return _connection;
    }

private:
    /// \brief The session id, if available.  Poco::UUID::null if null.
    const Poco::UUID& _sessionId;

    /// \brief A reference to the WebSocketConnection.
    const WebSocketConnection& _connection;

};


class WebSocketErrorEventArgs: public WebSocketEventArgs
{
public:
    WebSocketErrorEventArgs(const Poco::UUID& sessionId,
                           const WebSocketConnection& connection,
                           WebSocketError error):
        WebSocketEventArgs(sessionId, connection),
        _error(error)
    {
    }

    /// \returns the error code if any, otherwise WS_ERROR_NONE.
    WebSocketError getError() const
    {
        return _error;
    }

protected:
    /// \brief The WebSocketError associated with the event.
    WebSocketError _error;


};



class WebSocketOpenEventArgs: public WebSocketEventArgs
{
public:
    WebSocketOpenEventArgs(const Poco::UUID& sessionId,
                           const WebSocketConnection& connection,
                           const Poco::Net::HTTPServerRequest& request):
        WebSocketEventArgs(sessionId, connection),
        _request(request)
    {
    }


    const Poco::Net::HTTPServerRequest& getRequest() const
    {
        return _request;
    }

protected:
    const Poco::Net::HTTPServerRequest& _request;
    
};


class WebSocketCloseEventArgs: public WebSocketEventArgs
{
public:
    WebSocketCloseEventArgs(const Poco::UUID& sessionId,
                            const WebSocketConnection& connection,
                            unsigned short code,
                            const std::string& reason):
        WebSocketEventArgs(sessionId, connection),
        _code(code),
        _reason(reason)
    {
    }

    /// \returns A code, 0 if unset.
    unsigned short getCode() const
    {
        return _code;
    }

    /// \returns A UTF-8 encoded client close reason, empty if unset.
    const std::string& getReason() const
    {
        return _reason;
    }

protected:
    /// \brief The WebSocket status code (optional).
    ///
    /// This is parsed during frame close events, otherwise 0.
    ///
    /// \sa http://tools.ietf.org/html/rfc6455#section-5.5.1
    unsigned short _code;

    /// \brief The WebSocket UTF-8 encoded reason code (optional)
    ///
    /// This is parsed during frame close events, otherwise empty.
    ///
    /// \sa http://tools.ietf.org/html/rfc6455#section-5.5.1
    const std::string& _reason;

};


class WebSocketFrameEventArgs: public WebSocketEventArgs
{
public:
    /// \brief Create a WebSocketFrameEventArgs object.
    /// \param connection A reference to the associated WebSocketConnection.
    /// \param error An error, if any, associated with the event.
    WebSocketFrameEventArgs(const WebSocketFrame& frame,
                            const Poco::UUID& sessionId,
                            const WebSocketConnection& connection):
        WebSocketEventArgs(sessionId, connection),
        _frame(frame)
    {
    }

    /// \returns A reference to the WebSocketFrame associated with the event.
    const WebSocketFrame& getFrameRef() const
    {
        return _frame;
    }

private:
    /// \brief A reference to the WebSocketFrame associated with the event.
    const WebSocketFrame& _frame;

};

class WebSocketEvents
{
public:
    ofEvent<WebSocketOpenEventArgs>  onOpenEvent;
    ofEvent<WebSocketCloseEventArgs> onCloseEvent;
    ofEvent<WebSocketFrameEventArgs> onFrameReceivedEvent;
    ofEvent<WebSocketFrameEventArgs> onFrameSentEvent;
    ofEvent<WebSocketErrorEventArgs> onErrorEvent;
    
};

    
} } // namespace ofx::HTTP
