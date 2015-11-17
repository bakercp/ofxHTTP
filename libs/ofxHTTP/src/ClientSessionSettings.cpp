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


#include "ofx/HTTP/ClientSessionSettings.h"


namespace ofx {
namespace HTTP {


const std::string    ClientSessionSettings::DEFAULT_USER_AGENT        = "Mozilla/5.0 (compatible; Client/1.0 +https://github.com/bakercp/ofxHTTP)";
const int            ClientSessionSettings::DEFAULT_MAX_REDIRECTS     = 20;
const Poco::Timespan ClientSessionSettings::DEFAULT_KEEPALIVE_TIMEOUT = Poco::Timespan::SECONDS * 8;


ClientSessionSettings::ClientSessionSettings():
    _virtualHost(""),
    _defaultHost(""),
    _userAgent(DEFAULT_USER_AGENT),
    _maxRedirects(DEFAULT_MAX_REDIRECTS),
    _keepAlive(true),
    _keepAliveTimeout(DEFAULT_KEEPALIVE_TIMEOUT)
{
}


ClientSessionSettings::~ClientSessionSettings()
{
}


void ClientSessionSettings::setVirtualHost(const std::string& virtualHost)
{
    _virtualHost = virtualHost;
}


std::string ClientSessionSettings::getVirtualHost() const
{
    return _virtualHost;
}


void ClientSessionSettings::setDefaultHost(const std::string& defaultHost)
{
    _defaultHost = defaultHost;
}


std::string ClientSessionSettings::getDefaultHost() const
{
    return _defaultHost;
}


void ClientSessionSettings::setUserAgent(const std::string& userAgent)
{
    _userAgent = userAgent;
}


std::string ClientSessionSettings::getUserAgent() const
{
    return _userAgent;
}


void ClientSessionSettings::setMaxRedirects(std::size_t maxRedirects)
{
    _maxRedirects = maxRedirects;
}


std::size_t ClientSessionSettings::getMaxRedirects() const
{
    return _maxRedirects;
}


const Poco::Net::NameValueCollection& ClientSessionSettings::getDefaultHeaders() const
{
    return _defaultHeaders;
}


void ClientSessionSettings::setDefaultHeaders(const Poco::Net::NameValueCollection& headers)
{
    _defaultHeaders = headers;
}


void ClientSessionSettings::setkeepAlive(bool keepAlive)
{
    _keepAlive = keepAlive;
}


bool ClientSessionSettings::getKeepAlive() const
{
    return _keepAlive;
}


void ClientSessionSettings::setKeepAliveTimeout(Poco::Timespan keepAliveTimeout)
{
    _keepAliveTimeout = keepAliveTimeout;
}


Poco::Timespan ClientSessionSettings::getKeepAliveTimeout() const
{
    return _keepAliveTimeout;
}


void ClientSessionSettings::setProxy(const ProxySettings& proxy)
{
    _proxy = proxy;
}


const ProxySettings& ClientSessionSettings::getProxySettings() const
{
    return _proxy;
}


} } // namespace ofx::HTTP
