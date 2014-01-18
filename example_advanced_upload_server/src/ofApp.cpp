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

    HTTP::BasicPostServerSettings settings;
    settings.setPort(8998);
    settings.setMaximumFileUploadSize(104857600); // 100 MB

    server = HTTP::BasicPostServer::makeShared(settings);

    server->getPostRoute()->registerPostEvents(this);

    server->start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}


void ofApp::draw()
{
    ofBackground(255);
    ofDrawBitmapStringHighlight("See " + server->getURL(), 10, 16);

    ofDrawBitmapStringHighlight("Upload Progress:", 10, 45);

    int y = 70;

    std::map<std::string,float>::const_iterator iter = uploadProgress.begin();

    while(iter != uploadProgress.end())
    {
        std::string fileName = (*iter).first;
        float progress = (*iter).second;

        ofSetColor(0);
        ofNoFill();
        ofRect(8,y+10,384,5);
        ofFill();

        if(progress >= 1)
        {
            ofSetColor(0,255,0);
        }
        else
        {
            ofSetColor(255,255,0);
        }

        ofRect(8,y+10,progress*384,5);

        ofDrawBitmapStringHighlight(ofFilePath::getFileName(fileName),10,y);
        
        y += 35;
        ++iter;
    }

}


bool ofApp::onHTTPFormEvent(HTTP::HTTPFormEventArgs& args)
{
    ofLogNotice("ofApp::onHTTPFormEvent") << "";
    HTTP::Utils::dumpNameValueCollection(args.form, ofGetLogLevel());
    return true;
}

bool ofApp::onHTTPRawFormEvent(HTTP::HTTPRawFormEventArgs& args)
{
    ofLogNotice("ofApp::onHTTPRawFormEvent") << "";
    HTTP::Utils::dumpNameValueCollection(HTTP::Utils::splitTextPlainPost(args.form),
                                         ofGetLogLevel());
    return true;
}


bool ofApp::onHTTPUploadStartedEvent(HTTP::HTTPUploadEventArgs& args)
{
    uploadProgress[args.getOriginalFilename()] = 0;
    return true;
}

bool ofApp::onHTTPUploadProgressEvent(HTTP::HTTPUploadEventArgs& args)
{
    uploadProgress[args.getOriginalFilename()] = args.getNumBytesTransferred() / (float) args.getRequestContentLength();

    return true;
}

bool ofApp::onHTTPUploadFinishedEvent(HTTP::HTTPUploadEventArgs& args)
{
    uploadProgress[args.getOriginalFilename()] = 1;
    return true;
}
