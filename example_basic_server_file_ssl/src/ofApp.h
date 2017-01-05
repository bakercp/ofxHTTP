//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxHTTP.h"


class ofApp: public ofBaseApp
{
public:
    void setup();
    void draw();

    void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLPrivateKeyPassphraseRequired(std::string& passphrase);

    ofxHTTP::SimpleFileServer server;

};
