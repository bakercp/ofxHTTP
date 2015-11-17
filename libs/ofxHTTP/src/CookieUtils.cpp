//// =============================================================================
////
//// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
////
//// Permission is hereby granted, free of charge, to any person obtaining a copy
//// of this software and associated documentation files (the "Software"), to deal
//// in the Software without restriction, including without limitation the rights
//// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//// copies of the Software, and to permit persons to whom the Software is
//// furnished to do so, subject to the following conditions:
////
//// The above copyright notice and this permission notice shall be included in
//// all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//// THE SOFTWARE.
////
//// =============================================================================
//
//
//#include "ofx/HTTP/CookieUtils.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
//  Poco::UTF8::icompare(contentEncoding, "use me")
//
//bool CookieUtils::equals(const std::string& thisString,
//                         const std::string& otherString)
//{
//    return 0 == thisString.compare(otherString);
//}
//
//bool CookieUtils::startsWith(const std::string& fullString,
//                             const std::string& beginning)
//{
//    return fullString.length() >= beginning.length()
//        && 0 == fullString.compare(0, beginning.length(), beginning);
//}
//
//bool CookieUtils::endsWith(const std::string& fullString,
//                           const std::string& ending)
//{
//    return fullString.length() >= ending.length()
//        && 0 == fullString.compare(fullString.length() - ending.length(),
//                               ending.length(),
//                               ending);
//}
//
//
//bool CookieUtils::pathMatches(const Poco::URI& uri,
//                              const Poco::Net::HTTPCookie& cookie)
//{
//    std::string path = uri.getPath();
//    std::string pathToMatchWith = cookie.getPath();
//
//    return equals(path, pathToMatchWith) || startsWith(path, pathToMatchWith);
//}
//
//
//bool CookieUtils::domainMatches(const Poco::URI& uri,
//                                const Cookie& cookie)
//{
//    // https://code.google.com/p/gdata-java-client/source/browse/trunk/java/src/com/google/gdata/client/http/GoogleGDataRequest.java
//
//    if(uri.getHost().empty())
//    {
//        ofLogError("Cookie::matchesDomain") << "URI host empty.";
//        return false;
//    }
//
//    if(cookie.getDomain().empty())
//    {
//        ofLogError("Cookie::matchesDomain") << "Cookie domain empty.";
//        return false;
//    }
//
//    std::string uriHost      = Poco::UTF8::toLower(uri.getHost());
//    std::string cookieDomain = Poco::UTF8::toLower(cookie.getDomain());
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
//}
//
//bool CookieUtils::compare(const Cookie& a,
//                          const Cookie& b)
//{
//    // names must be equal
//    if (!equals(a.getName(), b.getName()))
//    {
//        return false;
//    }
//    else if (startsWith(a.getPath(), b.getPath()))
//    {
//        return true;
//    }
//    else if (startsWith(b.getPath(), a.getPath()))
//    {
//        return false;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//
//void CookieUtils::sortByPath(std::vector<Cookie>& cookies)
//{
//    std::sort(cookies.begin(), cookies.end(), compare);
//}
//
//
//bool CookieUtils::hasExpired(const Cookie& cookie)
//{
//    int maxAge = cookie.getMaxAge();
//
//    if (0 == maxAge)
//    {
//        return true;
//    }
//
//    if (0 > maxAge)
//    {
//        return false;
//    }
//
//    Poco::Timestamp currentTime;
//
//    
//
//
//}
//
////        235         long deltaSecond = (System.currentTimeMillis() - whenCreated) / 1000;
////        236         if (deltaSecond > maxAge)
////            237             return true;
////        238         else
////            239             return false;
////        240     }
//
//
//} } // namespace ofx::HTTP
