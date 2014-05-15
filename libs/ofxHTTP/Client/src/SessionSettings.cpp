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


#include "ofx/HTTP/Client/SessionSettings.h"


namespace ofx {
namespace HTTP {


const std::string    SessionSettings::DEFAULT_USER_AGENT        = "Mozilla/5.0 (compatible; Client/1.0 +https://github.com/bakercp/ofxHTTP)";
const int            SessionSettings::DEFAULT_MAX_REDIRECTS     = 20;
const Poco::Timespan SessionSettings::DEFAULT_KEEPALIVE_TIMEOUT = Poco::Timespan::SECONDS * 8;


SessionSettings::SessionSettings():
    _virtualHost(""),
    _defaultHost(""),
    _userAgent(DEFAULT_USER_AGENT),
    _maxRedirects(DEFAULT_MAX_REDIRECTS),
    _keepAlive(true),
    _keepAliveTimeout(DEFAULT_KEEPALIVE_TIMEOUT)
{
}


SessionSettings::~SessionSettings()
{
}


void SessionSettings::setVirtualHost(const std::string& virtualHost)
{
    _virtualHost = virtualHost;
}


std::string SessionSettings::getVirtualHost() const
{
    return _virtualHost;
}


void SessionSettings::setDefaultHost(const std::string& defaultHost)
{
    _defaultHost = defaultHost;
}


std::string SessionSettings::getDefaultHost() const
{
    return _defaultHost;
}


void SessionSettings::setUserAgent(const std::string& userAgent)
{
    _userAgent = userAgent;
}


std::string SessionSettings::getUserAgent() const
{
    return _userAgent;
}


void SessionSettings::setMaxRedirects(std::size_t maxRedirects)
{
    _maxRedirects = maxRedirects;
}


std::size_t SessionSettings::getMaxRedirects() const
{
    return _maxRedirects;
}


const Poco::Net::NameValueCollection& SessionSettings::getDefaultHeaders() const
{
    return _defaultHeaders;
}


void SessionSettings::setDefaultHeaders(const Poco::Net::NameValueCollection& headers)
{
    _defaultHeaders = headers;
}


void SessionSettings::setkeepAlive(bool keepAlive)
{
    _keepAlive = keepAlive;
}


bool SessionSettings::getKeepAlive() const
{
    return _keepAlive;
}


void SessionSettings::setKeepAliveTimeout(Poco::Timespan keepAliveTimeout)
{
    _keepAliveTimeout = keepAliveTimeout;
}


Poco::Timespan SessionSettings::getKeepAliveTimeout() const
{
    return _keepAliveTimeout;
}


void SessionSettings::setProxy(const ProxySettings& proxy)
{
    _proxy = proxy;
}


const ProxySettings& SessionSettings::getProxySettings() const
{
    return _proxy;
}


} } // namespace ofx::HTTP
