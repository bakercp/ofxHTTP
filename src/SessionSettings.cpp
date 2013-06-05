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


#include "SessionSettings.h"


namespace ofx {
namespace HTTP {


const std::string    DEFAULT_USER_AGENT        = "Mozilla/5.0 (compatible; Client/1.0 +https://github.com/bakercp/ofxHTTP)";
const int            DEFAULT_MAX_REDIRECTS     = 20;
const Poco::Timespan DEFAULT_KEEPALIVE_TIMEOUT = Poco::Timespan::SECONDS * 8;

//------------------------------------------------------------------------------
SessionSettings::SessionSettings() :
    _virtualHost(""),
    _defaultHost(""),
    _userAgent(DEFAULT_USER_AGENT),
    _maxRedirects(DEFAULT_MAX_REDIRECTS),
    _keepAliveTimeout(DEFAULT_KEEPALIVE_TIMEOUT),
    _bUseProxy(false),
    _proxy(ProxySettings()),
    _bUseCookieStore(true),
    _bUseCredentialStore(true)
{
}

//------------------------------------------------------------------------------
SessionSettings::SessionSettings(SessionSettings& that)
{
    ofScopedLock lock(that._mutex);

    _virtualHost         = that._virtualHost;
    _defaultHost         = that._defaultHost;
    _userAgent           = that._userAgent;
    _maxRedirects        = that._maxRedirects;
    _defaultHeaders      = that._defaultHeaders;
    _keepAliveTimeout    = that._keepAliveTimeout;
    _bUseProxy           = that._bUseProxy;
    _proxy               = that._proxy;
    _bUseCredentialStore = that._bUseCredentialStore;
    _bUseCookieStore     = that._bUseCookieStore;
    
}

//------------------------------------------------------------------------------
SessionSettings& SessionSettings::operator = (SessionSettings& that)
{
    ofScopedLock thisLock(_mutex);
    ofScopedLock thatLock(that._mutex);
    
    _virtualHost         = that._virtualHost;
    _defaultHost         = that._defaultHost;
    _userAgent           = that._userAgent;
    _maxRedirects        = that._maxRedirects;
    _defaultHeaders      = that._defaultHeaders;
    _keepAliveTimeout    = that._keepAliveTimeout;
    _bUseProxy           = that._bUseProxy;
    _proxy               = that._proxy;
    _bUseCredentialStore = that._bUseCredentialStore;
    _bUseCookieStore     = that._bUseCookieStore;
    
}

//------------------------------------------------------------------------------
SessionSettings::~SessionSettings()
{
}

//------------------------------------------------------------------------------
void SessionSettings::setVirtualHost(const std::string& virtualHost)
{
    ofScopedLock lock(_mutex);
    _virtualHost = virtualHost;
}

//------------------------------------------------------------------------------
string SessionSettings::getVirtualHost() const
{
    ofScopedLock lock(_mutex);
    return _virtualHost;
}

//------------------------------------------------------------------------------
void SessionSettings::setDefaultHost(const std::string& defaultHost)
{
    ofScopedLock lock(_mutex);
    _defaultHost = defaultHost;
}

//------------------------------------------------------------------------------
std::string SessionSettings::getDefaultHost() const
{
    ofScopedLock lock(_mutex);
    return _defaultHost;
}

//------------------------------------------------------------------------------
void SessionSettings::setUserAgent(const std::string& userAgent)
{
    ofScopedLock lock(_mutex);
    _userAgent = userAgent;
}

//------------------------------------------------------------------------------
std::string SessionSettings::getUserAgent() const
{
    ofScopedLock lock(_mutex);
    return _userAgent;
}

//------------------------------------------------------------------------------
void SessionSettings::setMaxRedirects(int maxRedirects)
{
    ofScopedLock lock(_mutex);
    _maxRedirects = maxRedirects;
}

//------------------------------------------------------------------------------
int SessionSettings::getMaxRedirects() const
{
    ofScopedLock lock(_mutex);
    return _maxRedirects;
}

//------------------------------------------------------------------------------
Poco::Net::NameValueCollection SessionSettings::getDefaultHeaders() const
{
    ofScopedLock lock(_mutex);
    return _defaultHeaders;
}

//------------------------------------------------------------------------------
void SessionSettings::addDefaultHeader(const std::string& name,
                                       const std::string& value)
{
    ofScopedLock lock(_mutex);
    _defaultHeaders.add(name,value);
}

//------------------------------------------------------------------------------
void SessionSettings::addDefaultHeaders(const Poco::Net::NameValueCollection& headers)
{
    ofScopedLock lock(_mutex);
    Poco::Net::NameValueCollection::ConstIterator iter = headers.begin();
    while(iter != headers.end()) {
        _defaultHeaders.add((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool SessionSettings::hasDefaultHeaders() const
{
    ofScopedLock lock(_mutex);
    return !_defaultHeaders.empty();
}

//------------------------------------------------------------------------------
bool SessionSettings::hasDefaultHeader(const std::string& name)
{
    ofScopedLock lock(_mutex);
    return _defaultHeaders.has(name);
}

//------------------------------------------------------------------------------
void SessionSettings::removeDefaultHeader(const std::string& name)
{
    ofScopedLock lock(_mutex);
    _defaultHeaders.erase(name);
}

//------------------------------------------------------------------------------
void SessionSettings::clearDefaultHeaders()
{
    ofScopedLock lock(_mutex);
    _defaultHeaders.clear();
}

//------------------------------------------------------------------------------
void SessionSettings::setKeepAliveTimeout(Poco::Timespan keepAliveTimeout)
{
    ofScopedLock lock(_mutex);
    _keepAliveTimeout = keepAliveTimeout;
}

//------------------------------------------------------------------------------
Poco::Timespan SessionSettings::getKeepAliveTimeout() const
{
    ofScopedLock lock(_mutex);
    return _keepAliveTimeout;
}

//------------------------------------------------------------------------------
void SessionSettings::setProxyUsername(const std::string& username)
{
    ofScopedLock lock(_mutex);
    _proxy.setUsername(username);
    _bUseProxy = true;
}

//------------------------------------------------------------------------------
string SessionSettings::getProxyUsername() const
{
    ofScopedLock lock(_mutex);
    return _proxy.getUsername();
}

//------------------------------------------------------------------------------
void SessionSettings::setProxyPassword(const string& password)
{
    ofScopedLock lock(_mutex);
    _proxy.setPassword(password);
    _bUseProxy = true;
}

//------------------------------------------------------------------------------
std::string SessionSettings::getProxyPassword() const
{
    ofScopedLock lock(_mutex);
    return _proxy.getPassword();
}

//------------------------------------------------------------------------------
void SessionSettings::setProxyHost(const std::string& host)
{
    ofScopedLock lock(_mutex);
    _proxy.setHost(host);
    _bUseProxy = true;
}

//------------------------------------------------------------------------------
std::string SessionSettings::getProxyHost() const
{
    ofScopedLock lock(_mutex);
    return _proxy.getHost();
}

//------------------------------------------------------------------------------
unsigned short SessionSettings::getProxyPort() const
{
    ofScopedLock lock(_mutex);
    return _proxy.getPort();
}

//------------------------------------------------------------------------------
void SessionSettings::setProxyPort(unsigned short port)
{
    ofScopedLock lock(_mutex);
    _proxy.setPort(port);
    _bUseProxy = true;
}

//------------------------------------------------------------------------------
void SessionSettings::clearProxy()
{
    ofScopedLock lock(_mutex);
    _proxy.clear();
    _bUseProxy = false;
}

//------------------------------------------------------------------------------
bool SessionSettings::isProxyEnabled() const
{
    ofScopedLock lock(_mutex);
    return _bUseProxy;
}

//------------------------------------------------------------------------------
bool SessionSettings::hasProxyCredentials() const
{
    ofScopedLock lock(_mutex);
    return _bUseProxy && _proxy.hasCredentials();
}

//------------------------------------------------------------------------------
void SessionSettings::setUseCredentialStore(bool bUseCredentialStore)
{
    ofScopedLock lock(_mutex);
    _bUseCredentialStore = bUseCredentialStore;
}

//------------------------------------------------------------------------------
bool SessionSettings::useCredentialStore() const
{
    ofScopedLock lock(_mutex);
    return _bUseCredentialStore;
}

//------------------------------------------------------------------------------
void SessionSettings::setUseCookielStore(bool bUseCookieStore)
{
    ofScopedLock lock(_mutex);
    _bUseCookieStore = bUseCookieStore;
}

//------------------------------------------------------------------------------
bool SessionSettings::useCookieStore() const
{
    ofScopedLock lock(_mutex);
    return _bUseCookieStore;
}


} }
