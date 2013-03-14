#include "testApp.h"

//--------------------------------------------------------------
void testApp::exit() {
    server->stop();
    delete server;
}

//--------------------------------------------------------------
void testApp::setup() {
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    server = new ofxHTTPServerBasic();
    server->loadSettings();
    
    uploadRoute = ofxHTTPServerUploadRoute::Instance();
    
    server->addRoute(uploadRoute);

    server->start();
    
    ofLaunchBrowser(server->getURL());
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(255);
    ofSetColor(255);
}


