#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);

    server = ofxHTTPServerBasic::instance();
    uploadRoute = ofxHTTPServerUploadRoute::Instance();
    server->addRoute(uploadRoute);
    server->start();
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(255);
    ofSetColor(255);
}


