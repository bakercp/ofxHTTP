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


//------------------------------------------------------------------------------
SessionSettings::SessionSettings()
: virtualHost("")
, defaultHost("")
, userAgent("Mozilla/5.0 (compatible; Client/1.0 +https://github.com/bakercp/ofxHTTP)")
, maxRedirects(20)
, keepAliveTimeout(8000)
, bUseProxy(false)
, proxy(ProxySettings())
, bUseCookieStore(true)
, bUseCredentialStore(true)
{

}

//------------------------------------------------------------------------------
SessionSettings::SessionSettings(SessionSettings& that) {
    ofScopedLock lock(that.mutex);

    virtualHost         = that.virtualHost;
    defaultHost         = that.defaultHost;
    userAgent           = that.userAgent;
    maxRedirects        = that.maxRedirects;
    defaultHeaders      = that.defaultHeaders;
    keepAliveTimeout    = that.keepAliveTimeout;
    bUseProxy           = that.bUseProxy;
    proxy               = that.proxy;
    bUseCredentialStore = that.bUseCredentialStore;
    bUseCookieStore     = that.bUseCookieStore;
    
}

//------------------------------------------------------------------------------
SessionSettings& SessionSettings::operator = (SessionSettings& that) {
    ofScopedLock thisLock(mutex);
    ofScopedLock thatLock(that.mutex);
    
    virtualHost         = that.virtualHost;
    defaultHost         = that.defaultHost;
    userAgent           = that.userAgent;
    maxRedirects        = that.maxRedirects;
    defaultHeaders      = that.defaultHeaders;
    keepAliveTimeout    = that.keepAliveTimeout;
    bUseProxy           = that.bUseProxy;
    proxy               = that.proxy;
    bUseCredentialStore = that.bUseCredentialStore;
    bUseCookieStore     = that.bUseCookieStore;
    
}

//------------------------------------------------------------------------------
SessionSettings::SessionSettings(const SessionSettings& that) { }

//------------------------------------------------------------------------------
SessionSettings& SessionSettings::operator = (const SessionSettings& that) { }

//------------------------------------------------------------------------------
SessionSettings::~SessionSettings() { }

//------------------------------------------------------------------------------
void SessionSettings::setVirtualHost(const string& _virtualHost) {
    ofScopedLock lock(mutex);
    virtualHost = _virtualHost;
}

//------------------------------------------------------------------------------
string SessionSettings::getVirtualHost() {
    ofScopedLock lock(mutex);
    return virtualHost;
}

//------------------------------------------------------------------------------
void SessionSettings::setDefaultHost(const string& _defaultHost) {
    ofScopedLock lock(mutex);
    defaultHost = _defaultHost;
}

//------------------------------------------------------------------------------
string SessionSettings::getDefaultHost() {
    ofScopedLock lock(mutex);
    return defaultHost;
}

//------------------------------------------------------------------------------
void SessionSettings::setUserAgent(const string& _userAgent) {
    ofScopedLock lock(mutex);
    userAgent = _userAgent;
}

//------------------------------------------------------------------------------
string SessionSettings::getUserAgent() {
    ofScopedLock lock(mutex);
    return userAgent;
}

//------------------------------------------------------------------------------
void SessionSettings::setMaxRedirects(int _maxRedirects) {
    ofScopedLock lock(mutex);
    maxRedirects = _maxRedirects;
}

//------------------------------------------------------------------------------
int SessionSettings::getMaxRedirects() {
    ofScopedLock lock(mutex);
    return maxRedirects;
}

//------------------------------------------------------------------------------
Poco::Net::NameValueCollection SessionSettings::getDefaultHeaders() {
    ofScopedLock lock(mutex);
    return defaultHeaders;
}

//------------------------------------------------------------------------------
void SessionSettings::addDefaultHeader(const string& name, const string& value) {
    ofScopedLock lock(mutex);
    defaultHeaders.add(name,value);
}

