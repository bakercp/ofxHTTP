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

#include "ofBaseApp.h"
#include "ofGraphics.h"
#include "ofImage.h"
#include "ofAppRunner.h"

#include "ofxWebSocketServerBasic.h"

//#include "ofxHTTPServerBasic.h"
//#include "ofxWebSocketRoute.h"
//#include "ofxBaseWebSocketEventListener.h"

class testApp : public ofBaseApp {//, public ofxBaseWebSocketEventListener {

public:
    void setup();
    void draw();
    void exit();
    
//    void onOpenEvent(/*const void* _connection, */ofxWebSocketEventArgs& _evtArgs) { }

   
    
    
    void onOpenEvent(ofxWebSocketEventArgs& _evtArgs) {
        cout << "got open event!!!!" << endl;
    }
    
    void onCloseEvent(const void* _connection, ofxWebSocketEventArgs& _evtArgs) {
        cout << "got close event! " << _evtArgs.getError() << endl;
    }

    void onFrameReceivedEvent(const void* _connection, ofxWebSocketFrameEventArgs& _evtArgs) {
        cout << "got onFrameReceivedEvent event! " << _evtArgs.getError() << endl;
    }
    
    void onFrameSentEvent(const void* _connection, ofxWebSocketFrameEventArgs& _evtArgs) {
        cout << "got onFrameSentEvent event! " << _evtArgs.getError() << endl;
    }
    
    void onErrorEvent(const void* _connection, ofxWebSocketEventArgs& _evtArgs) {
        cout << "got error event! " << _evtArgs.getError() << endl;
    }

    
    ofImage logo;
//    ofxWebSocketRoute::Ptr wsRoute;
    
    ofxWebSocketServerBasic::Ptr server;

};
