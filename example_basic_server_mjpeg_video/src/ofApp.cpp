//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(30);

    // Set up our video to broadcast.
    player.load("fingers.mp4");
    player.play();
    player.setLoopState(OF_LOOP_NORMAL);

    ofxHTTP::SimpleIPVideoServerSettings settings;

    // Many other settings are available.
    settings.setPort(7890);

    // The default maximum number of client connections is 5.
    settings.ipVideoRouteSettings.setMaxClientConnections(1);

    // Apply the settings.
    server.setup(settings);

    // Start the server.
    server.start();

#if !defined(TARGET_LINUX_ARM)
    // Launch a browser with the address of the server.
    ofLaunchBrowser(server.url());
#endif

}


void ofApp::update()
{
    // Update the video player.
    player.update();

    // If the frame is new, then send it to the server to be broadcast.
    if(player.isFrameNew())
    {
        // This can be any kind of pixels.
        server.send(player.getPixels());
    }
}


void ofApp::draw()
{
    // Draw the video on the server screen.
    player.draw(0,0);

    // Display the number of connected clients for reference.
    std::stringstream ss;

    ss << "Num clients connected: ";
    ss << server.numConnections();

    ofDrawBitmapStringHighlight(ss.str(), 20, 20);
}
