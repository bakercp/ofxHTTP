// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/SessionCache.h"



namespace ofx {
namespace HTTP {


const std::string SessionCache::DEFAULT_SESSION_KEY_NAME = "session_key";


SessionCache::SessionCache(): _sessionKeyName(DEFAULT_SESSION_KEY_NAME)
{
}


SessionCache::SessionCache(const std::string& sessionKeyName):
    _sessionKeyName(sessionKeyName)
{
}


SessionCache::~SessionCache()
{
}


Session::SharedPtr SessionCache::getSession(Poco::Net::HTTPServerRequest& request,
                                            Poco::Net::HTTPServerResponse& response)
{
    // lock everything
    Poco::FastMutex::ScopedLock lock(_mutex);

    // get the cookies from the client.
    Poco::Net::NameValueCollection cookies;

    // get the cookies
    request.getCookies(cookies);

    // try to find a cookie with our session key name.
    Poco::Net::NameValueCollection::ConstIterator cookieIter = cookies.find(_sessionKeyName);

    Poco::UUID uuid;

    SessionHash::iterator sessionHashIter = _sessionHash.find(uuid);

    // Did the client present a valid session cookie and can we parse it?
    if (cookieIter != cookies.end() && uuid.tryParse(cookieIter->second))
    {
        if (sessionHashIter != _sessionHash.end())
        {
            return sessionHashIter->second;
        }
        else
        {
            // Initialize new session data
            Session::SharedPtr session = Session::makeShared(uuid);

            // Add session to our hash
            _sessionHash[uuid] = session;

            return session;
        }
    }
    else
    {
        // Initialize new session data
        Session::SharedPtr session = Session::makeShared();

        // Add session to our hash
        _sessionHash[uuid] = session;

        // Create a cookie.
        Poco::Net::HTTPCookie cookie(_sessionKeyName, session->getId().toString());

        // Send our cookie with the response.
        response.addCookie(cookie);

        // Return the session
        return session;
    }
}


} } // namespace ofx::HTTP
