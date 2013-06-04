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
#include "Cookie.h"


namespace ofx {
namespace HTTP {

    
class Client;  // N.B. forward declaration needed for sub-namespace
               // to befriend a class in a parent namespace


namespace Request {


class BaseRequest {
public:
    
    BaseRequest(const std::string& _httpMethod,
                const std::string& _url,
                const std::string& _httpVersion);

    BaseRequest(const std::string& _httpMethod,
                const Poco::URI& _uri,
                const std::string& _httpVersion);

    virtual ~BaseRequest();
    
    std::string getMethod() const;
    std::string getVersion() const;
    
    bool hasValidURI() const;
    Poco::URI  getURI() const;

    // these cookies will replace any matching cookies provided by the client's cookie store
    void addCookie(const std::string& name,
                   const std::string& value = "",
                   bool isValueEscaped = false);
    void addCookie(const Poco::Net::HTTPCookie& cookie);
    void addCookie(const Cookie& cookie);
    void addCookies(const vector<Cookie>& _cookies);
    void clearCookies();
    
    void addFormField(const std::string& name, const std::string& value = "");
    void addFormFields(const Poco::Net::NameValueCollection& formFields);
    bool hasFormFields() const;
    bool hasFormField(const std::string& name) const;
    void removeFormField(const std::string& name);
    void clearFormFields();
    
    void addHeader(const std::string& name, const std::string& value = "");
    void addHeaders(const Poco::Net::NameValueCollection& formFields);
    bool hasHeaders() const;
    bool hasHeader(const std::string& name) const;
    void removeHeader(const std::string& name);
    void clearHeaders();

protected:
    
    void setFormFieldsFromURI(const Poco::URI& uri);
    
    virtual void prepareRequest(Poco::Net::HTTPRequest& request) const = 0;
    virtual void sendRequestBody(ostream& requestStream) const { }; // optional for POST, PUT, etc.
    
    bool bHasValidURI;
    std::string invalidURI;
    Poco::URI uri;
    
    std::string  httpMethod;
    std::string  httpVersion;

    vector<Cookie> cookies;
    
    Poco::Net::NameValueCollection form;
    Poco::Net::NameValueCollection headers;

    friend class Client;
    
};


} } }
