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
 
 ==============================================================================*/

#pragma once

#include <vector>
#include <string>

#include "Poco/RegularExpression.h"
#include "Poco/String.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NameValueCollection.h"

#include "ofLog.h"
#include "ofTypes.h"
#include "ofUtils.h"

#include "ofxHTTPConstants.h"
#include "ofxHTTPCookie.h"
#include "ofxHTTPUtils.h"

using std::vector;
using std::string;

using Poco::URI;
using Poco::Net::HTTPCookie;
using Poco::Net::HTTPResponse;

// thread-safe cookie store
class ofxHTTPCookieStore {
public:
    ofxHTTPCookieStore();
    ofxHTTPCookieStore(ofxHTTPCookieStore& that);
	ofxHTTPCookieStore& operator = (ofxHTTPCookieStore& that);

    virtual ~ofxHTTPCookieStore();
    
    vector<ofxHTTPCookie> getCookesForURI(const URI& uri, bool matchSessionCookes = true);
    
    void store(const HTTPResponse& response);

    void addCookie(const string& name, const string& value = "", bool isValueEscaped = false);
    void addCookie(const HTTPCookie& cookie);
    void addCookie(const ofxHTTPCookie& cookie);
    
    void addCookies(const vector<HTTPCookie>& cookies);

    bool clearCookiesWithName(const string& name);
    bool clearCookiesWithDomain(const string& domain);
    bool clearCookiesWithPath(const string& path);
    bool clearExpired(unsigned long long expiredAt = ofGetSystemTime());
    void clear();

    void dump();
    
protected:
    void addCookieWithExistingLock(const ofxHTTPCookie& cookie);

private:
    ofxHTTPCookieStore(const ofxHTTPCookieStore& that);
	ofxHTTPCookieStore& operator = (const ofxHTTPCookieStore& that);

    ofMutex mutex;
    
    vector<ofxHTTPCookie> cookies;
    
};