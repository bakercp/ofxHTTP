//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SSEConnection.h"
#include "ofx/HTTP/SSERoute.h"
#include "ofx/HTTP/SSEEvents.h"
#include "Poco/Net/NetException.h"
#include <chrono>
#include <thread>


namespace ofx {
namespace HTTP {


const std::string SSEConnection::SSE_CONTENT_TYPE = "text/event-stream";
const std::string SSEConnection::SSE_EVENT_BOUNDARY = "\n\n";


SSEConnection::SSEConnection(SSERoute& _route):
//    BaseConnection_<SSERoute, IndexedSSEFrame>(_route)
    BaseRouteHandler_<SSERoute>(_route)
{
    route().registerConnection(this);
}


SSEConnection::~SSEConnection()
{
    // std::cout << "1. destroying connection ... " << std::endl;
    stop();
    route().unregisterConnection(this);
    // std::cout << "2. destroying connection ... " << std::endl;
}


void SSEConnection::stop()
{
//    std::cout << "STOPPING" << std::endl;
    std::unique_lock<std::mutex> lock(_mutex);
    _isConnected = false;
    _condition.notify_all();
}


void SSEConnection::handleRequest(ServerEventArgs& evt)
{
    try
    {
        _requestHeaders = evt.request();
        _clientAddress = evt.request().clientAddress();

        auto& response = evt.response();
        // now set response headers.

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
        response.setKeepAlive(true);
        response.setContentType(SSE_CONTENT_TYPE);
        response.setChunkedTransferEncoding(true);
        response.set("Cache-Control", "no-cache");

        // Send the response headers and open the stream for writing.
        std::ostream& responseStream = response.send();

        // Mark the connection as open.
        _mutex.lock();
        _isConnected = true;
        _mutex.unlock();

        SSEOpenEventArgs eventArgs(evt, *this);
        ofNotifyEvent(route().events.onSSEOpenEvent, eventArgs, this);

        /// \brief Send the client retry interval.
        responseStream << "retry: " << route().settings().getClientRetryInterval();
        responseStream << SSE_EVENT_BOUNDARY;
        responseStream.flush();

        do
        {
            while (sendQueueSize() > 0) // lock
            {
                _mutex.lock();
                IndexedSSEFrame frame = _frameQueue.front();
                _frameQueue.pop();
                _mutex.unlock();

                responseStream << "data: " << ofGetTimestampString() << SSE_EVENT_BOUNDARY;
                responseStream.flush();

//                if (frame.size() > 0)
//                {
//                    if (ws.poll(route().settings().getPollTimeout(),
//                                Poco::Net::Socket::SELECT_WRITE))
//                    {
//                        int numBytesSent = 0;
//
//                        const char* pData = frame.getCharPtr();
//
//                        numBytesSent = ws.sendFrame(pData,
//                                                    frame.size(),
//                                                    frame.flags());
//
//                        _totalBytesSent += numBytesSent;
//
//                        // WebSocketError error = WS_ERR_NONE;
//
//                        if (0 == numBytesSent)
//                        {
//                            ofLogWarning("WebSocketConnection::handleRequest") << "WebSocket numBytesSent == 0";
//                            // error = WS_ERROR_ZERO_BYTE_FRAME_SENT;
//                        }
//                        // TODO ofBuffer::size() returns long ... sendFrame returns int ... :/
//                        else if(numBytesSent < frame.size())
//                        {
//                            ofLogWarning("WebSocketConnection::handleRequest") << "WebSocket numBytesSent < frame.size()";
//                            // error = WS_ERROR_INCOMPLETE_FRAME_SENT;
//                        }
//
//                        WebSocketFrameEventArgs eventArgs(evt, *this, frame);
//
//                        ofNotifyEvent(route().events.onWebSocketFrameSentEvent,
//                                      eventArgs,
//                                      this);
//                    }
//                }
            }

//            // Check for read error
//            if (ws.poll(route().settings().getPollTimeout(),
//                        Poco::Net::Socket::SELECT_ERROR))
//            {
//                std::unique_lock<std::mutex> lock(_mutex);
//                _isConnected = false;
//            }

            responseStream << "data: " << ofGetTimestampString() << SSE_EVENT_BOUNDARY;
            responseStream.flush();
            std::unique_lock<std::mutex> lock(_mutex);
            _condition.wait_for(lock, std::chrono::milliseconds(1000));
        }
        while (_isConnected && responseStream);

        ofLogNotice("SSEConnection::handleRequest") << "SSE connection closed.";

    }
    catch (const Poco::TimeoutException& exc)
    {
        ofLogError("SSEConnection::handleRequest") << "TimeoutException: " << exc.code() << " Desc: " << exc.what();
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
//        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_TIMEOUT);
//        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    }
    catch (const Poco::Net::NetException& exc)
    {
        ofLogError("SSEConnection::handleRequest") << "NetException: " << exc.code() << " Desc: " << exc.what();
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
//        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_NET_EXCEPTION);
//        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("SSEConnection::handleRequest") << "Exception: " << exc.displayText();
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
//        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_OTHER);
//        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
    }
    catch (const std::exception& exc)
    {
        ofLogError("SSEConnection::handleRequest") << "exception: " << exc.what();
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
//        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_OTHER);
//        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
    }
    catch ( ... )
    {
        ofLogError("SSEConnection::handleRequest") << "... Unknown exception.";
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
//        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_OTHER);
//        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
    }
}


bool SSEConnection::send(const IndexedSSEFrame& frame) const
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (_isConnected)
    {
        _frameQueue.push(frame);
        _condition.notify_all();
        return true;
    }
    else
    {
        ofLogError("SSEConnection::sendFrame") << "Not connected, frame not sent.";
        return false;
    }
}


std::size_t SSEConnection::sendQueueSize() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _frameQueue.size();
}


void SSEConnection::clearSendQueue()
{
    std::unique_lock<std::mutex> lock(_mutex);
    std::queue<IndexedSSEFrame> empty; // a way to clear queues.
    std::swap(_frameQueue, empty);
}

                   
Poco::Net::NameValueCollection SSEConnection::requestHeaders() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _requestHeaders;
}


Poco::Net::SocketAddress SSEConnection::clientAddress() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _clientAddress;
}


bool SSEConnection::isConnected() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _isConnected;
}


std::size_t SSEConnection::totalBytesSent() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _totalBytesSent;
}


} } // namespace ofx::HTTP
