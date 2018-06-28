//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <queue>
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include "Poco/Timespan.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/WebSocketEvents.h"
#include "ofx/HTTP/WebSocketFrame.h"
#include "ofx/HTTP/WebSocketRoute.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


/// \brief A class representing a WebSocket connection with a single client.
///
/// Frames can be sent across thread boundaries and are queued for sending
/// during the WebSocketConnection's service loop. All accessors are
/// synchronized and thread-safe.
class WebSocketConnection: public BaseRouteHandler_<WebSocketRoute>
{
public:
    /// \brief Create a WebSocketConnection.
    /// \param route A reference to the parent WebSocketRoute.
    WebSocketConnection(WebSocketRoute& route);

    /// \brief Destroy the WebSocketConnection.
    virtual ~WebSocketConnection();

    void handleRequest(ServerEventArgs& evt) override;

    /// \brief Queue a frame to be sent.
    /// \returns false iff frame not queued
    bool sendFrame(const WebSocketFrame& frame) const;

    void stop() override;

//    /// \brief Called when a WebSocketFrame is received.
//    /// \details Subclasses can implement this method.
//    /// \param frame The WebSocketFrame that was received.
//    virtual void frameReceived(const WebSocketFrame& frame);
//
//    /// \brief Called when a WebSocketFrame is sent.
//    /// \details Subclasses can implement this method.
//    /// \param frame The WebSocketFrame that was sent.
//    /// \param numBytesSent The number of bytes sent in the WebSocketFrame.
//    virtual void frameSent(const WebSocketFrame& frame,
//                           std::size_t numBytesSent);

//    /// \brief Called when a WebSocketConnection is closed.
//    /// \details Subclasses can implement this method.
//    virtual void socketClosed();

    /// \returns The original http request headers.
    Poco::Net::NameValueCollection requestHeaders() const;

    /// \returns the client's SocketAddress.
    Poco::Net::SocketAddress clientAddress() const;

    /// \returns true iff this WebSocketConnect is connected to a client.
    bool isConnected() const;

    /// \returns the size of the send queue.
    std::size_t sendQueueSize() const;

    /// \brief Clears the send queue.
    void clearSendQueue();

    /// \brief Get the total bytes sent to the client.
    /// \returns the total bytes sent to the client.
    std::size_t totalBytesSent() const;

    /// \brief Get the total bytes received from the client.
    /// \returns the total bytes received from the client.
    std::size_t totalBytesReceived() const;

    /// \brief Take ownership of the passed std::unique_ptr<ExtensionType>.
    /// \param filter the rvalue reference to the filter.
    void addWebSocketFilter(std::unique_ptr<AbstractWebSocketFilter> filter);

private:
    void handleOrigin(ServerEventArgs& evt);
    
    void handleSubprotocols(ServerEventArgs& evt);

    void handleExtensions(ServerEventArgs& evt);

    std::size_t sendFrames(Poco::Net::WebSocket& ws);

    // this is all fixed in Poco 1.4.6 and 1.5.+
    void applyFirefoxHack(ServerEventArgs& evt);

    /// \brief The original request headers for reference.
    Poco::Net::NameValueCollection _requestHeaders;

    /// \brief The client's SocketAddress for reference.
    Poco::Net::SocketAddress _clientAddress;

    /// \brief True iff the WebSocketConnection is connected to a client.
    bool _isConnected = false;

    /// \brief The total number of bytes sent to the client.
    std::size_t _totalBytesSent = 0;

    /// \brief The total number of bytes received from the client.
    std::size_t _totalBytesReceived = 0;

    /// \brief A collection of WebSocketFilters.
    std::vector<std::unique_ptr<AbstractWebSocketFilter>> _filters;

    /// \brief A queue of the WebSocketFrames scheduled for delivery.
    mutable std::queue<WebSocketFrame> _frameQueue;

    /// \brief A mutex for threadsafe access to the frame queue, etc.
    mutable std::mutex _mutex;

};


} } // namespace ofx::HTTP
