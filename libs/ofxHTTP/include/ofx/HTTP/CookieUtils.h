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
