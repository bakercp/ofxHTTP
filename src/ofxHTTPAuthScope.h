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

#include <iostream>
#include <sstream>
#include <string>

#include "ofxHTTPConstants.h"

#include "Poco/URI.h"

using std::istream;
using std::ostream;
using std::string;
using std::stringstream;

using Poco::URI;

class ofxHTTPAuthScope {
public:
    
    ofxHTTPAuthScope();
    
    ofxHTTPAuthScope(const string& host);

    ofxHTTPAuthScope(const string& host,
                     unsigned short _port);

    ofxHTTPAuthScope(const string& host,
                     ofxHTTPAuthType _type);
    
    ofxHTTPAuthScope(const string& host,
                     unsigned short _port,
                     ofxHTTPAuthType _type);

    ofxHTTPAuthScope(const string& _scheme,
                     const string& _host,
                     unsigned short _port,
                     const string& _realm,
                     ofxHTTPAuthType _type);
    
    ofxHTTPAuthScope(const URI& uri);
    
    bool isSchemeSet() const;
    void clearScheme();
    void setScheme(const string& _scheme);
    string getScheme() const;

    bool isHostSet() const;
    void clearHost();
    void setHost(const string& _host);
    string getHost() const;

    bool isPortSet() const;
    void clearPort();
    void setPort(unsigned short _port);
    unsigned short getPort() const;

    bool isRealmSet() const;
    void clearRealm();
    void setRealm(const string& _realm);
    string getRealm() const;

    bool isAuthTypeSet() const;
    void clearAuthType();
    void setAuthType(ofxHTTPAuthType _authType);
    ofxHTTPAuthType getAuthType() const;

    int match(const ofxHTTPAuthScope& scope) const;
    
    bool operator== (const ofxHTTPAuthScope& scope) const;
    bool operator!= (const ofxHTTPAuthScope& scope) const;
    bool operator<  (const ofxHTTPAuthScope& scope) const;

    string toString() const;
    
private:
    void init();
    
    bool bIsSchemeSet;
    string scheme;

    bool bIsHostSet;
    string host;
    
    bool bIsPortSet;
    unsigned short port;
    
    bool bIsRealmSet;
    string realm;
    
    bool bIsAuthTypeSet;
    ofxHTTPAuthType authType;

};
