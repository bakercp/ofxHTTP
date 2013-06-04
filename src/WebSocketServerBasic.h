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


#include "ServerBasic.h"
#include "WebSocketRoute.h"


namespace ofx {
namespace HTTP {



//------------------------------------------------------------------------------
class ofxWebSocketServerBasicSettings
: public ServerBasic::Settings
, public ofxWebSocketRoute::Settings
{

};

//------------------------------------------------------------------------------
class ofxWebSocketServerBasic : public ServerBasic {
public:
    typedef ofxWebSocketServerBasicSettings Settings;
    typedef ofPtr<ofxWebSocketServerBasic> Ptr;

    ofxWebSocketServerBasic(const Settings& _settings = Settings()) :
    ServerBasic(_settings),
    settings(_settings)
    {
        websocket = ofxWebSocketRoute::Instance(_settings);
        addRoute(websocket);
    }
    
    virtual ~ofxWebSocketServerBasic() { }
    
    void broadcast(const ofxWebSocketFrame& frame) {
        if(websocket == NULL) {
            ofLogError("ofxWebSocketServerBasic::broadcast") << "call loadSettings first.";
            return;
        }
        
        websocket->broadcast(frame);
    }
    
    
    void broadcast(ofPixelsRef pixels) {
        if(websocket == NULL) {
            ofLogError("ofxWebSocketServerBasic::broadcast") << "call loadSettings first.";
            return;
        }
        
        websocket->broadcast(pixels);
    }
    
    bool sendFrame(ofxWebSocketRouteHandler* handler, const ofxWebSocketFrame& frame) {
        if(websocket == NULL) {
            ofLogError("ofxWebSocketServerBasic::sendFrame") << "call loadSettings first.";
            return false;
        }
        
        return websocket->sendFrame(handler,frame);
    }
    
    void disconnect(ofxWebSocketRouteHandler* handler) {
        if(websocket == NULL) {
            ofLogError("ofxWebSocketServerBasic::disconnect") << "call loadSettings first.";
            return;
        }
        websocket->disconnect(handler);
    }
    
    void disconnectAll() {
        if(websocket == NULL) {
            ofLogError("ofxWebSocketServerBasic::disconnectAll") << "call loadSettings first.";
            return;
        }
        websocket->disconnectAll();
    }

    size_t getNumClientsConnected() {
        if(websocket == NULL) {
            ofLogError("ofxWebSocketServerBasic::getNumClientsConnected") << "call loadSettings first.";
            return -1;
        }
        return websocket->getNumClientsConnected();
    }


    template<class ListenerClass>
    void registerWebSocketEvents(ListenerClass * listener) {
        if(websocket == NULL) {
            ofLogError("ofxWebSocketServerBasic::registerWebSocketEvents") << "call loadSettings first.";
            return;
        }
        websocket->registerWebSocketEvents(listener);
    }

//    template<class ListenerClass>
//    void registerWebSocketEvents(const ListenerClass& listener) {
//        websocket->registerWebSocketEvents(&listener);
//    }

    
    template<class ListenerClass>
    void unregisterWebSocketEvents(ListenerClass * listener) {
        if(websocket == NULL) {
            ofLogError("ofxWebSocketServerBasic::unregisterWebSocketEvents") << "call loadSettings first.";
            return;
        }
        websocket->unregisterWebSocketEvents(listener);
    }

//    template<class ListenerClass>
//    void unregisterWebSocketEvents(const ListenerClass& listener) {
//        websocket->unregisterWebSocketEvents(&listener);
//    }

    
    ofPtr<ofxWebSocketRoute>         websocket;

    static Ptr instance(const Settings& _settings = Settings()) {
        return Ptr(new ofxWebSocketServerBasic(_settings));
    }
protected:
    Settings settings;

};


} }
