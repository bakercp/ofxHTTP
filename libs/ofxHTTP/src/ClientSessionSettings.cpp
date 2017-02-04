//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ClientSessionSettings.h"


namespace ofx {
namespace HTTP {


const std::string ClientSessionSettings::DEFAULT_USER_AGENT = "Mozilla/5.0 (compatible; Client/1.0 +https://github.com/bakercp/ofxHTTP)";
const std::size_t ClientSessionSettings::DEFAULT_MAX_REDIRECTS = 20;
const Poco::Timespan ClientSessionSettings::DEFAULT_KEEPALIVE_TIMEOUT = Poco::Timespan::SECONDS * 8;
const Poco::Timespan ClientSessionSettings::DEFAULT_TIMEOUT = Poco::Timespan::SECONDS * 60;
const int64_t ClientSessionSettings::DEFAULT_MINIMUM_BYTES_PER_PROGRESS_UPDATE = 1024;
const Poco::Timespan ClientSessionSettings::DEFAULT_MAXIMUM_PROGRESS_UPDATE_INTERVAL = Poco::Timespan::SECONDS * 1;


ClientSessionSettings::ClientSessionSettings():
    _userAgent(DEFAULT_USER_AGENT),
    _maxRedirects(DEFAULT_MAX_REDIRECTS),
    _keepAlive(true),
    _keepAliveTimeout(DEFAULT_KEEPALIVE_TIMEOUT),
    _timeout(DEFAULT_TIMEOUT),
    _minimumBytesPerProgressUpdate(DEFAULT_MINIMUM_BYTES_PER_PROGRESS_UPDATE),
    _maximumProgressUpdateInterval(DEFAULT_MAXIMUM_PROGRESS_UPDATE_INTERVAL)
{
}


ClientSessionSettings::~ClientSessionSettings()
{
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


void ClientSessionSettings::addDefaultHeader(const std::string& name, const std::string& value)
{
    _defaultHeaders.add(name, value);
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


void ClientSessionSettings::setTimeout(Poco::Timespan timeout)
{
    _timeout = timeout;
}


Poco::Timespan ClientSessionSettings::getTimeout() const
{
    return _timeout;
}


void ClientSessionSettings::setProxy(const ProxySettings& proxy)
{
    _proxy = proxy;
}


const ProxySettings& ClientSessionSettings::getProxySettings() const
{
    return _proxy;
}


void ClientSessionSettings::setMinimumBytesPerProgressUpdate(int64_t minimumBytesPerProgressUpdate)
{
    _minimumBytesPerProgressUpdate = minimumBytesPerProgressUpdate;
}


int64_t ClientSessionSettings::getMinimumBytesPerProgressUpdate() const
{
    return _minimumBytesPerProgressUpdate;
}


void ClientSessionSettings::setMaximumProgressUpdateInterval(Poco::Timespan interval)
{
    _maximumProgressUpdateInterval = interval;
}


Poco::Timespan ClientSessionSettings::getMaximumProgressUpdateInterval() const
{
    return _maximumProgressUpdateInterval;
}


} } // namespace ofx::HTTP
