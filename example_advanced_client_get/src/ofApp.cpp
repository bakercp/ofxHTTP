//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Testing redirects and https.
    // This server will echo back everything that you send to it.
    std::string url = "https://httpbin.org/get";

    // Create a client.
    ofxHTTP::Client client;

    // Create a request.
    ofxHTTP::GetRequest request(url);

    // Create a context.
    ofxHTTP::Context context;

    // Set custom session settings.
    //
    // See the class documentation for many additional settings.
    ofxHTTP::ClientSessionSettings sessionSettings;

    // Use an iPhone X user agent string.
    sessionSettings.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");

    // Set a 60 second keep-alive timeout (default is 8 seconds).
    sessionSettings.setKeepAliveTimeout(Poco::Timespan::SECONDS * 60);

    // Save the session settings with the context.
    context.setClientSessionSettings(sessionSettings);

    // Do the query!
    try
    {
        // Execute the request within the given context.
        auto response = client.execute(context, request);

        // Check the response.
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            // A successful response.
            ofLogNotice("ofApp::setup") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";

            // Buffer the response, or otherwise consume the stream.
            ofBuffer buffer(response->stream());

            ofLogNotice("ofApp::setup") << "Content Begin";

            std::cout << buffer << std::endl;

            ofLogNotice("ofApp::setup") << "Content End";
        }
        else
        {
            ofLogError("ofApp::setup") << response->getStatus() << " " << response->getReason();
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << exc.displayText();
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::setup") << exc.what();
    }
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
}
