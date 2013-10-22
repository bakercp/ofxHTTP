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


#include <set>
#include "ofBaseTypes.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/WebSocket/WebSocketEvents.h"
#include "ofx/HTTP/WebSocket/WebSocketFrame.h"


namespace ofx {
namespace HTTP {


class BaseWebSocketSessionManager
{
public:
//    typedef std::vector<std::string>                 SubprotocolList;
//    typedef std::vector<std::string>::iterator       SubprotocolListIter;
//    typedef std::vector<std::string>::const_iterator SubprotocolListConstIter;

    BaseWebSocketSessionManager();
    virtual ~BaseWebSocketSessionManager();

    bool sendFrame(AbstractWebSocketConnection* connection, const WebSocketFrame& frame);
    void close(AbstractWebSocketConnection* connection);

    void close();

    bool sendBinary(AbstractWebSocketConnection* connection, ofBaseHasPixels& image);
    bool sendBinary(AbstractWebSocketConnection* connection, ofPixels& pixels);
    bool sendBinary(AbstractWebSocketConnection* connection,
                    unsigned char* data,
                    std::size_t size);

    void broadcast(ofPixels& pixels);
    void broadcast(const std::string& text);
    void broadcast(const WebSocketFrame& frame);

    template<class ListenerClass>
    void registerWebSocketEvents(ListenerClass* listener);

    template<class ListenerClass>
    void unregisterWebSocketEvents(ListenerClass* listener);

    void registerWebSocketConnection(AbstractWebSocketConnection* connection);
    void unregisterWebSocketConnection(AbstractWebSocketConnection* connection);

    std::size_t getNumWebSocketConnections();

//    SubprotocolList getAvailableSubprotcols();
//
//    bool selectSubprotocol(const SubprotocolList& proposedSubprotocols,
//                           std::string& selectedSubprotocol);
    WebSocketEvents events;

protected:
    BaseWebSocketSessionManager(const BaseWebSocketSessionManager& that);
	BaseWebSocketSessionManager& operator = (const BaseWebSocketSessionManager& that);

    typedef std::set<AbstractWebSocketConnection*>              WebSocketConnections;
    typedef std::set<AbstractWebSocketConnection*>::iterator    WebSocketConnectionsIter;
    typedef std::map<std::string,WebSocketEvents>               EventMap;
    typedef std::map<std::string,WebSocketEvents>::iterator     EventMapIter;
    
    WebSocketConnections _connections;
    
    EventMap _eventMap;
    
    mutable ofMutex _mutex; // locks the handlers set
    
};


template<class ListenerClass>
void BaseWebSocketSessionManager::registerWebSocketEvents(ListenerClass* listener) {
    ofAddListener(events.onOpenEvent, listener, &ListenerClass::onWebSocketOpenEvent);
    ofAddListener(events.onCloseEvent,listener, &ListenerClass::onWebSocketCloseEvent);
    ofAddListener(events.onFrameReceivedEvent, listener, &ListenerClass::onWebSocketFrameReceivedEvent);
    ofAddListener(events.onFrameSentEvent,listener,&ListenerClass::onWebSocketFrameSentEvent);
    ofAddListener(events.onErrorEvent,listener,&ListenerClass::onWebSocketErrorEvent);
}


template<class ListenerClass>
void BaseWebSocketSessionManager::unregisterWebSocketEvents(ListenerClass* listener) {
    ofRemoveListener(events.onOpenEvent,listener,&ListenerClass::onWebSocketOpenEvent);
    ofRemoveListener(events.onCloseEvent,listener,&ListenerClass::onWebSocketCloseEvent);
    ofRemoveListener(events.onFrameReceivedEvent,listener,&ListenerClass::onWebSocketFrameReceivedEvent);
    ofRemoveListener(events.onFrameSentEvent,listener,&ListenerClass::onWebSocketFrameSentEvent);
    ofRemoveListener(events.onErrorEvent,listener,&ListenerClass::onWebSocketErrorEvent);
}


} } // namespace ofx::HTTP
