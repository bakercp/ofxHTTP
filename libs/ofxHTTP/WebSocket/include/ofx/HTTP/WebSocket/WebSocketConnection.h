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
#include "ofx/HTTP/WebSocket/BaseWebSocketSessionManager.h"
#include "ofx/HTTP/WebSocket/WebSocketRouteInterface.h"
#include "ofx/HTTP/WebSocket/WebSocketRouteSettings.h"
#include "ofx/HTTP/WebSocket/WebSocketEvents.h"
#include "ofx/HTTP/WebSocket/WebSocketFrame.h"
#include "ofx/HTTP/Utils/Utils.h"


namespace ofx {
namespace HTTP {

// TODO:
// - move default constants to enum


class WebSocketConnection: public AbstractWebSocketConnection
{
public:
    WebSocketConnection(WebSocketRouteInterface& parent);

    virtual ~WebSocketConnection();

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response);

    bool sendFrame(const WebSocketFrame& frame) const; // returns false if frame not queued

    void stop();

    virtual void frameReceived(const WebSocketFrame& frame);
    virtual void frameSent(const WebSocketFrame& frame, std::size_t numBytesSent);
    virtual void socketClosed();

    void broadcast(const WebSocketFrame& frame) const;

    Poco::Net::NameValueCollection getRequestHeaders() const;
    Poco::Net::SocketAddress getClientAddress() const;

    bool isConnected() const;

//    std::string getSubprotocol() const;

    std::size_t getSendQueueSize() const;
    void clearSendQueue();

protected:
    void setIsConnected(bool bIsConnected);

    WebSocketRouteInterface& _parent;

private:
//    void handleErrorResponse(Poco::Net::HTTPServerResponse& response);

//    void handleSubprotocols(Poco::Net::HTTPServerRequest& request,
//                            Poco::Net::HTTPServerResponse& response);
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

