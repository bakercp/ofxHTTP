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
#include "ofxHTTP.h"
#include "ofxJSONElement.h"


using namespace ofx;


class ofApp: public ofBaseApp
{
public:
    void setup();
    void exit();
    void update();
    void draw();

    void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLPrivateKeyPassphraseRequired(std::string& args);

    bool onHTTPClientResponseEvent(HTTP::Client::ClientResponseEventArgs& args);
    bool onHTTPClientErrorEvent(HTTP::Client::ClientErrorEventArgs& args);

    bool onHTTPClientRequestProgress(HTTP::Client::ClientRequestProgressArgs& args);
    bool onHTTPClientResponseProgress(HTTP::Client::ClientResponseProgressArgs& args);

    bool onHTTPClientRequestFilterEvent(HTTP::Client::MutableClientRequestArgs& args);
    bool onHTTPClientResponseFilterEvent(HTTP::Client::MutableClientResponseArgs& args);

    HTTP::Client::Context context;
    HTTP::Client::DefaultClient client;

};