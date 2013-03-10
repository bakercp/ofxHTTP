#include "ofxHTTPSessionSettings.h"

//------------------------------------------------------------------------------
ofxHTTPSessionSettings::ofxHTTPSessionSettings() :
virtualHost(""),
defaultHost(""),
userAgent("Mozilla/5.0 (compatible; ofxHTTPClient/1.0 +https://github.com/bakercp/ofxHTTP)"),
maxRedirects(20),
keepAliveTimeout(8000),
bUseProxy(false),
proxy(ofxHTTPProxySettings()),
bUseCookieStore(true),
bUseCredentialStore(true)
{ }

//------------------------------------------------------------------------------
ofxHTTPSessionSettings::ofxHTTPSessionSettings(ofxHTTPSessionSettings& that) {
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
ofxHTTPSessionSettings& ofxHTTPSessionSettings::operator = (ofxHTTPSessionSettings& that) {
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
ofxHTTPSessionSettings::ofxHTTPSessionSettings(const ofxHTTPSessionSettings& that) { }

//------------------------------------------------------------------------------
ofxHTTPSessionSettings& ofxHTTPSessionSettings::operator = (const ofxHTTPSessionSettings& that) { }

//------------------------------------------------------------------------------
ofxHTTPSessionSettings::~ofxHTTPSessionSettings() { }

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setVirtualHost(const string& _virtualHost) {
    ofScopedLock lock(mutex);
    virtualHost = _virtualHost;
}

//------------------------------------------------------------------------------
string ofxHTTPSessionSettings::getVirtualHost() {
    ofScopedLock lock(mutex);
    return virtualHost;
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setDefaultHost(const string& _defaultHost) {
    ofScopedLock lock(mutex);
    defaultHost = _defaultHost;
}

//------------------------------------------------------------------------------
string ofxHTTPSessionSettings::getDefaultHost() {
    ofScopedLock lock(mutex);
    return defaultHost;
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setUserAgent(const string& _userAgent) {
    ofScopedLock lock(mutex);
    userAgent = _userAgent;
}

//------------------------------------------------------------------------------
string ofxHTTPSessionSettings::getUserAgent() {
    ofScopedLock lock(mutex);
    return userAgent;
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setMaxRedirects(size_t _maxRedirects) {
    ofScopedLock lock(mutex);
    maxRedirects = _maxRedirects;
}

//------------------------------------------------------------------------------
size_t ofxHTTPSessionSettings::getMaxRedirects() {
    ofScopedLock lock(mutex);
    return maxRedirects;
}

//------------------------------------------------------------------------------
NameValueCollection ofxHTTPSessionSettings::getDefaultHeaders() {
    ofScopedLock lock(mutex);
    return defaultHeaders;
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::addDefaultHeader(const string& name, const string& value) {
    ofScopedLock lock(mutex);
    defaultHeaders.add(name,value);
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::addDefaultHeaders(const NameValueCollection& _headers) {
    ofScopedLock lock(mutex);
    NameValueCollection::ConstIterator iter = _headers.begin();
    while(iter != _headers.end()) {
        defaultHeaders.add((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool ofxHTTPSessionSettings::hasDefaultHeaders() {
    ofScopedLock lock(mutex);
    return !defaultHeaders.empty();
}

//------------------------------------------------------------------------------
bool ofxHTTPSessionSettings::hasDefaultHeader(const string& name) {
    ofScopedLock lock(mutex);
    return defaultHeaders.has(name);
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::removeDefaultHeader(const string& name) {
    ofScopedLock lock(mutex);
    defaultHeaders.erase(name);
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::clearDefaultHeaders() {
    ofScopedLock lock(mutex);
    defaultHeaders.clear();
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setKeepAliveTimeout(unsigned long long milliseconds) {
    ofScopedLock lock(mutex);
    keepAliveTimeout = milliseconds;
}

//------------------------------------------------------------------------------
unsigned long long ofxHTTPSessionSettings::getKeepAliveTimeout() {
    ofScopedLock lock(mutex);
    return keepAliveTimeout;
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setProxyUsername(const string& username) {
    ofScopedLock lock(mutex);
    proxy.setUsername(username);
    bUseProxy = true;
}

//------------------------------------------------------------------------------
string ofxHTTPSessionSettings::getProxyUsername() {
    ofScopedLock lock(mutex);
    return proxy.getUsername();
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setProxyPassword(const string& password) {
    ofScopedLock lock(mutex);
    proxy.setPassword(password);
    bUseProxy = true;
}

//------------------------------------------------------------------------------
string ofxHTTPSessionSettings::getProxyPassword() {
    ofScopedLock lock(mutex);
    return proxy.getPassword();
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setProxyHost(const string& _host) {
    ofScopedLock lock(mutex);
    proxy.setHost(_host);
    bUseProxy = true;
}

//------------------------------------------------------------------------------
string ofxHTTPSessionSettings::getProxyHost() {
    ofScopedLock lock(mutex);
    return proxy.getHost();
}


//------------------------------------------------------------------------------
unsigned short ofxHTTPSessionSettings::getProxyPort() {
    ofScopedLock lock(mutex);
    return proxy.getPort();
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setProxyPort(unsigned short _port) {
    ofScopedLock lock(mutex);
    proxy.setPort(_port);
    bUseProxy = true;
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::clearProxy() {
    ofScopedLock lock(mutex);
    proxy.clear();
    bUseProxy = false;
}

//------------------------------------------------------------------------------
bool ofxHTTPSessionSettings::isProxyEnabled() {
    ofScopedLock lock(mutex);
    return bUseProxy;
}

bool ofxHTTPSessionSettings::hasProxyCredentials() {
    ofScopedLock lock(mutex);
    return bUseProxy && proxy.hasCredentials();
}


//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setUseCredentialStore(bool _bUseCredentialStore) {
    ofScopedLock lock(mutex);
    bUseCredentialStore = _bUseCredentialStore;
}

//------------------------------------------------------------------------------
bool ofxHTTPSessionSettings::useCredentialStore() {
    ofScopedLock lock(mutex);
    return bUseCredentialStore;
}

//------------------------------------------------------------------------------
void ofxHTTPSessionSettings::setUseCookielStore(bool _bUseCookieStore) {
    ofScopedLock lock(mutex);
    bUseCookieStore = _bUseCookieStore;
}

//------------------------------------------------------------------------------
bool ofxHTTPSessionSettings::useCookieStore() {
    ofScopedLock lock(mutex);
    return bUseCookieStore;
}
