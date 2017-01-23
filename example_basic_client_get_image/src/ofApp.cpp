//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Testing redirects and https.
    std::string url = "https://httpbin.org/image/png";

    // Testing redirects and https.
    ofxHTTP::HTTPClient client;

    auto response = client.get(url);

    if (response->isSuccess())
    {
        ofLoadImage(texture, response->getBuffer());
    }
    else
    {
        std::cout << response->error() << std::endl;
    }
}


void ofApp::draw()
{
    texture.draw(0, 0);
}
