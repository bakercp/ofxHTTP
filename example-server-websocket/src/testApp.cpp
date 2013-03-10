#include "testApp.h"

//--------------------------------------------------------------
void testApp::exit() {
    server.unregisterWebSocketEvents(this);
}

//--------------------------------------------------------------
void testApp::setup() {
    ofSetFrameRate(1);
    ofSetLogLevel(OF_LOG_NOTICE);

    server.loadSettings();
    server.registerWebSocketEvents(this);
    server.start();
    
    
    
//    ofLaunchBrowser(server.getURL());
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(255);
    ofSetColor(255);
//    logo.draw(0,0,114,60);
    
    server.broadcast(ofxWebSocketFrame(ofBuffer("test " + ofToString(ofRandom(1)))));

    size_t numConnected = server.getNumClientsConnected();
    
    ofSetColor(0);
    ofDrawBitmapString("Num Connected: " + ofToString(numConnected),ofPoint(40,40));
    
}


