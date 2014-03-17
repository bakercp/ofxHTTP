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


SessionSettings::SessionSettings() :
    _virtualHost(""),
    _defaultHost(""),
    _userAgent(DEFAULT_USER_AGENT),
    _maxRedirects(DEFAULT_MAX_REDIRECTS),
    _keepAliveTimeout(DEFAULT_KEEPALIVE_TIMEOUT),
    _bUseProxy(false),
    _proxy(ProxySettings()),
    _useCookieStore(true),
    _useCredentialStore(true)
{
}

SessionSettings::SessionSettings(SessionSettings& that)
{
    Poco::FastMutex::ScopedLock lock(that._mutex);

    _virtualHost         = that._virtualHost;
    _defaultHost         = that._defaultHost;
    _userAgent           = that._userAgent;
    _maxRedirects        = that._maxRedirects;
    _defaultHeaders      = that._defaultHeaders;
    _keepAliveTimeout    = that._keepAliveTimeout;
    _bUseProxy           = that._bUseProxy;
    _proxy               = that._proxy;
    _useCredentialStore  = that._useCredentialStore;
    _useCookieStore      = that._useCookieStore;
    
}


SessionSettings& SessionSettings::operator = (SessionSettings& that)
{
    Poco::FastMutex::ScopedLock thisLock(_mutex);
    Poco::FastMutex::ScopedLock thatLock(that._mutex);
    
    _virtualHost         = that._virtualHost;
    _defaultHost         = that._defaultHost;
    _userAgent           = that._userAgent;
    _maxRedirects        = that._maxRedirects;
    _defaultHeaders      = that._defaultHeaders;
    _keepAliveTimeout    = that._keepAliveTimeout;
    _bUseProxy           = that._bUseProxy;
    _proxy               = that._proxy;
    _useCredentialStore  = that._useCredentialStore;
    _useCookieStore      = that._useCookieStore;

    return *this;
    
}


SessionSettings::~SessionSettings()
{
}


void SessionSettings::setVirtualHost(const std::string& virtualHost)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _virtualHost = virtualHost;
}


string SessionSettings::getVirtualHost() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _virtualHost;
}


void SessionSettings::setDefaultHost(const std::string& defaultHost)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _defaultHost = defaultHost;
}


std::string SessionSettings::getDefaultHost() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _defaultHost;
}


void SessionSettings::setUserAgent(const std::string& userAgent)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _userAgent = userAgent;
}


std::string SessionSettings::getUserAgent() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _userAgent;
}


void SessionSettings::setMaxRedirects(int maxRedirects)
{
    ofScopedLock lock(_mutex);
    _maxRedirects = maxRedirects;
}


int SessionSettings::getMaxRedirects() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _maxRedirects;
}


Poco::Net::NameValueCollection SessionSettings::getDefaultHeaders() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _defaultHeaders;
}


void SessionSettings::addDefaultHeader(const std::string& name,
                                       const std::string& value)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _defaultHeaders.add(name,value);
}


void SessionSettings::addDefaultHeaders(const Poco::Net::NameValueCollection& headers)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    Poco::Net::NameValueCollection::ConstIterator iter = headers.begin();
    while (iter != headers.end())
    {
        _defaultHeaders.add((*iter).first,(*iter).second);
        ++iter;
    }
}


bool SessionSettings::hasDefaultHeaders() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return !_defaultHeaders.empty();
}


bool SessionSettings::hasDefaultHeader(const std::string& name)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _defaultHeaders.has(name);
}


void SessionSettings::removeDefaultHeader(const std::string& name)
{
    ofScopedLock lock(_mutex);
    _defaultHeaders.erase(name);
}


void SessionSettings::clearDefaultHeaders()
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _defaultHeaders.clear();
}


void SessionSettings::setKeepAliveTimeout(Poco::Timespan keepAliveTimeout)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _keepAliveTimeout = keepAliveTimeout;
}


Poco::Timespan SessionSettings::getKeepAliveTimeout() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _keepAliveTimeout;
}


void SessionSettings::setProxyUsername(const std::string& username)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _proxy.setUsername(username);
    _bUseProxy = true;
}


std::string SessionSettings::getProxyUsername() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _proxy.getUsername();
}


void SessionSettings::setProxyPassword(const std::string& password)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _proxy.setPassword(password);
    _bUseProxy = true;
}


std::string SessionSettings::getProxyPassword() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _proxy.getPassword();
}


void SessionSettings::setProxyHost(const std::string& host)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _proxy.setHost(host);
    _bUseProxy = true;
}


std::string SessionSettings::getProxyHost() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _proxy.getHost();
}


uint16_t SessionSettings::getProxyPort() const
{
    ofScopedLock lock(_mutex);
    return _proxy.getPort();
}


void SessionSettings::setProxyPort(uint16_t port)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _proxy.setPort(port);
    _bUseProxy = true;
}


void SessionSettings::clearProxy()
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _proxy.clear();
    _bUseProxy = false;
}


bool SessionSettings::isProxyEnabled() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _bUseProxy;
}


bool SessionSettings::hasProxyCredentials() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _bUseProxy && _proxy.hasCredentials();
}


void SessionSettings::setUseCredentialStore(bool useCredentialStore)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _useCredentialStore = useCredentialStore;
}


bool SessionSettings::useCredentialStore() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _useCredentialStore;
}


void SessionSettings::setUseCookielStore(bool useCookieStore)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _useCookieStore = useCookieStore;
}


bool SessionSettings::useCookieStore() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _useCookieStore;
}


} } // namespace ofx::HTTP
