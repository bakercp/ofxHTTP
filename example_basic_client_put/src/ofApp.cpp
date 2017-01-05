//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    std::string uri = "https://httpbin.org/put";

    auto request = std::make_unique<ofxHTTP::PutRequest>(uri);

    request->set("My-special-request-header", "My-special-request-header-value");

    request->setPutBuffer(ofBuffer("My body buffer."));


    // Create a client.
    ofxHTTP::HTTPClient client;

    // When requesting with a custom request, we must transfer ownership of the
    // request. The original request will be available for reference in the
    // completed PackagedResponse, in addition to the context and the buffered
    // response data..
    auto response = client.executeBuffered<ofxHTTP::PutRequest>(std::move(request));

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
