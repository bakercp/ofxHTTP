#include "ofxHTTPCookie.h"


//------------------------------------------------------------------------------
ofxHTTPCookie::ofxHTTPCookie(const HTTPCookie& _cookie, unsigned long long _createdAt) :
cookie(_cookie),
createdAt(_createdAt)
{
}

//------------------------------------------------------------------------------
unsigned long long ofxHTTPCookie::getCreatedAt() const {
    return createdAt;
}

//------------------------------------------------------------------------------
bool ofxHTTPCookie::isExpired(unsigned long long expiredAt) const {
    int maxAge = cookie.getMaxAge();

    return maxAge >= 0 && expiredAt > ( createdAt + ( maxAge / 1000 ) );
}

//------------------------------------------------------------------------------
bool ofxHTTPCookie::isSession() const {
    // based on Poco::Net::HTTPCookie
    // if max age == -1, persistant, expiry not set, so a session cookie
    // if max age == 0, then it will be deleted immediately
    // if max age >  0, then it will expire sometime in the future
    return cookie.getMaxAge() < 0;
}

//------------------------------------------------------------------------------
bool ofxHTTPCookie::matchesDomain(const URI& uri) const {
    // https://code.google.com/p/gdata-java-client/source/browse/trunk/java/src/com/google/gdata/client/http/GoogleGDataRequest.java
    
    if(uri.getHost().empty()) {
        ofLogError("ofxCookieStore::matchCookieDomain") << "URI host empty.";
        return false;
    }
    
    if(cookie.getDomain().empty()) {
        ofLogError("ofxCookieStore::matchCookieDomain") << "Cookie domain empty.";
        return false;
    }
    
    string uriHost      = toLower(uri.getHost());
    string cookieDomain = toLower(cookie.getDomain());
    
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
bool ofxHTTPCookie::matchesPath(const URI& uri) const {
    return uri.getPath().find(cookie.getPath()) == 0;
}

//------------------------------------------------------------------------------
bool ofxHTTPCookie::matchesURI(const URI& uri, bool matchSessionCookies) const {

    // double check scheme
    if(uri.getScheme() != "http" && uri.getScheme() != "https") {
        return false;
    }
    
    // check 
    if(cookie.getSecure() && uri.getScheme() != "https") {
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

bool ofxHTTPCookie::matches(const ofxHTTPCookie& other) const {
    bool isMatch = cookie.getName() == other.getCookie().getName();
        
    if (isMatch) {
        // do not differentiate empty and null domains
        string thisDomain = cookie.getDomain();
        
        if(thisDomain.find( "." ) == string::npos) {
            thisDomain += ".local";
        }
        
        string thatDomain = other.cookie.getDomain();
        
        if(thatDomain.find( "." ) == string::npos) {
            thatDomain += ".local";
        }
        
        isMatch = (icompare(thisDomain,thatDomain) == 0);
    }
    
    if (isMatch) {
        isMatch = (cookie.getPath() == other.cookie.getPath());
    }
    
    return isMatch;

}


//------------------------------------------------------------------------------
const HTTPCookie& ofxHTTPCookie::getCookie() const {
    return cookie;
}

//------------------------------------------------------------------------------
string ofxHTTPCookie::toString() const {
    stringstream ss;
    ss << cookie.toString() << " Created @ " << createdAt;
    return ss.str();
}

//------------------------------------------------------------------------------
bool ofxHTTPCookie::endsWith(string const& fullString, string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(),
                                        ending.length(),
                                        ending));
    } else {
        return false;
    }
}