//------------------------------------------------------------------------------
void SessionSettings::addDefaultHeaders(const Poco::Net::NameValueCollection& _headers) {
    ofScopedLock lock(mutex);
    Poco::Net::NameValueCollection::ConstIterator iter = _headers.begin();
    while(iter != _headers.end()) {
        defaultHeaders.add((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool SessionSettings::hasDefaultHeaders() {
    ofScopedLock lock(mutex);
    return !defaultHeaders.empty();
}

//------------------------------------------------------------------------------
bool SessionSettings::hasDefaultHeader(const string& name) {
    ofScopedLock lock(mutex);
    return defaultHeaders.has(name);
}

//------------------------------------------------------------------------------
void SessionSettings::removeDefaultHeader(const string& name) {
    ofScopedLock lock(mutex);
    defaultHeaders.erase(name);
}

//------------------------------------------------------------------------------
void SessionSettings::clearDefaultHeaders() {
    ofScopedLock lock(mutex);
    defaultHeaders.clear();
}

//------------------------------------------------------------------------------
void SessionSettings::setKeepAliveTimeout(unsigned long long milliseconds) {
    ofScopedLock lock(mutex);
    keepAliveTimeout = milliseconds;
}

//------------------------------------------------------------------------------
unsigned long long SessionSettings::getKeepAliveTimeout() {
    ofScopedLock lock(mutex);
    return keepAliveTimeout;
}

//------------------------------------------------------------------------------
void SessionSettings::setProxyUsername(const string& username) {
    ofScopedLock lock(mutex);
    proxy.setUsername(username);
    bUseProxy = true;
}

//------------------------------------------------------------------------------
string SessionSettings::getProxyUsername() {
    ofScopedLock lock(mutex);
    return proxy.getUsername();
}

//------------------------------------------------------------------------------
void SessionSettings::setProxyPassword(const string& password) {
    ofScopedLock lock(mutex);
    proxy.setPassword(password);
    bUseProxy = true;
}

//------------------------------------------------------------------------------
string SessionSettings::getProxyPassword() {
    ofScopedLock lock(mutex);
    return proxy.getPassword();
}

//------------------------------------------------------------------------------
void SessionSettings::setProxyHost(const string& _host) {
    ofScopedLock lock(mutex);
    proxy.setHost(_host);
    bUseProxy = true;
}

//------------------------------------------------------------------------------
string SessionSettings::getProxyHost() {
    ofScopedLock lock(mutex);
    return proxy.getHost();
}

//------------------------------------------------------------------------------
unsigned short SessionSettings::getProxyPort() {
    ofScopedLock lock(mutex);
    return proxy.getPort();
}

//------------------------------------------------------------------------------
void SessionSettings::setProxyPort(unsigned short _port) {
    ofScopedLock lock(mutex);
    proxy.setPort(_port);
    bUseProxy = true;
}

//------------------------------------------------------------------------------
void SessionSettings::clearProxy() {
    ofScopedLock lock(mutex);
    proxy.clear();
    bUseProxy = false;
}

//------------------------------------------------------------------------------
bool SessionSettings::isProxyEnabled() {
    ofScopedLock lock(mutex);
    return bUseProxy;
}

//------------------------------------------------------------------------------
bool SessionSettings::hasProxyCredentials() {
    ofScopedLock lock(mutex);
    return bUseProxy && proxy.hasCredentials();
}

//------------------------------------------------------------------------------
void SessionSettings::setUseCredentialStore(bool _bUseCredentialStore) {
    ofScopedLock lock(mutex);
    bUseCredentialStore = _bUseCredentialStore;
}

//------------------------------------------------------------------------------
bool SessionSettings::useCredentialStore() {
    ofScopedLock lock(mutex);
    return bUseCredentialStore;
}

//------------------------------------------------------------------------------
void SessionSettings::setUseCookielStore(bool _bUseCookieStore) {
    ofScopedLock lock(mutex);
    bUseCookieStore = _bUseCookieStore;
}

//------------------------------------------------------------------------------
bool SessionSettings::useCookieStore() {
    ofScopedLock lock(mutex);
    return bUseCookieStore;
}


} }
