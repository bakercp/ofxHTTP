// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <string>
#include "Poco/Timespan.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofTypes.h"
#include "ofx/HTTP/Client/ProxySettings.h"
#include "ofx/HTTP/HeaderUtils.h"


namespace ofx {
namespace HTTP {

    
class SessionSettings
{
public:
    SessionSettings();
    SessionSettings(SessionSettings& that);
	SessionSettings& operator = (SessionSettings& that);

    virtual ~SessionSettings();

    void setVirtualHost(const std::string& virtualHost);
    std::string getVirtualHost() const;

    void setDefaultHost(const std::string& defaultHost);
    std::string getDefaultHost() const;

    void setUserAgent(const std::string& userAgent);
    std::string getUserAgent() const;
        
    void setMaxRedirects(int maxRedirects);
    int getMaxRedirects() const;

    Poco::Net::NameValueCollection getDefaultHeaders() const;
    void addDefaultHeader(const std::string& name, const std::string& value = "");
    void addDefaultHeaders(const Poco::Net::NameValueCollection& headers);
    bool hasDefaultHeaders() const;
    bool hasDefaultHeader(const std::string& name);
    void removeDefaultHeader(const std::string& name);
    void clearDefaultHeaders();
    
    void setKeepAliveTimeout(Poco::Timespan keepAliveTimeout);
    Poco::Timespan getKeepAliveTimeout() const;
    
    void setProxyUsername(const std::string& username);
    std::string getProxyUsername() const;

    void setProxyPassword(const std::string& password);
    std::string getProxyPassword() const;

    void setProxyHost(const std::string& host);
    std::string getProxyHost() const;
    
    void setProxyPort(uint16_t port);
    uint16_t getProxyPort() const;
    
    bool hasProxyCredentials() const;
    bool isProxyEnabled() const;
    void clearProxy();
    
    void setUseCredentialStore(bool bUseCredentialStore);
    bool useCredentialStore() const;

    void setUseCookielStore(bool bUseCookieStore);
    bool useCookieStore() const;
    
    static const std::string    DEFAULT_USER_AGENT;
    static const int            DEFAULT_MAX_REDIRECTS;
    static const Poco::Timespan DEFAULT_KEEPALIVE_TIMEOUT;

private:
    SessionSettings(const SessionSettings& that);
    SessionSettings& operator = (const SessionSettings& that);

    mutable Poco::FastMutex _mutex;
    
    std::string _virtualHost;
    std::string _defaultHost;

    std::string _userAgent;

    int _maxRedirects;

    Poco::Timespan _keepAliveTimeout;

    bool            _bUseProxy;
    ProxySettings   _proxy;

    bool _useCookieStore;
    bool _useCredentialStore;

    Poco::Net::NameValueCollection _defaultHeaders;

};

    
} } // namespace ofx::HTTP
