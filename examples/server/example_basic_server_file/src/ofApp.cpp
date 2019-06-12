//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(30);

    ofxHTTP::SimpleFileServerSettings settings;

    // Many other settings are available.
    settings.setPort(7890);

    // Create a whitelist.
    // ofx::Net::IPAddressRange::List whitelist;
    // whitelist.push_back(ofx::Net::IPAddressRange("127.0.0.1/32"));

    // Set the whitelist.
    // settings.setWhitelist(whitelist);

    // Apply the settings.
    server.setup(settings);

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
}
