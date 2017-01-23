//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/UUID.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "ofEvents.h"
#include "ofEventUtils.h"
#include "ofx/HTTP/ServerEvents.h"


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
class WebSocketEventArgs: public ServerEventArgs
{
public:
    /// \brief Create a WebSocketEventArgs object with the provided params.
    /// \param args The server event arguments.
    /// \param connection A reference to the associated WebSocketConnection.
    WebSocketEventArgs(ServerEventArgs& args,
                       WebSocketConnection& connection):
        ServerEventArgs(args),
        _connection(connection)
    {
    }

    WebSocketConnection& connection()
    {
        return _connection;
    }

private:
    /// \brief A reference to the WebSocketConnection.
    WebSocketConnection& _connection;

};


class WebSocketErrorEventArgs: public WebSocketEventArgs
{
public:
    WebSocketErrorEventArgs(ServerEventArgs& evt,
                            WebSocketConnection& connection,
                            WebSocketError error):
        WebSocketEventArgs(evt, connection),
        _error(error)
    {
    }

    /// \returns the error code if any, otherwise WS_ERROR_NONE.
    WebSocketError error() const
    {
        return _error;
    }

protected:
    /// \brief The WebSocketError associated with the event.
    WebSocketError _error;


};


class WebSocketCloseEventArgs: public WebSocketEventArgs
{
public:
    WebSocketCloseEventArgs(ServerEventArgs& evt,
                            WebSocketConnection& connection,
                            uint16_t code,
                            const std::string& reason):
        WebSocketEventArgs(evt, connection),
        _code(code),
        _reason(reason)
    {
    }

    /// \returns A code, 0 if unset.
    uint16_t code() const
    {
        return _code;
    }

    /// \returns A UTF-8 encoded client close reason, empty if unset.
    const std::string& reason() const
    {
        return _reason;
    }

protected:
    /// \brief The WebSocket status code (optional).
    ///
    /// This is parsed during frame close events, otherwise 0.
    ///
    /// \sa http://tools.ietf.org/html/rfc6455#section-5.5.1
    uint16_t _code;

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
    /// \param args The server event arguments.
    /// \param connection A reference to the associated WebSocketConnection.
    /// \param frame The WebSocketFrame the referenced frame.
    WebSocketFrameEventArgs(ServerEventArgs& args,
                            WebSocketConnection& connection,
                            const WebSocketFrame& frame):
        WebSocketEventArgs(args, connection),
        _frame(frame)
    {
    }

    /// \returns A const reference to the WebSocketFrame associated with the event.
    const WebSocketFrame& frame() const
    {
        return _frame;
    }

private:
    /// \brief A reference to the WebSocketFrame associated with the event.
    const WebSocketFrame& _frame;

};


/// \brief A typedef for WebSocketOpenEventArgs.
typedef WebSocketEventArgs WebSocketOpenEventArgs;


/// \brief A collection of events called by WebSocketConnections.
class WebSocketEvents
{
public:
    /// \brief An event that is called when a Web Socket connection is opened.
    ofEvent<WebSocketOpenEventArgs>  onWebSocketOpenEvent;

    /// \brief An event that is called when a Web Socket connection is closed.
    ofEvent<WebSocketCloseEventArgs> onWebSocketCloseEvent;

    /// \brief An event that is called when a Web Socket frame is received.
    ofEvent<WebSocketFrameEventArgs> onWebSocketFrameReceivedEvent;

    /// \brief An event that is called when a Web Socket frame is sent.
    ofEvent<WebSocketFrameEventArgs> onWebSocketFrameSentEvent;

    /// \brief An event that is called when a Web Socket encounters an error.
    ofEvent<WebSocketErrorEventArgs> onWebSocketErrorEvent;
    
};

    
} } // namespace ofx::HTTP
