#include "ofxHTTPGet.h"

//------------------------------------------------------------------------------
ofxHTTPGet::ofxHTTPGet(const string& _url, const string& _httpVersion) :
ofxHTTPBaseRequest(HTTPRequest::HTTP_GET, _url, _httpVersion) { }

//------------------------------------------------------------------------------
ofxHTTPGet::ofxHTTPGet(const URI& _uri, const string& _httpVersion) :
ofxHTTPBaseRequest(HTTPRequest::HTTP_GET, _uri, _httpVersion) { }

//------------------------------------------------------------------------------
ofxHTTPGet::~ofxHTTPGet() { }

//------------------------------------------------------------------------------
void ofxHTTPGet::prepareRequest(HTTPRequest& request) const {
 
    cout << "prepping" << endl;
}
