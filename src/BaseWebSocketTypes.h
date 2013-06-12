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
#include "WebSocketEvents.h"
#include "WebSocketFrame.h"


namespace ofx {
namespace HTTP {


class BaseWebSocketRouteHandler : public BaseServerRouteHandler {
public:
    BaseWebSocketRouteHandler(const BaseRouteSettings& settings) :
        BaseServerRouteHandler(settings)
    {
    }

    virtual ~BaseWebSocketRouteHandler()
    {
    }
    
    virtual bool sendFrame(const WebSocketFrame& _frame) = 0;
    virtual void disconnect() = 0;
    
};

class BaseWebSocketSessionManager {
public:
    
    BaseWebSocketSessionManager()
    {
    }
    
    virtual ~BaseWebSocketSessionManager()
    {
    }

    //virtual void getSessionData(ofxWebSocketRouteHandler* handler) = 0;

    bool sendFrame(BaseWebSocketRouteHandler* handler,
                   const WebSocketFrame& frame)
    {
        if(handler != NULL) {
            return handler->sendFrame(frame);
        } else {
            ofLogError("ofxBaseWebSocketSessionManager::sendFrame") << "handler == NULL";
            return false;
        }
    }
    
    void disconnect(BaseWebSocketRouteHandler* handler) {
        if(handler != NULL) {
            handler->disconnect();
        } else {
            ofLogError("ofxBaseWebSocketSessionManager::disconnect") << "handler == NULL";
        }
    }
    
    void disconnectAll() {
        ofScopedLock lock(mutex);

        HandlerSetIter iter = handlers.begin();

        while(iter != handlers.end()) {
            disconnect(*iter);
            ++iter;
        }
    }
    
    bool sendBinary(BaseWebSocketRouteHandler* handler, ofBaseHasPixels& image) {
        return sendBinary(handler, image.getPixelsRef());
    }
    
    bool sendBinary(BaseWebSocketRouteHandler* handler, ofPixels& pixels) {
        return sendBinary(handler,pixels.getPixels(),static_cast<unsigned int>(pixels.size()));
    }

    bool sendBinary(BaseWebSocketRouteHandler* handler, char* data, size_t size) {
        return sendFrame(handler,WebSocketFrame(data,size,Poco::Net::WebSocket::FRAME_BINARY));
    }

    bool sendBinary(BaseWebSocketRouteHandler* handler, unsigned char * data, size_t size ) {
        return sendFrame(handler,WebSocketFrame(data,size,Poco::Net::WebSocket::FRAME_BINARY));
    }
    
    void broadcast(ofPixelsRef pixels) {
        ofScopedLock lock(mutex);
        std::set<BaseWebSocketRouteHandler*>::iterator iter = handlers.begin();
        
        int numChannels = pixels.getNumChannels();
        int width       = pixels.getWidth();
        int height      = pixels.getHeight();
        
        while(iter != handlers.end()) {
            ofPixels pixels;
            //sendFrame(*iter,frame);
            ++iter;
        }
    }
    
    void broadcast(const string& text) {
        broadcast(WebSocketFrame(text));
    }

    
    void broadcast(const WebSocketFrame& frame) {
        ofScopedLock lock(mutex);

        std::set<BaseWebSocketRouteHandler*>::iterator iter = handlers.begin();

        while(iter != handlers.end()) {
            sendFrame(*iter,frame);
            ++iter;
        }
    }
    
    template<class ListenerClass>
    void registerWebSocketEvents(ListenerClass * listener) {
        ofAddListener(events.onOpenEvent, listener, &ListenerClass::onOpenEvent);
        ofAddListener(events.onCloseEvent,listener, &ListenerClass::onCloseEvent);
        ofAddListener(events.onFrameReceivedEvent, listener, &ListenerClass::onFrameReceivedEvent);
        ofAddListener(events.onFrameSentEvent,listener,&ListenerClass::onFrameSentEvent);
        ofAddListener(events.onErrorEvent,listener,&ListenerClass::onErrorEvent);
    }
    
    template<class ListenerClass>
    void unregisterWebSocketEvents(ListenerClass * listener) {
        ofRemoveListener(events.onOpenEvent,listener,&ListenerClass::onOpenEvent);
        ofRemoveListener(events.onCloseEvent,listener,&ListenerClass::onCloseEvent);
        ofRemoveListener(events.onFrameReceivedEvent,listener,&ListenerClass::onFrameReceivedEvent);
        ofRemoveListener(events.onFrameSentEvent,listener,&ListenerClass::onFrameSentEvent);
        ofRemoveListener(events.onErrorEvent,listener,&ListenerClass::onErrorEvent);
    }
    
    void registerRouteHandler(BaseWebSocketRouteHandler* handler) {
        cout << "registering ..." << endl;

        ofScopedLock lock(mutex);
        if(!handlers.insert(handler).second) {
            ofLogError("ofxWebSocketManager::registerRouteHandler") << "Element was already in set!";
        }
    }
    
    void unregisterRouteHandler(BaseWebSocketRouteHandler* handler) {
        cout << "unregistering ..." << endl;
        ofScopedLock lock(mutex);
        size_t numErased = handlers.erase(handler);
        
        if(numErased != 1) {
            ofLogError("ofxWebSocketManager::unregisterRouteHandler") << "Num erased != 1: " << numErased;
        }
    }

    size_t getNumClientsConnected() {
        ofScopedLock lock(mutex);
        return handlers.size();
    }
    
    vector<string> getAvailableSubprotcols() {
        ofScopedLock lock(mutex);

        std::vector<std::string> availableSubprotocols;

        EventMapIter eventIter = eventMap.begin();
        
        while(eventIter != eventMap.end()) {
            availableSubprotocols.push_back((*eventIter).first);
            ++eventIter;
        }
        
        return availableSubprotocols;
    }
        
    bool selectSubprotocol(const std::vector<std::string>& proposedSubprotocols,
                           std::string& selectedSubprotocol) {

        selectedSubprotocol.clear();

        if(proposedSubprotocols.empty()) {
            return false;
        } else {
            ofScopedLock lock(mutex);

            std::vector<std::string>::const_iterator iter = proposedSubprotocols.begin();

            while(iter != proposedSubprotocols.end()) {

                EventMapIter eventIter = eventMap.find(*iter);

                if(eventIter != eventMap.end()) {
                    selectedSubprotocol = (*eventIter).first;
                    return true;
                }
                ++iter;
            }
            return false;
        }
    }
    
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


} }
