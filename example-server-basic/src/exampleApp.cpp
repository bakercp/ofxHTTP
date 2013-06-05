/*==============================================================================

 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 =============================================================================*/


#include "exampleApp.h"


//------------------------------------------------------------------------------
void exampleApp::setup() {
    ofSetFrameRate(30);

    // Get an instance of a basic server.
    // An instance is a shared pointer, which will ensure that the server
    // will be shut down cleanly upon exit (i.e. when the shared pointer
    // goes out of context.
    server = BasicServer::Instance();

    // Start the server.
    server->start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}

//------------------------------------------------------------------------------
void exampleApp::draw() {
    ofBackground(255);
    ofDrawBitmapStringHighlight("See " + server->getURL(), 10, 16);
}

