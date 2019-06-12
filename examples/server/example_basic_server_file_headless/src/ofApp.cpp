//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"

void ofApp::setup()
{
    ofxHTTP::SimpleFileServerSettings settings;

    // Many other settings are available.
    settings.setPort(7890);

    // Apply the settings.
    server.setup(settings);

    // Start the server.
    server.start();

}
