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
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(30);

    BasicPostServerSettings settings;

    server = BasicPostServer::makeShared(settings);

    server->getPostRoute()->registerPostEvents(this);

    server->start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}


void ofApp::draw()
{
    ofBackground(255);
    ofDrawBitmapStringHighlight("See " + server->getURL(), 10, 16);
    ofDrawBitmapStringHighlight("See the Console", 10, 45);
    int y = 70;

}


void ofApp::onHTTPPostEvent(PostEventArgs& args)
{
    ofLogNotice("ofApp::onHTTPPostEvent") << "Data: " << args.data.getText();
}


void ofApp::onHTTPFormEvent(PostFormEventArgs& args)
{
    ofLogNotice("ofApp::onHTTPFormEvent") << "";
    Utils::dumpNameValueCollection(args.form, ofGetLogLevel());
}


void ofApp::onHTTPUploadEvent(PostUploadEventArgs& args)
{
    std::string stateString = "";

    switch (args.getState())
    {
        case PostUploadEventArgs::UPLOAD_STARTING:
            stateString = "STARTING";
            break;
        case PostUploadEventArgs::UPLOAD_PROGRESS:
            stateString = "PROGRESS";
            break;
        case PostUploadEventArgs::UPLOAD_FINISHED:
            stateString = "FINISHED";
            break;
    }

    ofLogNotice("ofApp::onHTTPUploadEvent") << "";
    cout << "         state: " << stateString << endl;
    cout << " formFieldName: " << args.getFormFieldName() << endl;
    cout << "orig. filename: " << args.getOriginalFilename() << endl;
    cout << "      filename: " << args.getFilename() << endl;
    cout << "      fileType: " << args.getFileType().toString() << endl;
    cout << "# bytes xfer'd: " << args.getNumBytesTransferred() << endl;
}
