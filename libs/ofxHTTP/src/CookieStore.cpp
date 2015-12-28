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
//#include "ofx/HTTP/CookieStore.h"
//#include "ofx/HTTP/Utils.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
//CookieStore::CookieStore(const AbstractCookiePolicy& policy):
//    _policy(policy)
//{
//}
//
//CookieStore::~CookieStore()
//{
//}
//
//// Add one cookie into cookie store.
//void CookieStore::add(const Poco::URI& uri, const Poco::Net::HTTPCookie& cookie)
//{
//    if (!_policy.shouldAccept(uri, cookie))
//    {
//        return false;
//    }
//
//    // remove old cookie
//    // cookieJar.remove(cookie);
//
//    if (0 != cookie.getMaxAge())
//    {
//        _cookies.push_back(cookie);
//
//        if (!cookie.getDomain().empty())
//        {
//            CookieIndex::iterator iter = _domainIndex.find(cookie.getDomain());
//
//            if (iter != _domainIndex.end())
//            {
//                // remove old cookie
//                // cookieJar.remove(cookie);
//                iter->second.push_back(cookie);
//            }
//            else
//            {
//                Cookies cookies;
//                cookies.push_back(cookie);
//                _domainIndex[cookie.getDomain()] = cookies;
//            }
//        }
//
//        CookieIndex::iterator iter = _uriIndex.find(uri.getHost());
//
//        if (iter != _uriIndex.end())
//        {
//            // remove old cookie
//            // cookieJar.remove(cookie);
//            iter->second.push_back(cookie);
//        }
//        else
//        {
//            Cookies cookies;
//            cookies.push_back(cookie);
//            _uriIndex[uri.getHost()] = cookies;
//        }
//    }
//}
//
//
//std::vector<Cookie> CookieStore::getFromDomainIndex(const Poco::URI& uri)
//{
//
//
//    bool isSecure = (uri.getScheme() == "https");
//    const std::string& host = uri.getHost();
//
//    Cookies matchingCookies;
//    Cookies toRemove;
//
//    Poco::FastMutex::ScopedLock lock(mutex);
//
//    CookieIndex::iterator indexIter = _domainIndex.begin();
//
//    while (indexIter != _domainIndex.end())
//    {
//        const std::string& domain = indexIter->first;
//        const Cookies& cookiesForDomain = indexIter->second;
//
//        Cookies::const_iterator cookieIter = cookiesForDomain.begin();
//
//        while (cookieIter != cookiesForDomain.end())
//        {
//            const Cookie& cookie = *cookieIter;
//
//            if ((0 == cookie.getVersion() && netscapeDomainMatches(domain, host))
//                ||
//
//            ++cookieIter;
//        }
//
//        ++indexIter;
//    }
//    
//}
//
//
////                222    /*
////                        223     * This is almost the same as HttpCookie.domainMatches except for
////                        224     * one difference: It won't reject cookies when the 'H' part of the
////                        225     * domain contains a dot ('.').
////                        226     * I.E.: RFC 2965 section 3.3.2 says that if host is x.y.domain.com
////                        227     * and the cookie domain is .domain.com, then it should be rejected.
////                        228     * However that's not how the real world works. Browsers don't reject and
////                        229     * some sites, like yahoo.com do actually expect these cookies to be
////                        230     * passed along.
////                        231     * And should be used for 'old' style cookies (aka Netscape type of cookies)
////                        232     */
////                233    private boolean More ...netscapeDomainMatches(String domain, String host)
////                234    {
////                    235        if (domain == null || host == null) {
////                        236            return false;
////                        237        }
////                    238
////                    239        // if there's no embedded dot in domain and domain is not .local
////                    240        boolean isLocalDomain = ".local".equalsIgnoreCase(domain);
////                    241        int embeddedDotInDomain = domain.indexOf('.');
////                    242        if (embeddedDotInDomain == 0) {
////                        243            embeddedDotInDomain = domain.indexOf('.', 1);
////                        244        }
////                    245        if (!isLocalDomain && (embeddedDotInDomain == -1 || embeddedDotInDomain == domain.length() - 1)) {
////                        246            return false;
////                        247        }
////                    248
////                    249        // if the host name contains no dot and the domain name is .local
////                    250        int firstDotInHost = host.indexOf('.');
////                    251        if (firstDotInHost == -1 && isLocalDomain) {
////                        252            return true;
////                        253        }
////                    254
////                    255        int domainLength = domain.length();
////                    256        int lengthDiff = host.length() - domainLength;
////                    257        if (lengthDiff == 0) {
////                        258            // if the host name and the domain name are just string-compare euqal
////                        259            return host.equalsIgnoreCase(domain);
////                        260        } else if (lengthDiff > 0) {
////                            261            // need to check H & D component
////                            262            String H = host.substring(0, lengthDiff);
////                            263            String D = host.substring(lengthDiff);
////                            264
////                            265            return (D.equalsIgnoreCase(domain));
////                            266        } else if (lengthDiff == -1) {
////                                267            // if domain is actually .host
////                                268            return (domain.charAt(0) == '.' &&
////                                                       269                    host.equalsIgnoreCase(domain.substring(1)));
////                                270        }
////                    271
////                    272        return false;
////                    273    }
////                274
//
//
//
////    280        for (Map.Entry<String, List<HttpCookie>> entry : cookieIndex.entrySet()) {
////            281            String domain = entry.getKey();
////            282            List<HttpCookie> lst = entry.getValue();
////            283            for (HttpCookie c : lst) {
////                284                if ((c.getVersion() == 0 && netscapeDomainMatches(domain, host)) ||
////                                       285                        (c.getVersion() == 1 && HttpCookie.domainMatches(domain, host))) {
////                    286                    if ((cookieJar.indexOf(c) != -1)) {
////                        287                        // the cookie still in main cookie store
////                        288                        if (!c.hasExpired()) {
////                            289                            // don't add twice and make sure it's the proper
////                            290                            // security level
////                            291                            if ((secureLink || !c.getSecure()) &&
////                                                               292                                    !cookies.contains(c)) {
////                                293                                cookies.add(c);
////                                294                            }
////                            295                        } else {
////                                296                            toRemove.add(c);
////                                297                        }
////                        298                    } else {
////                            299                        // the cookie has beed removed from main store,
////                            300                        // so also remove it from domain indexed store
////                            301                        toRemove.add(c);
////                            302                    }
////                    303                }
////                304            }
////            305            // Clear up the cookies that need to be removed
////            306            for (HttpCookie c : toRemove) {
////                307                lst.remove(c);
////                308                cookieJar.remove(c);
////                309
////                310            }
////            311            toRemove.clear();
////            312        }
////        313    }
//
//
//
////            120            // check domainIndex first
////            121            getInternal1(cookies, domainIndex, uri.getHost(), secureLink);
////            122            // check uriIndex then
////            123            getInternal2(cookies, uriIndex, getEffectiveURI(uri), secureLink);
////        128        return cookies;
////        129    }
//
////std::vector<Cookie> CookieStore::get(const Poco::URI& uri) const
////{
////    // TODO: this needs to be a bit more sophisticated
////    // http://stackoverflow.com/questions/4056306/how-to-handle-multiple-cookies-with-the-same-name
////
////    Poco::FastMutex::ScopedLock lock(mutex);
////
////    Cookies matchingCookies;
////
////    Cookies::const_iterator iter = _cookies.begin();
////
////    while (iter != _cookies.end())
////    {
////        const Poco::Net::HTTPCookie& cookie = *iter;
////
////        // apply path-matches rule (RFC 2965 sec. 3.3.4)
////        if (CookieUtils::pathMatches(uri, cookie))
////        {
////            matchingCookies.push_back((*iter));
////        }
////
////        ++iter;
////    }
////
////    // apply sort rule (RFC 2965 sec. 3.3.4)
////    CookieUtils::sortByPath(matchingCookies);
////
////    return matchingCookies;
////}
//
//
////void CookieStore::set(Poco::Net::HTTPRequest& request)
////{
////    Poco::URI uri(request.getURI());
////
////    CookieStore::Cookies cookies =  get(uri);
////
////    CookieStore::CookiesConstIter iter = cookies.begin();
////
////    Poco::Net::NameValueCollection collection;
////
////    while (iter != cookies.end())
////    {
////        collection.add((*iter).getName(), (*iter).getValue());
////        ++iter;
////    }
////
////    request.setCookies(collection);
////}
//
//
////void CookieStore::store(const Poco::Net::HTTPRequest& request,
////                        const Poco::Net::HTTPResponse& response)
////{
////    cout << "storing this!" << endl;
////
////    Utils::dumpHeaders(response);
////
////    std::vector<HTTPCookie> newCookies;
////    response.getCookies(newCookies);
////
////    Poco::FastMutex::ScopedLock lock(mutex);
////    std::vector<HTTPCookie>::iterator iter = newCookies.begin();
////    
////    while (iter != newCookies.end())
////    {
////        Cookie cookie(*iter);
////
////        cookie.setDomain(request.getHost());
////
////        cout << cookie.toString() << endl;
////
////        addCookieWithExistingLock(cookie);
////        ++iter;
////    }
////}
//
//
////void CookieStore::addCookie(const std::string& name,
////                            const std::string& value,
////                            bool isValueEscaped)
////{
////    std::string escapedValue = value;
////    
////    if(!isValueEscaped)
////    {
////        escapedValue = HTTPCookie::escape(value);
////    }
////
////    addCookie(HTTPCookie(name, escapedValue));
////}
////
////
////void CookieStore::addCookie(const HTTPCookie& cookie)
////{
////    addCookie(Cookie(cookie));
////}
////
////
////void CookieStore::addCookie(const Cookie& cookie)
////{
////    Poco::FastMutex::ScopedLock lock(mutex);
////    addCookieWithExistingLock(cookie);
////}
////
////
////void CookieStore::addCookieWithExistingLock(const Cookie& cookie)
////{
////    if(cookie.isExpired()) return;
////   
////    // did clear needed? 
////    //bool didClear = false; 
////    CookiesIter iter = cookies.begin();
////    while (iter != cookies.end())
////    {
////        ofLogVerbose("CookieStore::addCookieWithExistingLock") << "Adding cookie: " << (*iter).toString();
////
////        if((*iter).matches(cookie))
////        {
////            iter = cookies.erase(iter);
////            //didClear = true;
////        }
////        else {
////            ++iter;
////        }
////    }
////    
////    if(!cookie.isExpired())
////    {
////        cookies.push_back(cookie);
////    }
////}
////
////
////bool CookieStore::clearExpired(uint64_t expiredAt)
////{
////    Poco::FastMutex::ScopedLock lock(mutex);
////
////    bool didClear = false;
////    CookiesIter iter = cookies.begin();
////    while (iter != cookies.end())
////    {
////        Cookie cookie = (*iter);
////        if(cookie.isExpired(expiredAt))
////        {
////            iter = cookies.erase(iter);
////            didClear = true;
////        }
////        else
////        {
////            ++iter;
////        }
////    }
////    
////    return didClear;
////}
////
////
////bool CookieStore::clearCookiesWithName(const std::string& name)
////{
////    Poco::FastMutex::ScopedLock lock(mutex);
////    
////    bool didClear = false;
////    CookiesIter iter = cookies.begin();
////    while (iter != cookies.end())
////    {
////        if((*iter).getName() == name)
////        {
////            iter = cookies.erase(iter);
////            didClear = true;
////        }
////        else
////        {
////            ++iter;
////        }
////    }
////
////    return didClear;
////}
////
////
////bool CookieStore::clearCookiesWithDomain(const std::string& domain)
////{
////    Poco::FastMutex::ScopedLock lock(mutex);
////    
////    bool didClear = false;
////    CookiesIter iter = cookies.begin();
////    while (iter != cookies.end())
////    {
////        if((*iter).getDomain() == domain)
////        {
////            iter = cookies.erase(iter);
////            didClear = true;
////        }
////        else
////        {
////            ++iter;
////        }
////    }
////    
////    return didClear;
////}
////
////
////bool CookieStore::clearCookiesWithPath(const std::string& path)
////{
////    Poco::FastMutex::ScopedLock lock(mutex);
////    
////    bool didClear = false;
////    CookiesIter iter = cookies.begin();
////    while (iter != cookies.end())
////    {
////        if((*iter).getPath() == path)
////        {
////            iter = cookies.erase(iter);
////            didClear = true;
////        }
////        else
////        {
////            ++iter;
////        }
////    }
////    
////    return didClear;
////}
////
////
////void CookieStore::clear()
////{
////    Poco::FastMutex::ScopedLock lock(mutex);
////    cookies.clear();
////}
////
////
////void CookieStore::dump()
////{
////    std::cout << "dumping: " << std::endl;
////    Poco::FastMutex::ScopedLock lock(mutex);
////    CookiesIter iter = cookies.begin();
////    while (iter != cookies.end())
////    {
////        std::cout << (*iter).toString() << std::endl;
////        ++iter;
////    }
////    std::cout << "finished dumping: " << std::endl;
////
////}
//
//
//} } // namespace ofx::HTTP
