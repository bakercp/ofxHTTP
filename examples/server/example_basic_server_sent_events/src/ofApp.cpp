//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_NOTICE);

    ofSetFrameRate(30);

    ofxHTTP::SimpleSSEServerSettings settings;

    // Many other settings are available.
    settings.setPort(7890);

    // Apply the settings.
    server.setup(settings);

    // Register listeners.
    server.sseRoute().registerEventListeners(this);

    // Start the server.
    server.start();

#if !defined(TARGET_LINUX_ARM)
    // Launch a browser with the address of the server.
    ofLaunchBrowser(server.url());
#endif

}


void ofApp::draw()
{
    ofBackground(255);
    ofDrawBitmapStringHighlight("See " + server.url(), 10, 16);
    ofDrawBitmapStringHighlight("See the Console", 10, 42);
}


void ofApp::keyPressed(int key)
{
    if (key == 'c')
    {
        ofJson json;

        json["cached"] = true;
        json["date"] = ofGetTimestampString();

        ofxHTTP::SSEFrame frame(json.dump());
        server.sseRoute().send(frame, true);
    }
    else if (key == 'd')
    {
        ofxHTTP::SSEFrame frame("not");
        server.sseRoute().send(frame);
    }
}


void ofApp::onSSEOpenEvent(ofxHTTP::SSEOpenEventArgs& evt)
{
    ofLogNotice("ofApp::onSSEOpenEvent") << "Connection opened from: " << evt.connection().clientAddress().toString();
}


void ofApp::onSSECloseEvent(ofxHTTP::SSECloseEventArgs& evt)
{
    ofLogNotice("ofApp::onSSECloseEvent") << "Connection closed: " << evt.code() << " : " << evt.reason();
}


void ofApp::onSSEFrameSentEvent(ofxHTTP::SSEFrameEventArgs& evt)
{
    ofLogNotice("ofApp::onSSEFrameSentEvent") << "Frame sent: " << evt.frame().data();
}
