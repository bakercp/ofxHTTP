// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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
