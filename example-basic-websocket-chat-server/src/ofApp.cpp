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
    
    maxHistory = 100;

    BasicWebSocketServerSettings settings;
//    settings.setPort(80);

    server = BasicWebSocketServer::makeShared(settings);

    server->getWebSocketRoute()->registerWebSocketEvents(this);
    server->start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}

void ofApp::update()
{
}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    ofSetBackgroundColor(0);

    ofSetColor(255);
    
    std::size_t numConnected = server->getWebSocketRoute()->getNumWebSocketConnections();

    ofSetColor(0);

    ofDrawBitmapStringHighlight("Num Connected: " + ofToString(numConnected),ofPoint(0,12));


    ofScopedLock lock(mutex); // lock the history

    ofPushMatrix();

        std::vector<Message>::reverse_iterator iter = history.rbegin();

        int y = 30;


        while(iter != history.rend())
        {
            const Message& message = *iter; // create a const reference

            y+= 20;

            ofDrawBitmapStringHighlight(message.toString(), 10, y);

            ++iter;
        }

    ofPopMatrix();

}

//------------------------------------------------------------------------------
void ofApp::onWebSocketOpenEvent(WebSocketEventArgs& evt)
{
    cout << "Connection opened from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketCloseEvent(WebSocketEventArgs& evt)
{
    cout << "Connection closed from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
    std::string clientAddress = evt.getConnectionRef().getClientAddress().toString();
    activeUsers.erase(clientAddress);
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketFrameReceivedEvent(WebSocketFrameEventArgs& evt)
{
    std::string clientAddress = evt.getConnectionRef().getClientAddress().toString();
    std::string text = evt.getFrameRef().getText();


//    cout << "Frame From: " << evt.getConnectionRef().getClientAddress().toString() << endl;
//    cout << "\t=" << text << endl;

    std::map<std::string,User>::iterator iter = activeUsers.find(clientAddress);

    if(iter == activeUsers.end())
    {
        // generate a random color
        std::stringstream ss;

        // create a css compatible color string
        ss << "rgb(";
        ss << (int)ofRandom(255) << ",";
        ss << (int)ofRandom(255) << ",";
        ss << (int)ofRandom(255) << ")";

        // add the user to our local map
        User user(text,ss.str());

        activeUsers[clientAddress] = user;

        // send a message back to the web client with the client's color
        Json::Value json;

        json["type"] = "color";
        json["data"] = user.getColor();

        // send the message back to the client
        evt.getConnectionRef().sendFrame(json.toStyledString());

        // erase json, it was already sent and we don't need to create another
        json.clear();

//        json["type"] = "history"; // set message type
//
//        ofScopedLock lock(mutex); // lock because we are accessing the history
//
//        std::vector<Message>::iterator iter = history.begin();
//
////        cout << "there are " << history.size() << " items in the history vector." << endl;
//
//        while(iter != history.end())
//        {
//            const Message& message = *iter; // create a const reference
//
//            Json::Value messageJSON;
//
//            messageJSON["time"]   = message.getTime();
//            messageJSON["text"]   = message.getMessage();
//            messageJSON["author"] = message.getUser().getName();
//            messageJSON["color"]  = message.getUser().getColor();
//
//            json["data"].append(messageJSON);
//
////            cout << "HISTORY MESSAGE: " << message.toString() << endl;
//
//            ++iter;
//        }

        evt.getConnectionRef().sendFrame(json.toStyledString());

    }
    else
    {
        User user = (*iter).second;
        Message message(ofGetSystemTime(),user,text);

//        cout << clientAddress << " THIS MESSAGE ADDING>" << message.toString() << endl;


        Json::Value json;
        json["type"] = "message";

        Json::Value messageJSON;

        messageJSON["time"]   = message.getTime();
        messageJSON["text"]   = message.getMessage();
        messageJSON["author"] = message.getUser().getName();
        messageJSON["color"]  = message.getUser().getColor();

        json["data"] = messageJSON;


        // send the message out to all other clients
        server->getWebSocketRoute()->broadcast(json.toStyledString());


        ofScopedLock lock(mutex); // lock the history
        // add it to our history
        history.push_back(message);
        while(history.size() > maxHistory)
        {
            history.erase(history.begin()); // erase the oldest element
        }

    }
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketFrameSentEvent(WebSocketFrameEventArgs& evt)
{
    // frame was sent to clients
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketErrorEvent(WebSocketEventArgs& evt)
{
    cout << "Error from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

