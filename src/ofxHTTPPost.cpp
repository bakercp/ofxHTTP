#include "ofxHTTPPost.h"

//------------------------------------------------------------------------------
ofxHTTPPost::ofxHTTPPost(const string& _url, const string& _httpVersion) :
ofxHTTPBaseRequest(HTTPRequest::HTTP_POST,_url,_httpVersion) { }

//------------------------------------------------------------------------------
ofxHTTPPost::ofxHTTPPost(const URI& _uri, const string& _httpVersion) :
ofxHTTPBaseRequest(HTTPRequest::HTTP_POST,_uri,_httpVersion) { }

//------------------------------------------------------------------------------
ofxHTTPPost::~ofxHTTPPost() { }

//------------------------------------------------------------------------------
void ofxHTTPPost::addFormFile(const string& fieldName, const string& filePath) {
    formFiles.add(fieldName, ofToDataPath(filePath));
}

//------------------------------------------------------------------------------
void ofxHTTPPost::addFormFiles(const NameValueCollection& nvc) {
    NameValueCollection::ConstIterator iter = nvc.begin();
    while(iter != nvc.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void ofxHTTPPost::addFormFiles(const map<string,string>& nvc) {
    map<string,string>::const_iterator iter = nvc.begin();
    while(iter != nvc.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void ofxHTTPPost::addFormFiles(const multimap<string,string>&  nvc) {
    multimap<string,string>::const_iterator iter = nvc.begin();
    while(iter != nvc.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool ofxHTTPPost::hasFormFiles() const {
    return !formFiles.empty();
}

//------------------------------------------------------------------------------
void ofxHTTPPost::clearFormFiles() {
    formFiles.clear();
}

//------------------------------------------------------------------------------
const NameValueCollection& ofxHTTPPost::getFormFiles() const {
    return formFiles;
}

//------------------------------------------------------------------------------
void ofxHTTPPost::prepareRequest(HTTPClientSession& session, HTTPRequest& request, HTTPResponse& response) {
    
}
