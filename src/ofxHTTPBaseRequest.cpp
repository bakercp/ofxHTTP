#include "ofxHTTPBaseRequest.h"

//------------------------------------------------------------------------------
ofxHTTPBaseRequest::ofxHTTPBaseRequest(const string& _httpMethod, const string& _uri, const string& _httpVersion) :
httpMethod(_httpMethod),
httpVersion(_httpVersion)
{
    
    // this is kind of a mess, but we want to create requests from string uris
    try {
        uri = URI(_uri);
        bHasValidURI = true;
    } catch(const SyntaxException& exc) {
        invalidURI = _uri;
        uri = URI("http://127.0.0.1");
        bHasValidURI = false;
        ofLogError("ofxHTTPBaseRequest::ofxHTTPBaseRequest") << "Syntax exeption: " << exc.message() << " Setting uri to http://127.0.0.1";
    }
    
    setFormFieldsFromURI(uri);
}

//------------------------------------------------------------------------------
ofxHTTPBaseRequest::ofxHTTPBaseRequest(const string& _httpMethod, const URI& _uri, const string& _httpVersion) :
httpMethod(_httpMethod),
bHasValidURI(true),
uri(_uri),
httpVersion(_httpVersion)
{
    setFormFieldsFromURI(uri);
}

//------------------------------------------------------------------------------
ofxHTTPBaseRequest::~ofxHTTPBaseRequest() { }

//------------------------------------------------------------------------------
string ofxHTTPBaseRequest::getMethod() const {
    return httpMethod;
}

//------------------------------------------------------------------------------
string ofxHTTPBaseRequest::getVersion() const {
    return httpVersion;
}

//------------------------------------------------------------------------------
bool ofxHTTPBaseRequest::hasValidURI() const {
    return bHasValidURI;
}

//------------------------------------------------------------------------------
URI ofxHTTPBaseRequest::getURI() const {
    return uri;
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::addCookie(const string& name, const string& value, bool isValueEscaped) {
    addCookie(HTTPCookie(name, isValueEscaped ? value : HTTPCookie::escape(value)));
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::addCookie(const HTTPCookie& cookie) {
    addCookie(ofxHTTPCookie(cookie));
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::addCookie(const ofxHTTPCookie& cookie) {
    // erase any matching cookies
    vector<ofxHTTPCookie>::iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        if((*iter).matches(cookie)) {
            iter = cookies.erase(iter);
        } else {
            ++iter;
        }
    }
    
    if(!cookie.isExpired()) {
        cookies.push_back(cookie);
    }
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::addCookies(const vector<ofxHTTPCookie>& _cookies) {
    vector<ofxHTTPCookie>::const_iterator iter = _cookies.begin();
    while(iter != _cookies.end()) {
        addCookie(*iter);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::clearCookies() {
    cookies.clear();
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::addFormField(const string& name, const string& value) {
    form.add(name, value);
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::addFormFields(const NameValueCollection& formFields) {
    NameValueCollection::ConstIterator iter = formFields.begin();
    while(iter != formFields.end()) {
        addFormField((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool ofxHTTPBaseRequest::hasFormFields() const {
    return !form.empty();
}

//------------------------------------------------------------------------------
bool ofxHTTPBaseRequest::hasFormField(const string& name) const {
    return form.has(name);
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::removeFormField(const string& name) {
    form.erase(name);
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::clearFormFields() {
    form.clear();
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::addHeader(const string& name, const string& value) {
    headers.add(name,value);
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::addHeaders(const NameValueCollection& _headers) {
    NameValueCollection::ConstIterator iter = _headers.begin();
    while(iter != _headers.end()) {
        addHeader((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::removeHeader(const string& name) {
    headers.erase(name);
}

//------------------------------------------------------------------------------
bool ofxHTTPBaseRequest::hasHeaders() const {
    return !headers.empty();
}

//------------------------------------------------------------------------------
bool ofxHTTPBaseRequest::hasHeader(const string& name) const {
    headers.has(name);
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::clearHeaders() {
    headers.clear();
}

//------------------------------------------------------------------------------
void ofxHTTPBaseRequest::setFormFieldsFromURI(const URI& uri) {
    // attempt to extract uri query as form fields
    vector <string> queryTokens = ofSplitString(uri.getQuery(),"&",true,true);
    
    vector<string>::const_iterator iter = queryTokens.begin();
    
    while(iter != queryTokens.end()) {
        string queryToken = (*iter);
        size_t index = queryToken.find_first_of("=");
        
        if(index != string::npos) {
            string name  = queryToken.substr(0,index);
            string value = queryToken.substr(index + 1);
            addFormField(name,value);
        } else {
            addFormField(queryToken);
        }
        ++iter;
    }
}
