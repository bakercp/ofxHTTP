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
    ofSSLManager::registerAllEvents(this);

    client.registerClientEvents(this);
    client.registerClientFilterEvents(this);
    client.registerClientProgressEvents(this);

//    Poco::Net::NameValueCollection nvc;
//    nvc.add("q", "puppies");
//    nvc.add("safe", "off");
//    nvc.add("tbm", "isch");
//    nvc.add("source", "lnms");
//
//    HTTP::Client::GetRequest request("https://www.google.com/search", nvc);

//    HTTP::Client::GetRequest request("http://127.0.0.1:5000/headers");
//    HTTP::Client::BaseResponse response;
//
    asynClient.get("http://127.0.0.1:5000/headers");


//    client.submit(request, response, context);

}

void ofApp::exit()
{
    ofSSLManager::unregisterAllEvents(this);
}


void ofApp::update()
{
}


void ofApp::draw()
{
}


void ofApp::onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogVerbose("ofApp::onServerVerificationError") << args.errorMessage();

    // if you want to proceed, you should allow your user to inspect
    // the certificate and set:
//     args.setIgnoreError(true);
    // if they approve
}


void ofApp::onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogVerbose("ofApp::onClientVerificationError") << args.errorMessage();

    std::stringstream ss;

    ss << "Error: " << args.errorMessage() << " #" << args.errorNumber() << " depth: " << args.errorDepth() << std::endl;

    ss << "Certificate: " << std::endl;

    ss << "Issued By: " << args.certificate().issuerName() << std::endl;
    ss << "Subject Name: " << args.certificate().issuerName() << std::endl;
    ss << "Common Name: " << args.certificate().issuerName() << std::endl;

    Poco::DateTime ldtValidFrom = args.certificate().validFrom();
    Poco::DateTime ldtExpiresOn = args.certificate().expiresOn();

    ss << "Valid From: " << Poco::DateTimeFormatter::format(ldtValidFrom, "%dd %H:%M:%S.%i") << std::endl;
    ss << "Expires On: " << Poco::DateTimeFormatter::format(ldtExpiresOn, "%dd %H:%M:%S.%i") << std::endl;

    ofLogVerbose("ofApp::onServerVerificationError") << ss.str();

    // if you want to proceed, you should allow your user to inspect
    // the certificate and set:
//     args.setIgnoreError(true);
    // if they approve
}


void ofApp::onSSLPrivateKeyPassphraseRequired(std::string& args)
{
    ofLogVerbose("ofApp::onPrivateKeyPassphraseRequired") << args;
    
    // if you want to proceed, you should allow your user set the
    // the certificate and set:
    //     args.setIgnoreError(true);
    // if they approve
}


bool ofApp::onHTTPClientResponseEvent(HTTP::Client::ClientResponseEventArgs& args)
{
    std::cout << "ofApp::onHTTPClientResponseEvent" << std::endl;
    Poco::StreamCopier::copyStream(args.getResponseStream(), std::cout);
    std::cout << std::endl;
    return true;
}


bool ofApp::onHTTPClientErrorEvent(HTTP::Client::ClientErrorEventArgs& args)
{
    std::cout << "ofApp::onHTTPClientErrorEvent:" << std::endl;
    std::cout << args.getRequest().getRequestId().toString() << endl;
    std::cout << args.getException().displayText() << std::endl;
    return true;
}


bool ofApp::onHTTPClientRequestProgress(HTTP::Client::ClientRequestProgressArgs& args)
{
    return true;
}


bool ofApp::onHTTPClientResponseProgress(HTTP::Client::ClientResponseProgressArgs& args)
{
    return true;
}


bool ofApp::onHTTPClientRequestFilterEvent(HTTP::Client::MutableClientRequestArgs& args)
{
    return true;
}


bool ofApp::onHTTPClientResponseFilterEvent(HTTP::Client::MutableClientResponseArgs& args)
{
    return true;
}
