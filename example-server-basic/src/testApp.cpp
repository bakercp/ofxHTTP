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

    logo.loadImage("DocumentRoot/img/openFrameworks.png");
    
    server = new ofxHTTPServerBasic(8080,"DocumentRoot");
    server->start();

    ofLaunchBrowser(server->getURL());
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(255);
    ofSetColor(255);
    logo.draw(0,0,114,60);
}


