// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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
#include "ofx/HTTP/ProxySettings.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {

    
class ClientSessionSettings
{
public:
    ClientSessionSettings();
    virtual ~ClientSessionSettings();

    void setVirtualHost(const std::string& virtualHost);
    std::string getVirtualHost() const;

    void setDefaultHost(const std::string& defaultHost);
    std::string getDefaultHost() const;

    void setUserAgent(const std::string& userAgent);
    std::string getUserAgent() const;
        
    void setMaxRedirects(std::size_t maxRedirects);
    std::size_t getMaxRedirects() const;

    const Poco::Net::NameValueCollection& getDefaultHeaders() const;
    void setDefaultHeaders(const Poco::Net::NameValueCollection& headers);

    void setkeepAlive(bool keepAlive);
    bool getKeepAlive() const;

    void setKeepAliveTimeout(Poco::Timespan keepAliveTimeout);
    Poco::Timespan getKeepAliveTimeout() const;

    void setProxy(const ProxySettings& proxy);
    const ProxySettings& getProxySettings() const;

    static const std::string    DEFAULT_USER_AGENT;
    static const int            DEFAULT_MAX_REDIRECTS;
    static const Poco::Timespan DEFAULT_KEEPALIVE_TIMEOUT;

private:
    std::string _virtualHost;
    std::string _defaultHost;

    std::string _userAgent;

    std::size_t _maxRedirects;

    bool _keepAlive;
    Poco::Timespan _keepAliveTimeout;

    ProxySettings _proxy;

    Poco::Net::NameValueCollection _defaultHeaders;

};

    
} } // namespace ofx::HTTP
