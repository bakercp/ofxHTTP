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
    std::string uri = "https://httpbin.org/post";
    
    // Here we prepare form fields for the post request.
    // This can be any set of key / value text pairs.
    // We include simple
    std::multimap<std::string, std::string> formFields =
    {
        { "test_0", "123" },
        { "test_1", "321" },
        { "base_64_encoded_image", ofxIO::Base64Encoding::encode(ofBufferFromFile("pixel.gif")) }
    };

    // Here we prepare a multi-part file upload.
    std::vector<ofxHTTP::FormPart> formFiles = {
        ofxHTTP::FormPart(ofxHTTP::FormPart::Type::FILE,
                          "one_pixel_file",
                          "pixel.gif")
    };

    // Create a client.
    ofxHTTP::Client client;
    
    // Create a request.
    ofxHTTP::PostRequest request(uri);
    
    // Set the POST request data.
    // Add the form fields.
    request.addFormFields(formFields);
    // Add the file fields.
    request.addFormParts(formFiles);

    try
    {
        // Execute the request.
        auto response = client.execute(request);
        
        // Check the response.
        if (response->getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            // A successful response.
            ofLogNotice("ofApp::setup") << "Response success, expecting " << response->estimatedContentLength() << " bytes.";
            
            // We expect JSON in response, so we don't bother checking and
            // just read the response directly into an ofJson object.
            //
            // We can also get the response as a ofBuffer (response->buffer()),
            // ofXml, ofPixels, etc and can check to see the response type with
            // the response->isXXX() methods.

            ofJson responseJson = response->json();
            
            ofLogNotice("ofApp::setup") << "Content Begin";
            
            std::cout << responseJson.dump(4) << std::endl;
            
            ofLogNotice("ofApp::setup") << "Content End";
            
            // Here we get our echoed BASE64 encoded image and save it back to
            // the disk after decoding it.
            ofxIO::ByteBuffer encodedBuffer(responseJson["form"].value("base_64_encoded_image", ""));
            ofxIO::ByteBuffer decodedBuffer;
            ofxIO::Base64Encoding encoding;
            encoding.decode(encodedBuffer, decodedBuffer);

            // Now save the buffer back to disk.
            ofxIO::ByteBufferUtils::saveToFile(decodedBuffer, "decoded_pixel.gif");
            
            
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
