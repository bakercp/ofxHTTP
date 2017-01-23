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
//
//
//namespace ofx {
//namespace HTTP {
//
//
//class CookieUtils
//{
//public:
//    static bool equals(const std::string& thisString,
//                       const std::string& otherString);
//
//    static bool startsWith(const std::string& fullString,
//                           const std::string& beginning);
//
//    static bool endsWith(const std::string& fullString,
//                         const std::string& ending);
//
//    /// \brief path-matches algorithm, as defined by RFC 2965
//    static bool pathMatches(const Poco::URI& uri,
//                            const Poco::Net::HTTPCookie& cookie);
//
//    static bool domainMatches(const Poco::URI& uri,
//                              const Cookie& cookie);
//
//    static bool compare(const Cookie& a,
//                        const Cookie& b);
//
//    static void sortByPath(std::vector<Cookie>& cookies);
//
//    static bool hasExpired(const Cookie& cookie);
//
//};
//
//
//} } // namespace ofx::HTTP
