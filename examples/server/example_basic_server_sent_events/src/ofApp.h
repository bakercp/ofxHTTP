//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxHTTP.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    void keyPressed(int key) override;

    void onSSEOpenEvent(ofxHTTP::SSEOpenEventArgs& evt);
    void onSSECloseEvent(ofxHTTP::SSECloseEventArgs& evt);
    void onSSEFrameSentEvent(ofxHTTP::SSEFrameEventArgs& evt);

    ofxHTTP::SimpleSSEServer server;

};
