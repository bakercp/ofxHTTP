//
// Copyright (c) 2012 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxHTTP.h"


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    ofxHTTP::SimpleIPVideoServer server;

    ofVideoPlayer player;

};
