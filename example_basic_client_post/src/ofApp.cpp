//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Testing redirects and https.
    std::string uri = "http://httpbin.org/post";

    std::multimap<std::string, std::string> formFields =
    {
        { "test", "123" },
        { "test", "123" },
        { "123", "test" }
    };

    std::vector<ofxHTTP::FormPart> formFiles = {
        ofxHTTP::FormPart(ofxHTTP::FormPart::Type::FILE,
                          "one pixel",
                          "pixel.gif")
    };

    ofxHTTP::HTTPClient client;

    auto response = client.form(uri, formFields, formFiles);

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
