// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
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
    // Testing redirects and https.
    std::string url = "http://httpbin.org/put";

    ofx::HTTP::DefaultClient client;
    ofx::HTTP::Context context;

    ofx::HTTP::BaseResponse response;

    ofx::HTTP::PutRequest putRequest(url, Poco::Net::HTTPMessage::HTTP_1_1);

    putRequest.set("My-special-request-header", "My-special-request-header-value");

    ofBuffer bodyBuffer;
    bodyBuffer.set("Test body data.");

    putRequest.setPutBuffer(bodyBuffer);

    try
    {
        // Execute the request and get the response stream.
        std::istream& responseStream = client.execute(putRequest,
                                                      response,
                                                      context);

        // Request and response headers can be examined here.


        std::cout << "============" << endl;
        // Copy the output to the terminal.
        Poco::StreamCopier::copyStream(responseStream, std::cout);

        // Flush the input stream.
        std::cout << std::endl;

        std::cout << "============" << endl;


    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got Exception " << exc.displayText() << " " << exc.code();
    }
    catch (...)
    {
        ofLogError("ofApp::setup") << "Got unknown exception.";
    }
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
}
