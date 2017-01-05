//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Testing redirects and https.
    std::string url = "https://httpbin.org/get";

    // Testing redirects and https.
    ofxHTTP::Client client;

    auto response = client.get(url);

    if (response->isSuccess())
    {
        std::cout << response->getBuffer() << std::endl;
    }
    else
    {
        std::cout << response->error() << std::endl;
    }
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
}
