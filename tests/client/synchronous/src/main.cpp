//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofxHTTP.h"
#include "ofxUnitTests.h"


class ofApp: public ofxUnitTestsApp
{
    void run()
    {
        testHeaders();
        testMethods();
    }

    void testHeaders()
    {
        testUserAgent();

        Poco::Net::NameValueCollection defaultHeaders;
        defaultHeaders.add("Dnt", "1");
        defaultHeaders.add("Upgrade-Insecure-Requests","1");
        ofxHTTP::ClientSessionSettings settings;
        settings.setDefaultHeaders(defaultHeaders);
        ofxHTTP::Context context(settings);
        ofxHTTP::GetRequest request("https://httpbin.org/headers");
        auto response = client.execute(context, request);

        ofJson json;
        json << response->stream();

        for (auto& entry: defaultHeaders)
        {
            test_eq(json["headers"][entry.first], entry.second, "Test headers fields.");
        }

    }

    void testUserAgent()
    {
        std::string userAgent = "ofxHTTP Test Suite/1.0";

        ofxHTTP::ClientSessionSettings settings;
        settings.setUserAgent(userAgent);
        ofxHTTP::Context context(settings);
        ofxHTTP::GetRequest request("https://httpbin.org/user-agent");
        auto response = client.execute(context, request);

        ofJson json;
        json << response->stream();

        test_eq(json["user-agent"], userAgent, "Test user-agent.");

    }

    void testMethods()
    {
        testMethodGet();
        testMethodPost();
        testMethodPatch();
        testMethodPut();
        testMethodDelete();
    }


    void testMethodGet()
    {

    }

    void testMethodPost()
    {
        std::multimap<std::string, std::string> formFields =
        {
            { "test", "123" },
            { "123", "test" }
        };

        std::vector<ofxHTTP::FormPart> formFiles = {
            ofxHTTP::FormPart(ofxHTTP::FormPart::Type::FILE,
                              "one pixel",
                              "pixel.gif")
        };
        
        ofxHTTP::Context context;

        {
            ofxHTTP::PostRequest request("https://httpbin.org/post",
                                         Poco::Net::HTTPMessage::HTTP_1_0);
            request.addFormFields(formFields);
            request.addFormParts(formFiles);

            auto response = client.execute(context, request);

            ofJson json;
            json << response->stream();

            for (auto& entry: formFields)
            {
                test_eq(json["form"][entry.first], entry.second, "Test form fields.");
            }
        }
        {
            ofxHTTP::PostRequest request("https://httpbin.org/post",
                                         Poco::Net::HTTPMessage::HTTP_1_0);
            request.addFormFields(formFields);
            request.addFormParts(formFiles);

            auto response = client.execute(context, request);

            ofJson json;
            json << response->stream();

            for (auto& entry: formFields)
            {
                test_eq(json["form"][entry.first], entry.second, "Test form fields.");
            }
        }
    }


    void testIp()
    {

    }
    


    void testMethodPatch()
    {

    }

    void testMethodPut()
    {

    }

    void testMethodDelete()
    {

    }

    void testEncodingUTF8()
    {

    }

    void testContentEncodingGZIP()
    {

    }

    void testContentEncodingDeflate()
    {

    }

    void testStatusCodes()
    {

    }

    void testResponseHeaders()
    {

    }

    void testRedirects()
    {

    }

    void testRedirectRelative()
    {

    }

    void testRedirectAbsolute()
    {

    }

    void testCookies()
    {

    }


//    / This page.
//    /ip Returns Origin IP.
//    /user-agent Returns user-agent.
//    /headers Returns header dict.
//    /get Returns GET data.
//    /post Returns POST data.
//    /patch Returns PATCH data.
//    /put Returns PUT data.
//    /delete Returns DELETE data
//    /encoding/utf8 Returns page containing UTF-8 data.
//    /gzip Returns gzip-encoded data.
//    /deflate Returns deflate-encoded data.
//    /status/:code Returns given HTTP Status code.
//    /response-headers?key=val Returns given response headers.
//    /redirect/:n 302 Redirects n times.
//    /redirect-to?url=foo 302 Redirects to the foo URL.
//    /relative-redirect/:n 302 Relative redirects n times.
//    /absolute-redirect/:n 302 Absolute redirects n times.
//    /cookies Returns cookie data.
//    /cookies/set?name=value Sets one or more simple cookies.
//    /cookies/delete?name Deletes one or more simple cookies.
//    /basic-auth/:user/:passwd Challenges HTTPBasic Auth.
//    /hidden-basic-auth/:user/:passwd 404'd BasicAuth.
//    /digest-auth/:qop/:user/:passwd Challenges HTTP Digest Auth.
//    /stream/:n Streams min(n, 100) lines.
//    /delay/:n Delays responding for min(n, 10) seconds.
//        /drip?numbytes=n&duration=s&delay=s&code=code Drips data over a duration after an optional initial delay, then (optionally) returns with the given status code.
//        /range/1024?duration=s&chunk_size=code Streams n bytes, and allows specifying a Range header to select a subset of the data. Accepts a chunk_size and request duration parameter.
//        /html Renders an HTML Page.
//        /robots.txt Returns some robots.txt rules.
//        /deny Denied by robots.txt file.
//        /cache Returns 200 unless an If-Modified-Since or If-None-Match header is provided, when it returns a 304.
//        /cache/:n Sets a Cache-Control header for n seconds.
//            /bytes/:n Generates n random bytes of binary data, accepts optional seed integer parameter.
//            /stream-bytes/:n Streams n random bytes of binary data, accepts optional seed and chunk_size integer parameters.
//            /links/:n Returns page containing n HTML links.
//            /image Returns page containing an image based on sent Accept header.
//            /image/png Returns page containing a PNG image.
//            /image/jpeg Returns page containing a JPEG image.
//            /image/webp Returns page containing a WEBP image.
//            /image/svg Returns page containing a SVG image.
//            /forms/post HTML form that submits to /post
//            /xml Returns some XML

    ofxHTTP::Client client;

};


#include "ofAppNoWindow.h"
#include "ofAppRunner.h"


int main()
{
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
