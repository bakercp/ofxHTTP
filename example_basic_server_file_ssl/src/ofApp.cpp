//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Register for SSL Context events.
    ofSSLManager::registerServerEvents(this);

    ofSetFrameRate(30);

    ofxHTTP::SimpleFileServerSettings settings;
    settings.setPort(4433);

    // NOTE: the included private key / certificate
    // should not be used for production purposes.
    settings.setUseSSL(true);

    server.setup(settings);

    server.start();

#if !defined(TARGET_LINUX_ARM)
    // Launch a browser with the address of the server.
    ofLaunchBrowser(server.url());
#endif

}


void ofApp::draw()
{
    ofBackground(255);
    ofDrawBitmapStringHighlight("See " + server.url(), 10, 16);
}


void ofApp::onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogNotice("ofApp::onSSLServerVerificationError") << std::endl << ofToString(args);

    // If you want to proceed, you must allow the user to inspect the
    // certificate and set `args.setIgnoreError(true);` if they want to continue.

    args.setIgnoreError(true);
}


void ofApp::onSSLPrivateKeyPassphraseRequired(std::string& passphrase)
{
    // If you want to proceed, you must allow the user to input the assign the
    // private key's passphrase to the `passphrase` argument.  For example:

    passphrase = ofSystemTextBoxDialog("Enter the Private Key Passphrase", "");
}
