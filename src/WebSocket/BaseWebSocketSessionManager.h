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


#include "ofBaseTypes.h"
#include "BaseWebSocketRouteHandler.h"
#include "WebSocketEvents.h"
#include "WebSocketFrame.h"

namespace ofx {
namespace HTTP {


class BaseWebSocketSessionManager {
public:
    BaseWebSocketSessionManager();
    virtual ~BaseWebSocketSessionManager();

    bool sendFrame(BaseWebSocketRouteHandler* handler,const WebSocketFrame& frame);
    void disconnect(BaseWebSocketRouteHandler* handler);

    void disconnectAll();

    bool sendBinary(BaseWebSocketRouteHandler* handler, ofBaseHasPixels& image);

    bool sendBinary(BaseWebSocketRouteHandler* handler, ofPixels& pixels);

    bool sendBinary(BaseWebSocketRouteHandler* handler,
                    unsigned char* data,
                    size_t size);

    void broadcast(ofPixelsRef pixels);

    void broadcast(const std::string& text);

    void broadcast(const WebSocketFrame& frame);

    template<class ListenerClass>
    void registerWebSocketEvents(ListenerClass * listener);

    template<class ListenerClass>
    void unregisterWebSocketEvents(ListenerClass * listener);

    void registerRouteHandler(BaseWebSocketRouteHandler* handler);
    void unregisterRouteHandler(BaseWebSocketRouteHandler* handler);

    size_t getNumClientsConnected();

    std::vector<std::string> getAvailableSubprotcols();

    bool selectSubprotocol(const std::vector<std::string>& proposedSubprotocols,
                           std::string& selectedSubprotocol);
    
    WebsocketEvents events;

protected:
    BaseWebSocketSessionManager(const BaseWebSocketSessionManager& that);
	BaseWebSocketSessionManager& operator = (const BaseWebSocketSessionManager& that);

    typedef std::set<BaseWebSocketRouteHandler*>            HandlerSet;
    typedef std::set<BaseWebSocketRouteHandler*>::iterator  HandlerSetIter;
    typedef std::map<std::string,WebsocketEvents>           EventMap;
    typedef std::map<std::string,WebsocketEvents>::iterator EventMapIter;
    
    HandlerSet handlers;
    
    EventMap eventMap;
    
    mutable Poco::FastMutex mutex; // locks the handlers set
    
};


template<class ListenerClass>
void BaseWebSocketSessionManager::registerWebSocketEvents(ListenerClass * listener) {
    ofAddListener(events.onOpenEvent, listener, &ListenerClass::onOpenEvent);
    ofAddListener(events.onCloseEvent,listener, &ListenerClass::onCloseEvent);
    ofAddListener(events.onFrameReceivedEvent, listener, &ListenerClass::onFrameReceivedEvent);
    ofAddListener(events.onFrameSentEvent,listener,&ListenerClass::onFrameSentEvent);
    ofAddListener(events.onErrorEvent,listener,&ListenerClass::onErrorEvent);
}


template<class ListenerClass>
void BaseWebSocketSessionManager::unregisterWebSocketEvents(ListenerClass * listener) {
    ofRemoveListener(events.onOpenEvent,listener,&ListenerClass::onOpenEvent);
    ofRemoveListener(events.onCloseEvent,listener,&ListenerClass::onCloseEvent);
    ofRemoveListener(events.onFrameReceivedEvent,listener,&ListenerClass::onFrameReceivedEvent);
    ofRemoveListener(events.onFrameSentEvent,listener,&ListenerClass::onFrameSentEvent);
    ofRemoveListener(events.onErrorEvent,listener,&ListenerClass::onErrorEvent);
}


} } // namespace ofx::HTTP
