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


#include <iostream>
#include <sstream>
#include <string>
#include "Poco/URI.h"
#include "Authentication.h"


namespace ofx {
namespace HTTP {


class AuthScope {
public:
    
    AuthScope();
    
    AuthScope(const std::string& host);
    AuthScope(const std::string& host, unsigned short port);
    AuthScope(const std::string& host, Authentication::Type authType);
    AuthScope(const std::string& host,
              unsigned short port,
              Authentication::Type authType);
    AuthScope(const std::string& scheme,
              const std::string& host,
              unsigned short port,
              const std::string& realm,
              Authentication::Type authType);
    AuthScope(const Poco::URI& uri);
    
    bool isSchemeSet() const;
    void clearScheme();
    void setScheme(const std::string& scheme);
    std::string getScheme() const;

    bool isHostSet() const;
    void clearHost();
    void setHost(const std::string& host);
    std::string getHost() const;

    bool isPortSet() const;
    void clearPort();
    void setPort(unsigned short port);
    unsigned short getPort() const;

    bool isRealmSet() const;
    void clearRealm();
    void setRealm(const std::string& realm);
    std::string getRealm() const;

    bool isAuthTypeSet() const;
    void clearAuthType();
    void setAuthType(Authentication::Type authType);
    Authentication::Type getAuthType() const;

    int match(const AuthScope& scope) const;
    
    bool operator== (const AuthScope& scope) const;
    bool operator!= (const AuthScope& scope) const;
    bool operator<  (const AuthScope& scope) const;

    std::string toString() const;
    
private:
    bool _bIsSchemeSet;
    std::string _scheme;

    bool _bIsHostSet;
    std::string _host;
    
    bool _bIsPortSet;
    unsigned short _port;
    
    bool _bIsRealmSet;
    std::string _realm;
    
    bool _bIsAuthTypeSet;
    Authentication::Type _authType;

};


} }
