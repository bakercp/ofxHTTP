// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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

    server = BasicWebSocketServer::makeShared();

    server->getWebSocketRoute()->registerWebSocketEvents(this);
    server->start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}

void ofApp::update()
{
}

void ofApp::draw()
{
}

void ofApp::onWebSocketOpenEvent(WebSocketEventArgs& evt)
{
    std::string clientAddress = evt.getConnectionRef().getClientAddress().toString();
    ofLogNotice("ofApp::onWebSocketOpenEvent") << "Connection opened from: " << clientAddress << endl;
}

void ofApp::onWebSocketCloseEvent(WebSocketEventArgs& evt)
{
    std::string clientAddress = evt.getConnectionRef().getClientAddress().toString();

    ofLogNotice("ofApp::onWebSocketCloseEvent") << "Connection closed from: " << clientAddress << endl;

    activeClients.erase(clientAddress);
}

void ofApp::onWebSocketFrameReceivedEvent(WebSocketFrameEventArgs& evt)
{
    std::string clientAddress = evt.getConnectionRef().getClientAddress().toString();

    ofLogNotice("ofApp::onWebSocketFrameReceivedEvent") << "Connection closed from: " << clientAddress << endl;

    // get the message
    std::string rawStringMessage = evt.getFrameRef().getText(); // === e.g. "20,44"

    // parse the message!
    std:vector<std::string> tokens = ofSplitString(rawStringMessage,",");

    if(tokens.size() == 2)
    {
        int x = ofToInt(tokens[0]);
        int y = ofToInt(tokens[1]);

        ofVec2f point(x,y); // make the point

        // look up the map entry that corresponds to our client address
        std::map<std::string,ofPolyline>::iterator iter = activeClients.find(clientAddress);

        if(iter != activeClients.end()) // this means that the search found something
        {
            activeClients[clientAddress].addVertex(point);
            // here is where we might want to remove some points too ...
        }
        else // the search did not find anything
        {
            // create a new polyline for this client
            ofPolyline polyline;
            polyline.addVertex(point);
            activeClients[clientAddress] = polyline;
        }
    }
    else
    {
        ofLogError("ofApp::onWebSocketFrameReceivedEvent") << "The incoming message was not correctly formatted: " << rawStringMessage;
    }
}

void ofApp::onWebSocketFrameSentEvent(WebSocketFrameEventArgs& evt)
{
    // frame was sent to clients
}

void ofApp::onWebSocketErrorEvent(WebSocketEventArgs& evt)
{
    cout << "Error from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

