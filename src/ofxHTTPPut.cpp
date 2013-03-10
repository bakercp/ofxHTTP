#include "ofxHTTPPut.h"

//------------------------------------------------------------------------------
ofxHTTPPut::ofxHTTPPut(const string& _url, const string& _httpVersion) :
ofxHTTPBaseRequest(HTTPRequest::HTTP_PUT,_url,_httpVersion) { }

//------------------------------------------------------------------------------
ofxHTTPPut::ofxHTTPPut(const URI& _uri, const string& _httpVersion) :
ofxHTTPBaseRequest(HTTPRequest::HTTP_PUT,_uri,_httpVersion) { }

//------------------------------------------------------------------------------
ofxHTTPPut::~ofxHTTPPut() { }

//------------------------------------------------------------------------------
void ofxHTTPPut::prepareRequest(HTTPRequest& request) const {
    
}
