#include "ofxHTTPCookieStore.h"

//------------------------------------------------------------------------------
ofxHTTPCookieStore::ofxHTTPCookieStore() { }

//------------------------------------------------------------------------------
ofxHTTPCookieStore::ofxHTTPCookieStore(ofxHTTPCookieStore& that) {
    ofScopedLock thatLock(that.mutex);
    cookies = that.cookies;
}

//------------------------------------------------------------------------------
ofxHTTPCookieStore& ofxHTTPCookieStore::operator = (ofxHTTPCookieStore& that) {
    ofScopedLock thisLock(mutex);
    ofScopedLock thatLock(that.mutex);
    cookies = that.cookies;
    return *this;
}

//------------------------------------------------------------------------------
ofxHTTPCookieStore::ofxHTTPCookieStore(const ofxHTTPCookieStore& that) { }

//------------------------------------------------------------------------------
ofxHTTPCookieStore& ofxHTTPCookieStore::operator = (const ofxHTTPCookieStore& that) { 
    /* can't do this b/c no const copy for mutex */
    /* 
    ofScopedLock thisLock(mutex);
    ofScopedLock thatLock(that.mutex);
    cookies = that.cookies;
    return *this;
    */;
}

//------------------------------------------------------------------------------
ofxHTTPCookieStore::~ofxHTTPCookieStore() { }

//------------------------------------------------------------------------------
vector<ofxHTTPCookie> ofxHTTPCookieStore::getCookesForURI(const URI& uri, bool matchSessionCookes) {
    // TODO: this needs to be a bit more sophisticated
    // http://stackoverflow.com/questions/4056306/how-to-handle-multiple-cookies-with-the-same-name
    
    ofScopedLock lock(mutex);
    
    vector<ofxHTTPCookie> matchingCookies;
    
    vector<ofxHTTPCookie>::const_iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        ofxHTTPCookie cookie = (*iter);
        if(cookie.matchesURI(uri,matchSessionCookes)) {
            matchingCookies.push_back(cookie);
        }
        ++iter;
    }
    
    return matchingCookies;
}

//------------------------------------------------------------------------------
void ofxHTTPCookieStore::store(const HTTPResponse& response) {
    vector<HTTPCookie> newCookies;
    response.getCookies(newCookies);

    ofScopedLock lock(mutex);
    vector<HTTPCookie>::iterator iter = newCookies.begin();
    
    while(iter != newCookies.end()) {
        addCookieWithExistingLock(ofxHTTPCookie(*iter));
        ++iter;
    }
}

//------------------------------------------------------------------------------
void ofxHTTPCookieStore::addCookie(const string& name, const string& value, bool isValueEscaped) {
    addCookie(HTTPCookie(name, isValueEscaped ? value : HTTPCookie::escape(value)));
}

//------------------------------------------------------------------------------
void ofxHTTPCookieStore::addCookie(const HTTPCookie& cookie) {
    addCookie(ofxHTTPCookie(cookie));
}

//------------------------------------------------------------------------------
void ofxHTTPCookieStore::addCookie(const ofxHTTPCookie& cookie) {
    ofScopedLock lock(mutex);
    addCookieWithExistingLock(cookie);
}

//------------------------------------------------------------------------------
void ofxHTTPCookieStore::addCookieWithExistingLock(const ofxHTTPCookie& cookie) {
    if(cookie.isExpired()) return;
   
    // did clear needed? 
    //bool didClear = false; 
    vector<ofxHTTPCookie>::iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        if((*iter).matches(cookie)) {
            iter = cookies.erase(iter);
            //didClear = true;
        } else {
            ++iter;
        }
    }
    
    if(!cookie.isExpired()) {
        cookies.push_back(cookie.getCookie());
    }
}

//------------------------------------------------------------------------------
bool ofxHTTPCookieStore::clearExpired(unsigned long long expiredAt) {
    ofScopedLock lock(mutex);

    bool didClear = false;
    vector<ofxHTTPCookie>::iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        ofxHTTPCookie cookie = (*iter);
        if(cookie.isExpired(expiredAt)) {
            iter = cookies.erase(iter);
            didClear = true;
        } else {
            ++iter;
        }
    }
    return didClear;
}

//------------------------------------------------------------------------------
bool ofxHTTPCookieStore::clearCookiesWithName(const string& name) {
    ofScopedLock lock(mutex);
    
    bool didClear = false;
    vector<ofxHTTPCookie>::iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        ofxHTTPCookie cookie = (*iter);
        if(cookie.getCookie().getName() == name) {
            iter = cookies.erase(iter);
            didClear = true;
        } else {
            ++iter;
        }
    }
    return didClear;
}

//------------------------------------------------------------------------------
bool ofxHTTPCookieStore::clearCookiesWithDomain(const string& domain) {
    ofScopedLock lock(mutex);
    
    bool didClear = false;
    vector<ofxHTTPCookie>::iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        ofxHTTPCookie cookie = (*iter);
        if(cookie.getCookie().getDomain() == domain) {
            iter = cookies.erase(iter);
            didClear = true;
        } else {
            ++iter;
        }
    }
    return didClear;
}

//------------------------------------------------------------------------------
bool ofxHTTPCookieStore::clearCookiesWithPath(const string& path) {
    ofScopedLock lock(mutex);
    
    bool didClear = false;
    vector<ofxHTTPCookie>::iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        ofxHTTPCookie cookie = (*iter);
        if(cookie.getCookie().getPath() == path) {
            iter = cookies.erase(iter);
            didClear = true;
        } else {
            ++iter;
        }
    }
    return didClear;
}

//------------------------------------------------------------------------------
void ofxHTTPCookieStore::clear() {
    ofScopedLock lock(mutex);

    cookies.clear();
}
