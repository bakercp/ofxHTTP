/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 =============================================================================*/

#pragma once

#include <string>

#include "Poco/Exception.h"
#include "Poco/URI.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NameValueCollection.h"

#include "ofLog.h"
#include "ofUtils.h"

#include "ofxHTTPCookie.h"

using std::string;
using std::vector;

using Poco::SyntaxException;
using Poco::URI;

using Poco::Net::HTTPCookie;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPCredentials;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::NameValueCollection;

class ofxHTTPBaseRequest {
public:
    
    ofxHTTPBaseRequest(const string& _httpMethod,
                       const string& _url,
                       const string& _httpVersion);

    ofxHTTPBaseRequest(const string& _httpMethod,
                       const URI& _uri,
                       const string& _httpVersion);

    virtual ~ofxHTTPBaseRequest();
    
    string getMethod() const;
    string getVersion() const;
    
    bool hasValidURI() const;
    URI  getURI() const;

    // these cookies will replace any matching cookies provided by the client's cookie store
    void addCookie(const string& name, const string& value = "", bool isValueEscaped = false);
    void addCookie(const HTTPCookie& cookie);
    void addCookie(const ofxHTTPCookie& cookie);
    void addCookies(const vector<ofxHTTPCookie>& _cookies);
    void clearCookies();
    
    void addFormField(const string& name, const string& value = "");
    void addFormFields(const NameValueCollection& formFields);
    bool hasFormFields() const;
    bool hasFormField(const string& name) const;
    void removeFormField(const string& name);
    void clearFormFields();
    
    void addHeader(const string& name, const string& value = "");
    void addHeaders(const NameValueCollection& formFields);
    bool hasHeaders() const;
    bool hasHeader(const string& name) const;
    void removeHeader(const string& name);
    void clearHeaders();

protected:
    
    void setFormFieldsFromURI(const URI& uri);
    
    virtual void prepareRequest(HTTPRequest& request) const = 0;
    virtual void sendRequestBody(ostream& requestStream) const { }; // optional for POST, PUT, etc.
    
    bool bHasValidURI;
    string invalidURI;
    URI uri;
    
    string  httpMethod;
    string  httpVersion;

    vector<ofxHTTPCookie> cookies;
    
    NameValueCollection form;
    NameValueCollection headers;

    friend class ofxHTTPClient;
    
};