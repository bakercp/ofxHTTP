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
 
 ==============================================================================*/

#pragma once

#include "ofImage.h"

#include "ofxWebsocketEvents.h"
#include "ofxWebSocketFrame.h"

class ofxBaseWebSocketRouteHandler : public ofxHTTPServerRouteHandler {
public:
    ofxBaseWebSocketRouteHandler() { }
    virtual ~ofxBaseWebSocketRouteHandler() { }
    
    virtual bool sendFrame(const ofxWebSocketFrame& _frame) = 0;
    virtual void disconnect() = 0;
};

class ofxBaseWebSocketSessionManager {
public:
    
    ofxBaseWebSocketSessionManager() { }
    virtual ~ofxBaseWebSocketSessionManager() { }

    //virtual void getSessionData(ofxWebSocketRouteHandler* handler) = 0;

    bool sendFrame(ofxBaseWebSocketRouteHandler* handler, const ofxWebSocketFrame& frame) {
        if(handler != NULL) {
            return handler->sendFrame(frame);
        } else {
            ofLogError("ofxBaseWebSocketSessionManager::sendFrame") << "handler == NULL";
            return false;
        }
    }
    
    void disconnect(ofxBaseWebSocketRouteHandler* handler) {
        if(handler != NULL) {
            handler->disconnect();
        } else {
            ofLogError("ofxBaseWebSocketSessionManager::disconnect") << "handler == NULL";
        }
    }
    
    void disconnectAll() {
        ofScopedLock lock(mutex);
        set<ofxBaseWebSocketRouteHandler*>::iterator iter = handlers.begin();
        while(iter != handlers.end()) {
            disconnect(*iter);
            ++iter;
        }
    }
    
    bool sendBinary(ofxBaseWebSocketRouteHandler* handler, ofImage& image) {
        return sendBinary(handler, image.getPixelsRef());
    }
    
    bool sendBinary(ofxBaseWebSocketRouteHandler* handler, ofPixels& pixels) {
        return sendBinary(handler,pixels.getPixels(),static_cast<unsigned int>(pixels.size()));
    }

    bool sendBinary(ofxBaseWebSocketRouteHandler* handler, char * data, unsigned int size ) {
        return sendFrame(handler,ofxWebSocketFrame(data,size,WebSocket::FRAME_BINARY));
    }

    bool sendBinary(ofxBaseWebSocketRouteHandler* handler, unsigned char * data, unsigned int size ) {
        return sendFrame(handler,ofxWebSocketFrame(data,size,WebSocket::FRAME_BINARY));
    }
    
    void broadcast(const ofxWebSocketFrame& frame) {
        ofScopedLock lock(mutex);
        set<ofxBaseWebSocketRouteHandler*>::iterator iter = handlers.begin();
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
    
    void registerRouteHandler(ofxBaseWebSocketRouteHandler* handler) {
        cout << "registering ..." << endl;

        ofScopedLock lock(mutex);
        if(!handlers.insert(handler).second) {
            ofLogError("ofxWebSocketManager::registerRouteHandler") << "Element was already in set!";
        }
    }
    
    void unregisterRouteHandler(ofxBaseWebSocketRouteHandler* handler) {
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
        vector<string> availableSubprotocols;
        map<string,ofxWebsocketEvents>::iterator eventIter = subprotocolEvents.begin();
        while(eventIter != subprotocolEvents.end()) {
            availableSubprotocols.push_back((*eventIter).first);
            ++eventIter;
        }
        return availableSubprotocols;
    }
        
    bool selectSubprotocol(const vector<string>& proposedSubprotocols, string& selectedSubprotocol) {
        selectedSubprotocol.clear();

        if(proposedSubprotocols.empty()) {
            return false;
        } else {
            ofScopedLock lock(mutex);
            vector<string>::const_iterator iter = proposedSubprotocols.begin();
            while(iter != proposedSubprotocols.end()) {
                map<string,ofxWebsocketEvents>::iterator eventIter = subprotocolEvents.find(*iter);
                if(eventIter != subprotocolEvents.end()) {
                    selectedSubprotocol = (*eventIter).first;
                    return true;
                }
                ++iter;
            }
            return false;
        }
    }
    
    ofxWebsocketEvents events;

protected:
    ofMutex mutex; // locks the handlers set
    set<ofxBaseWebSocketRouteHandler*> handlers;
    
    map<string,ofxWebsocketEvents> subprotocolEvents;

};

