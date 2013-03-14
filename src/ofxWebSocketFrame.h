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
 
 ==============================================================================*/

#pragma once

#include "Poco/Net/WebSocket.h"

#include "ofFileUtils.h"

using Poco::Net::WebSocket;

class ofxWebSocketFrame : public ofBuffer {
public:
    ofxWebSocketFrame(const ofBuffer& _buffer, int _flags = WebSocket::FRAME_TEXT);
    ofxWebSocketFrame(const string& _text, int _flags = WebSocket::FRAME_TEXT);
    ofxWebSocketFrame(const char* _buffer, unsigned int _size, int _flags = WebSocket::FRAME_TEXT);
    ofxWebSocketFrame(const unsigned char* _buffer, unsigned int _size, int _flags = WebSocket::FRAME_TEXT);
    
    virtual ~ofxWebSocketFrame();
    
    int getFlags() const;
    
    bool isContinuation() const;
    bool isText() const;
    bool isBinary() const;
    bool isClose() const;
    bool isPing() const;
    bool isPong() const;
    
    bool isFinal() const;
    bool isRSV1()  const;
    bool isRSV2()  const;
    bool isRSV3()  const;
    
    string toString() const;

protected:
    int      flags;
    
};

