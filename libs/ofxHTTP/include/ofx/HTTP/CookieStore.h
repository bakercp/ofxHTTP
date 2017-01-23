//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//
//
//
//#pragma once
//
//
//#include <map>
//#include <string>
//#include <vector>
//#include "Poco/RegularExpression.h"
//#include "Poco/String.h"
//#include "Poco/URI.h"
//#include "Poco/Net/HTTPCookie.h"
//#include "Poco/Net/HTTPRequest.h"
//#include "Poco/Net/HTTPResponse.h"
//#include "Poco/Net/NameValueCollection.h"
//#include "ofx/HTTP/Cookie.h"
//#include "ofx/HTTP/CookieUtils.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
//class AbstractCookiePolicy
//{
//public:
//    virtual ~AbstractCookiePolicy()
//    {
//    }
//
//    virtual bool shouldAccept(const Poco::URI& uri,
//                              const Poco::Net::HTTPCookie& cookie) const = 0;
//
//};
//
//
//class DefaultAcceptAllCookiePolicy: public AbstractCookiePolicy
//{
//public:
//    virtual ~DefaultAcceptAllCookiePolicy()
//    {
//    }
//
//    bool shouldAccept(const Poco::URI& uri,
//                      const Poco::Net::HTTPCookie& cookie) const
//    {
//        return true;
//    }
//
//};
//
//
//class DefaultAcceptNoneCookiePolicy: public AbstractCookiePolicy
//{
//public:
//    virtual ~DefaultAcceptNoneCookiePolicy()
//    {
//    }
//
//    bool shouldAccept(const Poco::URI& uri,
//                      const Poco::Net::HTTPCookie& cookie) const
//    {
//        return false;
//    }
//
//};
//
//
//class DefaultAcceptOriginalServerCookiePolicy: public AbstractCookiePolicy
//{
//public:
//    virtual ~DefaultAcceptOriginalServerCookiePolicy()
//    {
//    }
//
//    bool shouldAccept(const Poco::URI& uri,
//                      const Poco::Net::HTTPCookie& cookie) const
//    {
//        return CookieUtils::domainMatches(uri, cookie);
//    }
//};
//
//
//class AbstractCookieStore
//{
//public:
//    virtual ~AbstractCookieStore()
//    {
//    }
//
//    virtual void add(const Poco::URI& uri,
//                     const Poco::Net::HTTPCookie& cookie) = 0;
//
//    virtual std::vector<Cookie> get(const Poco::URI& uri) const = 0;
//
//};
//
//
///// \brief thread-safe cookie store
//class CookieStore: public AbstractCookieStore
//{
//public:
//    typedef std::shared_ptr<CookieStore> SharedPtr;
//    typedef std::weak_ptr<CookieStore> WeakPtr;
//
//    typedef std::vector<Cookie> Cookies;
//
//    CookieStore(const AbstractCookiePolicy& policy = DefaultAcceptOriginalServerCookiePolicy());
//    virtual ~CookieStore();
//
//    void add(const Poco::URI& uri, const Poco::Net::HTTPCookie& cookie);
//    std::vector<Cookie> get(const Poco::URI& uri) const;
//
//private:
//    typedef std::map<std::string, Cookies> CookieIndex;
//
//    CookieStore(const CookieStore&);
//    CookieStore& operator = (const CookieStore&);
//
//    const AbstractCookiePolicy& _policy;
//
//    std::vector<Cookie> getFromDomainIndex(const Poco::URI& uri);
//    std::vector<Cookie> getFromURIIndex(const Poco::URI& uri);
////    std::vector<Cookie> getFromURIIndex(const Poco::URI& uri);
//
//    Cookies     _cookies;
//    CookieIndex _domainIndex;
//    CookieIndex _uriIndex;
//
//    mutable Poco::FastMutex mutex;
//
//};
//
//
//} } // namespace ofx::HTTP
