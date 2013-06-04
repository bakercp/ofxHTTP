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

using std::memset;
using std::swap;
using std::queue;

using Poco::Timespan;
using Poco::TimeoutException;
using Poco::Net::HTTPException;
using Poco::Net::Socket;
using Poco::Net::NetException;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;


namespace ofx {
namespace HTTP {


class ofxWebSocketRouteSettings : public BaseRouteSettings {
public:
    ofxWebSocketRouteSettings(const string& _route = "/") :
    BaseRouteSettings(_route),
    subprotocol(""),
    bAutoPingPongResponse(true),
    bKeepAlive(true),
    bAllowEmptySubprotocol(false),
    bAllowCrossOriginConnections(false),
    bIsBinary(false),
    receiveTimeout(Timespan(60 * Timespan::SECONDS)),
    sendTimeout(Timespan(60 * Timespan::SECONDS)),
    pollTimeout(Timespan(10 * Timespan::MILLISECONDS)),
    bufferSize(1024)
    { }
    
    virtual ~ofxWebSocketRouteSettings() { }
    
    string subprotocol;

    bool bAllowEmptySubprotocol;
    bool bAllowCrossOriginConnections;
    
    bool bIsBinary;
    
    bool bAutoPingPongResponse; // automatically return pong frames if true
    bool bKeepAlive;
    
    Timespan receiveTimeout; // remember that timespan uses microseconds
    Timespan sendTimeout;    // remember that timespan uses microseconds
    Timespan pollTimeout;    // remember that timespan uses microseconds
    
    size_t bufferSize;
    
};


//------------------------------------------------------------------------------
class ofxWebSocketRouteHandler : public ofxBaseWebSocketRouteHandler {
public:

    typedef ofxWebSocketRouteSettings Settings;
    
    ofxWebSocketRouteHandler(ofxBaseWebSocketSessionManager& _manager, const Settings& _settings);
    virtual ~ofxWebSocketRouteHandler();
    
    virtual void handleExchange(ServerExchange& exchange);
    
    bool sendFrame(const ofxWebSocketFrame& _frame); // returns false if frame not queued
    void disconnect();

    virtual void frameReceived(const ofxWebSocketFrame& _frame);
    virtual void frameSent(const ofxWebSocketFrame& _frame, int _nBytesSent);
    virtual void socketClosed();
    
    void broadcast(const ofxWebSocketFrame& _frame) {
        manager.broadcast(_frame);
    }
    
    bool isConnected();
    string getSubprotocol();
    
    size_t getSendQueueSize();
    void   clearSendQueue();
    
protected:
    void setIsConnected(bool _bIsConnected);
    
    Settings settings;

    ofxBaseWebSocketSessionManager& manager;
    
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
        
    bool bIsConnected;
    
    ofMutex mutex;
    queue<ofxWebSocketFrame> frameQueue;
    
    
};


} }
            
