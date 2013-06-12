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


#include "Cookie.h"


namespace ofx {
namespace HTTP {
        

//------------------------------------------------------------------------------
Cookie::Cookie(const Poco::Net::HTTPCookie& cookie) :
    _cookie(cookie),
    _createdAt(ofGetSystemTime())
{
}

//------------------------------------------------------------------------------
Cookie::Cookie(const Poco::Net::HTTPCookie& cookie,
               unsigned long long createdAt) :
    _cookie(cookie),
    _createdAt(createdAt)
{
}

//------------------------------------------------------------------------------
Cookie::~Cookie()
{
}

//------------------------------------------------------------------------------
unsigned long long Cookie::getCreatedAt() const {
    return _createdAt;
}

//------------------------------------------------------------------------------
bool Cookie::isExpired(unsigned long long expiredAt) const {
    int maxAge = _cookie.getMaxAge();

    return maxAge >= 0 && expiredAt > ( _createdAt + ( maxAge / 1000 ) );
}

//------------------------------------------------------------------------------
bool Cookie::isSession() const {
    // based on Poco::Net::HTTPCookie
    // if max age == -1, persistant, expiry not set, so a session cookie
    // if max age == 0, then it will be deleted immediately
    // if max age >  0, then it will expire sometime in the future
    return _cookie.getMaxAge() < 0;
}

//------------------------------------------------------------------------------
bool Cookie::matchesDomain(const Poco::URI& uri) const {
    // https://code.google.com/p/gdata-java-client/source/browse/trunk/java/src/com/google/gdata/client/http/GoogleGDataRequest.java
    
    if(uri.getHost().empty()) {
        ofLogError("ofxCookieStore::matchCookieDomain") << "URI host empty.";
        return false;
    }
    
    if(_cookie.getDomain().empty()) {
        ofLogError("ofxCookieStore::matchCookieDomain") << "Cookie domain empty.";
        return false;
    }
    
    std::string uriHost      = Poco::toLower(uri.getHost());
    std::string cookieDomain = Poco::toLower(_cookie.getDomain());
    
    // simple check
    if(!endsWith(uriHost,cookieDomain)) {
        return false;
    }
    
    // exact match
    if(uriHost.length() == cookieDomain.length()) {
        return true;
    }
    
    // Verify that a segment match happened, not a partial match
    if (cookieDomain[0] == '.') {
        return true;
    }

    return uriHost[uriHost.length() - cookieDomain.length() - 1] == '.';
    
}

//------------------------------------------------------------------------------
bool Cookie::matchesPath(const Poco::URI& uri) const {
    return uri.getPath().find(_cookie.getPath()) == 0;
}

//------------------------------------------------------------------------------
bool Cookie::matchesURI(const Poco::URI& uri, bool matchSessionCookies) const {

    // double check scheme
    if(uri.getScheme() != "http" && uri.getScheme() != "https") {
        return false;
    }
    
    // check 
    if(_cookie.getSecure() && uri.getScheme() != "https") {
        return false;
    }
    
    if(isExpired()) {
        return false;
    }
    
    if(!matchSessionCookies && isSession()) {
        return false;
    }
    
    if(!matchesDomain(uri)) {
        return false;
    }
    
    if(!matchesPath(uri)) {
        return false;
    }
    
    return true;
}

//------------------------------------------------------------------------------
bool Cookie::matches(const Cookie& other) const {
    bool isMatch = (_cookie.getName() == other.getCookie().getName());
        
    if (isMatch) {
        // do not differentiate empty and null domains
        std::string thisDomain = _cookie.getDomain();
        
        if(thisDomain.find( "." ) == std::string::npos) {
            thisDomain += ".local";
        }
        
        std::string thatDomain = other._cookie.getDomain();
        
        if(thatDomain.find( "." ) == std::string::npos) {
            thatDomain += ".local";
        }
        
        isMatch = (Poco::icompare(thisDomain,thatDomain) == 0);
    }
    
    if (isMatch) {
        isMatch = (_cookie.getPath() == other._cookie.getPath());
    }
    
    return isMatch;

}


//------------------------------------------------------------------------------
const Poco::Net::HTTPCookie& Cookie::getCookie() const {
    return _cookie;
}

//------------------------------------------------------------------------------
std::string Cookie::toString() const {
    std::stringstream ss;
    ss << _cookie.toString() << " Created @ " << _createdAt;
    return ss.str();
}

//------------------------------------------------------------------------------
bool Cookie::endsWith(const std::string& fullString, const std::string& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(),
                                        ending.length(),
                                        ending));
    } else {
        return false;
    }
}


} }
