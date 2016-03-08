// =============================================================================
//
// Copyright (c) 2012-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(30);

    // Set up our video to broadcast.
    player.load("fingers.mp4");
    player.play();
    player.setLoopState(OF_LOOP_NORMAL);

    ofx::HTTP::SimpleIPVideoServerSettings settings;

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
    ofLaunchBrowser(server.getURL());
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
    ss << server.getNumConnections();

    ofDrawBitmapStringHighlight(ss.str(), 20, 20);
}
