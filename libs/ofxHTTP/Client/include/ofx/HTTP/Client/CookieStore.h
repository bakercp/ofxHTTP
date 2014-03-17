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


#pragma once


#include <vector>
#include <string>
#include "Poco/RegularExpression.h"
#include "Poco/String.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NameValueCollection.h"
#include "Cookie.h"


using Poco::Net::HTTPCookie;


namespace ofx {
namespace HTTP {

    
// thread-safe cookie store
class CookieStore
{
public:
    typedef std::vector<Cookie>                 Cookies;
    typedef std::vector<Cookie>::iterator       CookiesIter;
    typedef std::vector<Cookie>::const_iterator CookiesConstIter;

    CookieStore();
    CookieStore(CookieStore& that);
	CookieStore& operator = (CookieStore& that);

    virtual ~CookieStore();
    
    Cookies getCookesForURI(const Poco::URI& uri,
                            bool matchSessionCookes = true) const;
    
    void store(const Poco::Net::HTTPResponse& response);

    void addCookie(const std::string& name,
                   const std::string& value = "",
                   bool isValueEscaped = false);

    void addCookie(const HTTPCookie& cookie);

    void addCookie(const Cookie& cookie);
    
    void addCookies(const std::vector<HTTPCookie>& cookies);


    bool clearCookiesWithName(const std::string& name);

    bool clearCookiesWithDomain(const std::string& domain);

    bool clearCookiesWithPath(const std::string& path);

    bool clearExpired(unsigned long long expiredAt = ofGetSystemTime());

    void clear();

    void dump();
    
protected:
    void addCookieWithExistingLock(const Cookie& cookie);

private:
    CookieStore(const CookieStore& that);
	CookieStore& operator = (const CookieStore& that);

    mutable Poco::FastMutex mutex;
    
    Cookies cookies;
    
};

    
} } // namespace ofx::HTTP
