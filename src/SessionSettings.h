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
#include "Poco/Timespan.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofTypes.h"
#include "ProxySettings.h"
#include "Utils.h"


namespace ofx {
namespace HTTP {

    
class SessionSettings {
public:
    
    SessionSettings();
    SessionSettings(SessionSettings& that);
	SessionSettings& operator = (SessionSettings& that);

    ~SessionSettings();

    void setVirtualHost(const string& _virtualHost);
    string getVirtualHost();

    void setDefaultHost(const string& _defaultHost);
    string getDefaultHost();

    void setUserAgent(const string& _userAgent);
    string getUserAgent();
        
    void setMaxRedirects(int _maxRedirects);
    int getMaxRedirects();

    Poco::Net::NameValueCollection getDefaultHeaders();
    void addDefaultHeader(const string& name, const string& value = "");
    void addDefaultHeaders(const Poco::Net::NameValueCollection& _headers);
    bool hasDefaultHeaders();
    bool hasDefaultHeader(const string& name);
    void removeDefaultHeader(const string& name);
    void clearDefaultHeaders();
    
    void setKeepAliveTimeout(unsigned long long milliseconds);
    unsigned long long getKeepAliveTimeout();
    
    void setProxyUsername(const string& username);
    string getProxyUsername();

    void setProxyPassword(const string& password);
    string getProxyPassword();

    void setProxyHost(const string& _host);
    string getProxyHost();
    
    void setProxyPort(unsigned short _port);
    unsigned short getProxyPort();
    
    bool hasProxyCredentials();
    bool isProxyEnabled();
    void clearProxy();
    
    void setUseCredentialStore(bool _bUseCredentialStore);
    bool useCredentialStore();

    void setUseCookielStore(bool _bUseCookieStore);
    bool useCookieStore();
    
private:
    SessionSettings(const SessionSettings& that);
    SessionSettings& operator = (const SessionSettings& that);

    ofMutex mutex;
    
    string virtualHost;
    string defaultHost;

    string userAgent;

    int maxRedirects;
    
    Poco::Net::NameValueCollection defaultHeaders;

    unsigned long long keepAliveTimeout;

    bool   bUseProxy;
    ProxySettings proxy;

    bool   bUseCredentialStore;
    bool   bUseCookieStore;
    
};

    
} }
