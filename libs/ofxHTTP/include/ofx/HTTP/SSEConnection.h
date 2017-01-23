//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <condition_variable>
#include <queue>
//#include "Poco/Buffer.h"
//#include "Poco/Exception.h"
//#include "Poco/Timespan.h"
//#include "Poco/Net/Socket.h"
//#include "Poco/Net/WebSocket.h"
//#include "Poco/Net/NetException.h"
//#include "ofFileUtils.h"
//#include "ofLog.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/HTTPUtils.h"
#include "ofx/HTTP/SSEFrame.h"
#include "ofx/HTTP/SSERoute.h"


namespace ofx {
namespace HTTP {


/// \brief A class representing a Server Sent Event Source with a single client.
///
/// Frames can be sent across thread boundaries and are queued for sending
/// during the SSERouteConnection's service loop. All accessors are
/// synchronized and thread-safe.
class SSEConnection: public /*BaseConnection_<SSERoute, IndexedSSEFrame>//*/BaseRouteHandler_<SSERoute>
{
public:
    /// \brief Create a SSEConnection.
    /// \param route A reference to the parent SSERoute.
    SSEConnection(SSERoute& route);

    /// \brief Destroy the SSEConnection.
    virtual ~SSEConnection();

    void handleRequest(ServerEventArgs& evt) override;

    void stop() override;

    /// \brief Queue data to be sent to the client.
    /// \param frame The data to send to the client.
    /// \param cache True if the frame should be cached.
    /// \returns false iff frame was not queued.
    bool send(const IndexedSSEFrame& frame) const;

    /// \returns The original http request headers.
    Poco::Net::NameValueCollection requestHeaders() const;

    /// \returns the client's SocketAddress.
    Poco::Net::SocketAddress clientAddress() const;

    /// \returns true iff this SSEConnection is connected to a client.
    bool isConnected() const;

    /// \returns the size of the send queue.
    std::size_t sendQueueSize() const;

    /// \brief Clears the send queue.
    void clearSendQueue();

    /// \brief Get the total bytes sent to the client.
    /// \returns the total bytes sent to the client.
    std::size_t totalBytesSent() const;

private:
    /// \brief The original request headers for reference.
    Poco::Net::NameValueCollection _requestHeaders;

    /// \brief The client's SocketAddress for reference.
    Poco::Net::SocketAddress _clientAddress;

    /// \brief True iff the SSEConnection is connected to a client.
    bool _isConnected = false;

    /// \brief The total number of bytes sent to the client.
    std::size_t _totalBytesSent = 0;

    /// \brief A queue of the SSEFrames scheduled for delivery.
    mutable std::queue<IndexedSSEFrame> _frameQueue;

    /// \brief A mutex for threadsafe access to the frame queue, etc.
    mutable std::mutex _mutex;

    /// \brief A condition for restarting the thread when a frame is ready.
    mutable std::condition_variable _condition;

    /// \brief The server sent event content type "text/event-stream".
    static const std::string SSE_CONTENT_TYPE;

    /// \brief Event boundary.
    static const std::string SSE_EVENT_BOUNDARY;

};


} } // namespace ofx::HTTP
