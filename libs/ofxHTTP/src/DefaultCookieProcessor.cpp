//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/DefaultCookieProcessor.h"


namespace ofx {
namespace HTTP {


//DefaultCookieProcessor::DefaultCookieProcessor()
//{
//}
//
//
//DefaultCookieProcessor::~DefaultCookieProcessor()
//{
//}
//
//
//void DefaultCookieProcessor::filter(Poco::Net::HTTPRequest& request,
//                                            Context& context)
//{
////    CookieStore::SharedPtr store(context.getCookieStore().lock());
////
////
////    if (store)
////    {
//////        store->set(request); // apply cookies;
////        cout << "DefaultCookieProcessor::processRequest!" << std::endl;
////    }
//}
//
//
//void DefaultCookieProcessor::filter(Poco::Net::HTTPRequest& request,
//                                             Poco::Net::HTTPResponse& response,
//                                             Context& context)
//{
////    CookieStore::SharedPtr store(context.getCookieStore().lock());
////
////
////    if (store)
////    {
////        cout << "storing this!" << endl;
////
////        Utils::dumpHeaders(response, OF_LOG_NOTICE);
////
////        std::vector<Poco::Net::HTTPCookie> newCookies;
////        response.getCookies(newCookies);
////
////        std::vector<Poco::Net::HTTPCookie>::iterator iter = newCookies.begin();
////        while (iter != newCookies.end())
////        {
////            cout << "COOKIE: " << (*iter).toString() << endl;
////
////
////            Cookie cookie(*iter);
//////            cookie.setDomain(request.getHost());
//////            cout << cookie.toString() << endl;
////            ++iter;
////        }
////
//////        store->store(request, response); // store cookies;
////        cout << "DefaultCookieProcessor::processResponse!" << std::endl;
////
//////        store->dump();
////    }
//}


} } // namespace ofx::HTTP
