// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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
    // Fill in your twitter developer info.
    std::string consumerKey = "";
    std::string consumerSecret = "";
    std::string accessToken = "";
    std::string accessKey = "";

    // Set up the request filter.
    twitterRequestFilter.setup(consumerKey, consumerSecret, accessToken, accessKey);

    // Add the request filter to the client.
    // The request filter will automatically sign all requests before sending.
    client.addRequestFilter(&twitterRequestFilter);

    // Choose a twitter end point.
    std::string url = "https://api.twitter.com/1.1/search/tweets.json";

    // Add search paramters for the end point.
    Poco::Net::NameValueCollection nvc;
    nvc.add("q", "love");

    // Create a get request.
    ofx::HTTP::GetRequest request(url, nvc);

    // Initialized an empty response.
    ofx::HTTP::BaseResponse response;

    try
    {
        // Execute teh get request.
        std::istream& responseStream = client.execute(request, response, context);

        // Copy the response to a string and parse it as JSON.
        std::string buffer;

        Poco::StreamCopier::copyToString(responseStream, buffer);

        Json::Value json;
        Json::Reader reader;

        if (!reader.parse(buffer, json))
        {
            std::cout << "Unable to parse string: " << reader.getFormattedErrorMessages() << std::endl;
        }
        else
        {
            std::cout << json.toStyledString() << std::endl;
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got exception " << exc.displayText() << " " << exc.code();
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
