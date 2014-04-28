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
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSSLManager::registerAllEvents(this);

    HTTP::DefaultSessionProvider defaultSessionProvider;
    HTTP::DefaultProxyProcessor defaultProxyProcessor;
    HTTP::DefaultAuthenticationProcessor defaultAuthenticationProcessor;
    HTTP::DefaultRedirectProcessor defaultRedirectProcessor;

    HTTP::Client::DefaultClient client(defaultSessionProvider,
                                       defaultProxyProcessor,
                                       defaultAuthenticationProcessor,
                                       defaultRedirectProcessor);

    client.addRequestProcessor(&twitterClient);

    consumerKey = "0temMq9KWLx8731GcTXevA";
    consumerSecret = "7bI0ScK5gBN3dlGxaC9oTuVFQQXThC9prfD28nmrY";
    accessToken = "526698210-VEBz3v06S2RZ21m6JWpYZEhd6mH3TqDYNiKMhN5f";
    accessKey = "X922JPvQehFVpa7WujPHz58nkxcaHjHJmbn69bEEMmXAR";

//    consumerKey = "5402922a022e48ef99a97dfe85c52716";
//    consumerSecret = "bbda9ab73774451ea92b70aa45f5e507";
//    accessToken = "4e6b759b252219a4d11ed95559bf667b";
//    accessKey = "17efedea0975c2a3360ca20526eb9d83";

    twitterClient.login(consumerKey, consumerSecret, accessToken, accessKey);

    std::string url = "https://api.twitter.com/1.1/search/tweets.json";
//    std::string url = "https://stream.twitter.com/1.1/statuses/filter.json";
//    std::string url = "https://api.twitter.com/1.1/statuses/update_with_media.json";
//                       /1.1/statuses/update_with_media.json
//    std::string url = "https://api.twitter.com/1.1/statuses/update.json";

//    std::string url = "https://api.fitbit.com/1/user/-/profile.json";

    HTTP::Context context;

    Poco::Net::NameValueCollection nvc;
    nvc.add("q", "love");

    HTTP::Client::GetRequest request(url, nvc);
//    HTTP::Client::PostRequest request(url, Poco::Net::HTTPMessage::HTTP_1_1);

//    request.addFormFile("media[]", "openFrameworks.png");
//    request.addFormField("status", "test from openFrameworks");

//    request.addFormField("track", "twitter");

    HTTP::Client::BaseResponse response;

    try
    {
        std::cout << "here : " << std::endl;

//        ofBuffer buffer;

        std::istream& responseStream = client.execute(request, response, context);

        std::cout << "1: here : " << std::endl;

        Poco::StreamCopier::copyStream(responseStream, std::cout);

//        ofxJSONElement json(buffer.getText());
////
//        cout << json.toStyledString() << endl;
//
//        cout << "======" << endl;
//        cout << buffer.getText() << endl;
//        cout << "======" << endl;

        HTTP::Utils::dumpHeaders(request);
        HTTP::Utils::dumpHeaders(response);

        cout << "URI = " << request.getURI() << endl;
        cout << "STATUS = " << response.getStatus() << endl;
        cout << "REASON = " << response.getReason() << endl;

    }
    catch(const Poco::SyntaxException& exc)
    {
        ofLogError("ofApp::setup") << "Got SyntaxException exception " << exc.displayText() << " " << exc.code();
    }
    catch(const Poco::Net::HostNotFoundException& exc)
    {
        ofLogError("ofApp::setup") << "Got HostNotFoundException exception " << exc.displayText() << " " << exc.code();
    }
    catch(const Poco::Net::HTTPException& exc)
    {
        ofLogError("ofApp::setup") << "Got HTTPException exception " << exc.displayText() << " " << exc.code();
    }
    catch(const Poco::Exception& exc)
    {
        ofLogError("ofApp::setup") << "Got Exception exception " << exc.displayText() << " " << exc.code();
    }
    catch(...)
    {
        ofLogError("ofApp::setup") << "Got unknown exception.";
    }

//    cookieStore->dump();

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
    std::cout << "ofApp::onHTTPClientResponseEvent:" << std::endl;
    std::cout << args.getRequest().getRequestId().toString() << endl;
    std::cout << args.getBuffer().getText() << endl;
    return true;
}


bool ofApp::onHTTPClientErrorEvent(HTTP::Client::ClientErrorEventArgs& args)
{
    std::cout << "ofApp::onHTTPClientErrorEvent:" << std::endl;
    std::cout << args.getRequest().getRequestId().toString() << endl;
    std::cout << args.getException().displayText() << std::endl;
    return true;
}
