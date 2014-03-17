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


#include "CookieStore.h"


namespace ofx {
namespace HTTP {

    

CookieStore::CookieStore()
{
}


CookieStore::CookieStore(CookieStore& that)
{
    Poco::FastMutex::ScopedLock thatLock(that.mutex);
    cookies = that.cookies;
}


CookieStore& CookieStore::operator = (CookieStore& that)
{
    Poco::FastMutex::ScopedLock thisLock(mutex);
    Poco::FastMutex::ScopedLock thatLock(that.mutex);
    cookies = that.cookies;
    return *this;
}


CookieStore::~CookieStore()
{
}


CookieStore::Cookies CookieStore::getCookesForURI(const Poco::URI& uri,
                                                  bool bMatchSessionCookes) const
{
    // TODO: this needs to be a bit more sophisticated
    // http://stackoverflow.com/questions/4056306/how-to-handle-multiple-cookies-with-the-same-name
    
    Poco::FastMutex::ScopedLock lock(mutex);
    
    Cookies matchingCookies;
    
    CookiesConstIter iter = cookies.begin();
    while(iter != cookies.end())
    {
        Cookie cookie = (*iter);
        if(cookie.matchesURI(uri,bMatchSessionCookes))
        {
            matchingCookies.push_back(cookie);
        }
        ++iter;
    }
    
    return matchingCookies;
}


void CookieStore::store(const Poco::Net::HTTPResponse& response)
{
    std::vector<HTTPCookie> newCookies;
    response.getCookies(newCookies);

    Poco::FastMutex::ScopedLock lock(mutex);
    std::vector<HTTPCookie>::iterator iter = newCookies.begin();
    
    while(iter != newCookies.end())
    {
        addCookieWithExistingLock(Cookie(*iter));
        ++iter;
    }
}


void CookieStore::addCookie(const std::string& name,
                            const std::string& value,
                            bool isValueEscaped)
{
    std::string escapedValue = value;
    
    if(!isValueEscaped)
    {
        escapedValue = HTTPCookie::escape(value);
    }

    addCookie(HTTPCookie(name,escapedValue));
}


void CookieStore::addCookie(const HTTPCookie& cookie)
{
    addCookie(Cookie(cookie));
}


void CookieStore::addCookie(const Cookie& cookie)
{
    Poco::FastMutex::ScopedLock lock(mutex);
    addCookieWithExistingLock(cookie);
}


void CookieStore::addCookieWithExistingLock(const Cookie& cookie)
{
    if(cookie.isExpired()) return;
   
    // did clear needed? 
    //bool didClear = false; 
    CookiesIter iter = cookies.begin();
    while(iter != cookies.end())
    {
        if((*iter).matches(cookie))
        {
            iter = cookies.erase(iter);
            //didClear = true;
        }
        else {
            ++iter;
        }
    }
    
    if(!cookie.isExpired())
    {
        cookies.push_back(cookie);
    }
}


bool CookieStore::clearExpired(unsigned long long expiredAt)
{
    Poco::FastMutex::ScopedLock lock(mutex);

    bool didClear = false;
    CookiesIter iter = cookies.begin();
    while(iter != cookies.end())
    {
        Cookie cookie = (*iter);
        if(cookie.isExpired(expiredAt))
        {
            iter = cookies.erase(iter);
            didClear = true;
        }
        else
        {
            ++iter;
        }
    }
    
    return didClear;
}


bool CookieStore::clearCookiesWithName(const std::string& name)
{
    Poco::FastMutex::ScopedLock lock(mutex);
    
    bool didClear = false;
    CookiesIter iter = cookies.begin();
    while(iter != cookies.end())
    {
        if((*iter).getName() == name)
        {
            iter = cookies.erase(iter);
            didClear = true;
        }
        else
        {
            ++iter;
        }
    }

    return didClear;
}


bool CookieStore::clearCookiesWithDomain(const std::string& domain)
{
    Poco::FastMutex::ScopedLock lock(mutex);
    
    bool didClear = false;
    CookiesIter iter = cookies.begin();
    while(iter != cookies.end())
    {
        if((*iter).getDomain() == domain)
        {
            iter = cookies.erase(iter);
            didClear = true;
        }
        else
        {
            ++iter;
        }
    }
    
    return didClear;
}


bool CookieStore::clearCookiesWithPath(const std::string& path)
{
    Poco::FastMutex::ScopedLock lock(mutex);
    
    bool didClear = false;
    CookiesIter iter = cookies.begin();
    while(iter != cookies.end())
    {
        if((*iter).getPath() == path)
        {
            iter = cookies.erase(iter);
            didClear = true;
        }
        else
        {
            ++iter;
        }
    }
    
    return didClear;
}


void CookieStore::clear()
{
    Poco::FastMutex::ScopedLock lock(mutex);
    cookies.clear();
}


} } // namespace ofx::HTTP
