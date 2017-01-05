//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Testing redirects and https.
    std::string url = "https://httpbin.org/redirect/20";

    // Testing redirects and https.
    ofxHTTP::HTTPClient client;

    auto response = client.get(url);

    if (response->isSuccess())
    {
        const auto& redirects = response->context().getRedirects();

        std::cout << "Redirects followed: " << std::endl << std::endl;
        for (std::size_t i = 0; i < redirects.size(); ++i)
        {
            std::cout << "\t" << i << ". " << redirects[i].toString() << std::endl;
        }

        std::cout << std::endl << "Data:" << std::endl << std::endl;
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
