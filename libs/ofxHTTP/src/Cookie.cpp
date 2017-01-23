//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/Cookie.h"


namespace ofx {
namespace HTTP {
        


Cookie::Cookie(const Poco::Net::HTTPCookie& cookie):
    Poco::Net::HTTPCookie(cookie),
    _createdAt(Poco::Timestamp())
{
}


Cookie::Cookie(const Poco::Net::HTTPCookie& cookie,
               Poco::Timestamp createdAt):
    Poco::Net::HTTPCookie(cookie),
    _createdAt(createdAt)
{
}


Cookie::~Cookie()
{
}


Poco::Timestamp Cookie::createdAt() const
{
    return _createdAt;
}


//bool Cookie::isExpired(Poco::Timestamp expiredAt) const
//{
//    int maxAge = getMaxAge();
//
//    return maxAge >= 0 && expiredAt > ( _createdAt + ( maxAge / 1000 ) );
//}
//
//
//bool Cookie::isSession() const
//{
//    // based on Poco::Net::HTTPCookie
//    // if max age == -1, persistant, expiry not set, so a session cookie
//    // if max age == 0, then it will be deleted immediately
//    // if max age >  0, then it will expire sometime in the future
//    return getMaxAge() < 0;
//}
//
//
//bool Cookie::matchesDomain(const Poco::URI& uri) const
//{
//    // https://code.google.com/p/gdata-java-client/source/browse/trunk/java/src/com/google/gdata/client/http/GoogleGDataRequest.java
//    
//    if(uri.getHost().empty())
//    {
//        ofLogError("Cookie::matchesDomain") << "URI host empty.";
//        return false;
//    }
//    
//    if(getDomain().empty())
//    {
//        ofLogError("Cookie::matchesDomain") << "Cookie domain empty.";
//        return false;
//    }
//    
//    std::string uriHost      = Poco::UTF8::toLower(uri.getHost());
//    std::string cookieDomain = Poco::UTF8::toLower(getDomain());
//    
//    // simple check
//    if(!endsWith(uriHost, cookieDomain))
//    {
//        return false;
//    }
//    
//    // exact match
//    if(uriHost.length() == cookieDomain.length())
//    {
//        return true;
//    }
//    
//    // Verify that a segment match happened, not a partial match
//    if (cookieDomain[0] == '.')
//    {
//        return true;
//    }
//
//    return uriHost[uriHost.length() - cookieDomain.length() - 1] == '.';
//    
//}


//bool Cookie::matchesPath(const Poco::URI& uri) const
//{
//    // path-matches algorithm, as defined by RFC 2965
//    return uri.getPath().find(getPath()) == 0;
//}
//
//
//bool Cookie::matchesURI(const Poco::URI& uri, bool matchSessionCookies) const
//{
//
//    // double check scheme
//    if(uri.getScheme() != "http" && uri.getScheme() != "https") return false;
//
//    if(getSecure() && uri.getScheme() != "https") return false;
//
//    if(isExpired()) return false;
//    
//    if(!matchSessionCookies && isSession()) return false;
//
//    if(!matchesDomain(uri)) return false;
//
//    if(!matchesPath(uri)) return false;
//
//    return true;
//}


//bool Cookie::matches(const Cookie& other) const
//{
//    bool isMatch = (getName() == other.getName());
//        
//    if(isMatch)
//    {
//        // do not differentiate empty and null domains
//        std::string thisDomain = getDomain();
//        
//        if(thisDomain.find( "." ) == std::string::npos)
//        {
//            thisDomain += ".local";
//        }
//        
//        std::string thatDomain = other.getDomain();
//        
//        if(thatDomain.find( "." ) == std::string::npos)
//        {
//            thatDomain += ".local";
//        }
//        
//        isMatch = (Poco::icompare(thisDomain,thatDomain) == 0);
//    }
//    
//    if (isMatch)
//    {
//        isMatch = (getPath() == other.getPath());
//    }
//    
//    return isMatch;
//}



//std::string Cookie::toString() const
//{
//    std::stringstream ss;
//    ss << Poco::Net::HTTPCookie::toString() << " Created @ " << _createdAt.elapsed();
//    return ss.str();
//}
//
//
//bool Cookie::endsWith(const std::string& fullString, const std::string& ending)
//{
//    if (fullString.length() >= ending.length())
//    {
//        return (0 == fullString.compare(fullString.length() - ending.length(),
//                                        ending.length(),
//                                        ending));
//    }
//    else
//    {
//        return false;
//    }
//}


} } // namespace ofx::HTTP
