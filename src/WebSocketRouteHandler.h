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
 
 =============================================================================*/


#pragma once


#include <queue>
#include "Poco/Exception.h"
#include "Poco/Timespan.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "BaseTypes.h"
#include "ServerDefaultRouteHandler.h"
#include "BaseWebSocketTypes.h"
#include "WebSocketEvents.h"
#include "WebSocketFrame.h"


namespace ofx {
namespace HTTP {


class WebSocketRouteSettings : public BaseRouteSettings {
public:
    WebSocketRouteSettings(const std::string& _route = "/")
    : BaseRouteSettings(_route)
    , subprotocol("")
    , bAutoPingPongResponse(true)
    , bKeepAlive(true)
    , bAllowEmptySubprotocol(false)
    , bAllowCrossOriginConnections(false)
    , bIsBinary(false)
    , receiveTimeout(Poco::Timespan(60 * Poco::Timespan::SECONDS))
    , sendTimeout(Poco::Timespan(60 * Poco::Timespan::SECONDS))
    , pollTimeout(Poco::Timespan(10 * Poco::Timespan::MILLISECONDS))
    , bufferSize(1024)
    {

    }
    
    virtual ~WebSocketRouteSettings()
    {

    }
    
    std::string subprotocol;

    bool bAllowEmptySubprotocol;
    bool bAllowCrossOriginConnections;
    
    bool bIsBinary;
    
    bool bAutoPingPongResponse; // automatically return pong frames if true
    bool bKeepAlive;
    
    Poco::Timespan receiveTimeout; // remember that timespan uses microseconds
    Poco::Timespan sendTimeout;    // remember that timespan uses microseconds
    Poco::Timespan pollTimeout;    // remember that timespan uses microseconds
    
    size_t bufferSize;
    
};


//------------------------------------------------------------------------------
class WebSocketRouteHandler : public BaseWebSocketRouteHandler {
public:

    typedef WebSocketRouteSettings Settings;
    
    WebSocketRouteHandler(BaseWebSocketSessionManager& manager,
                          const Settings& settings);
    
    virtual ~WebSocketRouteHandler();

    virtual void handleExchange(ServerExchange& exchange);
    
    bool sendFrame(const WebSocketFrame& _frame); // returns false if frame not queued
    void disconnect();

    virtual void frameReceived(const WebSocketFrame& frame);
    virtual void frameSent(const WebSocketFrame& frame, int numBytesSent);
    virtual void socketClosed();
    
    void broadcast(const WebSocketFrame& frame) const;

    bool isConnected() const;
    std::string getSubprotocol() const;
    
    size_t getSendQueueSize() const;
    void   clearSendQueue();
    
protected:
    void setIsConnected(bool _bIsConnected);
    
    Settings settings;

    BaseWebSocketSessionManager& _manager;
    
private:
    void handleSubprotocols(ServerExchange& exchange);
    void handleOrigin(ServerExchange& exchange);
    void handleExtensions(ServerExchange& exchange);
    
    void processFrameQueue(Poco::Net::WebSocket& ws);
    
    // this is all fixed in Poco 1.4.6 and 1.5.+
    void applyFirefoxHack(Poco::Net::HTTPServerRequest& _request) {
        // HACK FOR FIREFOX
        // require websocket upgrade headers
        std::string connectionHeader = Poco::toLower(_request.get("Connection", ""));
        if(Poco::icompare(connectionHeader, "Upgrade") != 0) {
            std::string userAgent = Poco::toLower(_request.get("User-Agent",""));
            if(!userAgent.empty() &&
               !connectionHeader.empty() &&
               ofIsStringInString(userAgent,"firefox") &&
               ofIsStringInString(connectionHeader,"upgrade")) {
                // this request is coming from firefox, which is known to send things that look like:
                // Connection:keep-alive, Upgrade
                // thus failing the standard Poco upgrade test.
                // we can't do this here, but will do a similar hack in the handler
                _request.set("Connection","Upgrade");
            }
        }
    }


    mutable ofMutex mutex;
    bool _bIsConnected;
    std::queue<WebSocketFrame> frameQueue;
    
    
};


} }
            
