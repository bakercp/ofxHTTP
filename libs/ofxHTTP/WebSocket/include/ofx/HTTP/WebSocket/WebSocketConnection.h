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


#include <queue>
#include "Poco/Exception.h"
#include "Poco/Timespan.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/WebSocket/WebSocketRouteSettings.h"
#include "ofx/HTTP/WebSocket/WebSocketEvents.h"
#include "ofx/HTTP/WebSocket/WebSocketFrame.h"
#include "ofx/HTTP/Utils/Utils.h"


namespace ofx {
namespace HTTP {


class WebSocketRoute;


class WebSocketConnection: public AbstractWebSocketConnection
    /// \brief A thread safe WebSocketConnection represents a
    ///         WebSocket connection with a single client.
    /// \details Frames can be sent across thread boundaries and are
    ///         queued for sending during the WebSocketConnection's
    ///         service loop.
{
public:
    WebSocketConnection(WebSocketRoute& parent);
        ///< \brief Create a WebSocketConnection.
        ///< \param parent A reference to the parent WebSocketRoute.

    virtual ~WebSocketConnection();
        ///< \brief Destroy the WebSocketConnection.

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response);
        ///< \brief Handle an HTTPServerRequest with an HTTPServerResponse.
        ///< \param request The HTTPServerRequest to handle.
        ///< \param response The HTTPServerResponse to return.

    bool sendFrame(const WebSocketFrame& frame) const;
        ///< \brief Queue a frame to be sent.
        ///< 
        ///< \returns false if frame not queued

    void close();

    virtual void frameReceived(const WebSocketFrame& frame);
        ///< \brief Called when a WebSocketFrame is received.
        ///< \details Subclasses can implement this method.
        ///< \param frame The WebSocketFrame that was received.

    virtual void frameSent(const WebSocketFrame& frame,
                           std::size_t numBytesSent);
        ///< \brief Called when a WebSocketFrame is sent.
        ///< \details Subclasses can implement this method.
        ///< \param frame The WebSocketFrame that was sent.
        ///< \param numBytesSent The number of bytes sent in the WebSocketFrame.

    virtual void socketClosed();
        ///< \brief Called when a WebSocketConnection is closed.
        ///< \details Subclasses can implement this method.

    Poco::Net::NameValueCollection getRequestHeaders() const;
        ///< \returns The request headers submitted when
        ///<        establishing this connection.

    Poco::Net::SocketAddress getClientAddress() const;
        ///< \returns the SocketAddress of the client that
        ///<        established this connection.

    bool isConnected() const;
        ///< \returns true iff this WebSocketConnect is connected to a client.

//    std::string getSubprotocol() const;

    std::size_t getSendQueueSize() const;
        ///< \returns the size of the send queue.

    void clearSendQueue();
        ///< \brief Clears the send queue.

protected:
    WebSocketRoute& _parent;
        ///< \brief A reference to the parent WebSocketRoute.

private:
    void handleOrigin(Poco::Net::HTTPServerRequest& request,
                      Poco::Net::HTTPServerResponse& response);
    
    void handleExtensions(Poco::Net::HTTPServerRequest& request,
                          Poco::Net::HTTPServerResponse& response);
    
    void processFrameQueue(Poco::Net::WebSocket& ws);
    
    // this is all fixed in Poco 1.4.6 and 1.5.+
    void applyFirefoxHack(Poco::Net::HTTPServerRequest& request);

    Poco::Net::NameValueCollection _requestHeaders;
    Poco::Net::SocketAddress _clientAddress;

    bool _isConnected;

    mutable std::queue<WebSocketFrame> _frameQueue;

    mutable Poco::FastMutex _mutex;
    
};


} } // namespace ofx::HTTP
