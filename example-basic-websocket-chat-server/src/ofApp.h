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


#pragma once


#include "ofMain.h"
#include "ofxJSONElement.h"
#include "BasicWebSocketServer.h"
#include "WebSocketEvents.h"
#include "WebSocketRoute.h"



using ofx::HTTP::BasicWebSocketServer;
using ofx::HTTP::BasicWebSocketServerSettings;
using ofx::HTTP::WebSocketEventArgs;
using ofx::HTTP::WebSocketFrameEventArgs;
using ofx::HTTP::WebSocketRoute;


class User
{
public:
    User(): _name(""), _color("")
    {
    }
    
    User(const std::string& name, const std::string& color):
        _name(name),
        _color(color)
    {
    }

    std::string getName() const
    {
        return _name;
    }

    std::string getColor() const
    {
        return _color;
    }

private:
    std::string _name;
    std::string _color;

};


class Message
{
public:
    Message(unsigned long long int time,
            const User& user,
            const std::string& message):
        _time(time),
        _user(user),
        _message(message)
    {
    }

    unsigned long long int getTime() const
    {
        return _time;
    }

    User getUser() const
    {
        return _user;
    }

    std::string getMessage() const
    {
        return _message;
    }

    std::string toString() const
    {
        return _user.getName() + " : " + _message;
    }

private:
    unsigned long long _time;
    User _user;
    std::string _message;
};


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void onWebSocketOpenEvent(WebSocketEventArgs& evt);
    void onWebSocketCloseEvent(WebSocketEventArgs& evt);
    void onWebSocketFrameReceivedEvent(WebSocketFrameEventArgs& evt);
    void onWebSocketFrameSentEvent(WebSocketFrameEventArgs& evt);
    void onWebSocketErrorEvent(WebSocketEventArgs& evt);

    BasicWebSocketServer::SharedPtr server;

    std::map<std::string,User> activeUsers;

    std::vector<Message> history;
    std::size_t maxHistory;


    mutable ofMutex mutex;

};
