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

    bgColor = ofColor::white;

    HTTP::BasicWebSocketServerSettings settings;

    server = HTTP::BasicWebSocketServer::makeShared(settings);

    server->getWebSocketRoute()->registerWebSocketEvents(this);
    server->start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}


void ofApp::update()
{
    Json::Value setCurrentFrameJSON;
    setCurrentFrameJSON["method"] = "setCurrentFrame";
    Json::Value setCurrentFrameParamsJSON;
    setCurrentFrameParamsJSON["frameNum"] = ofGetFrameNum();
    setCurrentFrameJSON["data"] = setCurrentFrameParamsJSON;

    server->getWebSocketRoute()->broadcast(setCurrentFrameJSON.toStyledString());
}


void ofApp::draw()
{
    ofBackground(bgColor);
    ofSetColor(255);
    
    std::size_t numConnected = server->getWebSocketRoute()->getNumWebSocketConnections();

    ofSetColor(0);

    ofDrawBitmapStringHighlight("Num Connected: " + ofToString(numConnected),ofPoint(40,40));

}


void ofApp::onWebSocketOpenEvent(HTTP::WebSocketEventArgs& evt)
{
    cout << "Connection opened from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}


void ofApp::onWebSocketCloseEvent(HTTP::WebSocketEventArgs& evt)
{
    cout << "Connection closed from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}


void ofApp::onWebSocketFrameReceivedEvent(HTTP::WebSocketFrameEventArgs& evt)
{
    cout << "Frame from: " << evt.getConnectionRef().getClientAddress().toString() << endl;

    ofxJSONElement json;

    if(json.parse(evt.getFrameRef().getText()))
    {
        std::cout << json.toStyledString() << std::endl;

        if(json.isMember("command") && json["command"] == "SET_BACKGROUND_COLOR")
        {
            if(json["data"] == "white")
            {
                bgColor = ofColor::white;
            }
            else if(json["data"] == "black")
            {
                bgColor = ofColor::black;
            }
            else
            {
                cout << "Unknown color: " << json["data"].toStyledString() << endl;
            }
        }
    }
    else
    {
        ofLogError("ofApp::onWebSocketFrameReceivedEvent") << "Unable to parse JSON: "  << evt.getFrameRef().getText();
    }
}


void ofApp::onWebSocketFrameSentEvent(HTTP::WebSocketFrameEventArgs& evt)
{
    // frame was sent to clients
}


void ofApp::onWebSocketErrorEvent(HTTP::WebSocketEventArgs& evt)
{
    cout << "Error from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

