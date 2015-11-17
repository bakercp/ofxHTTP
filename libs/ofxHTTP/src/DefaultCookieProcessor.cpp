// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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
