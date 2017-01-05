//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    std::string uri = "http://httpbin.org/post";

    ofJson json = {
        "hello", "jello"
    };

    ofxHTTP::HTTPClient client;

    auto response = client.post(uri, json);

    if (response->isSuccess())
    {
        std::cout << response->json().dump(4) << std::endl;
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
