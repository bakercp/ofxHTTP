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


#include "ofx/HTTP/BaseSessionStore.h"


namespace ofx {
namespace HTTP {


const std::string BaseSessionStore::DEFAULT_SESSION_KEY_NAME = "session_key";


BaseSessionStore::BaseSessionStore(): _sessionKeyName(DEFAULT_SESSION_KEY_NAME)
{
}


BaseSessionStore::BaseSessionStore(const std::string& sessionKeyName):
    _sessionKeyName(sessionKeyName)
{
}


BaseSessionStore::~BaseSessionStore()
{
}


std::shared_ptr<AbstractSession> BaseSessionStore::get(const Poco::Net::HTTPServerRequest& request)
{
    // The session id.
    Poco::UUID sessionId;

    // Lock everything.
    Poco::FastMutex::ScopedLock lock(_mutex);

    // Get the cookies from the client.
    Poco::Net::NameValueCollection cookies;

    // Get the cookies
    request.getCookies(cookies);

    // Try to find a cookie with our session key name.
    Poco::Net::NameValueCollection::ConstIterator cookieIter = cookies.find(_sessionKeyName);

    std::shared_ptr<AbstractSession> session;

    if (cookieIter != cookies.end() && sessionId.tryParse(cookieIter->second))
    {
        return get(sessionId);
    }
    else
    {
        return session;
    }
}


std::shared_ptr<AbstractSession> BaseSessionStore::get(const void* p)
{
    const AbstractHasSessionId* pSessionId = static_cast<const AbstractHasSessionId*>(p);

    if (pSessionId)
    {
        return get(pSessionId->getSessionId());
    }
    else
    {
        std::shared_ptr<AbstractSession> ptr;
        return ptr;
    }
}


void BaseSessionStore::handleRequest(Poco::Net::HTTPServerRequest& request,
                                     Poco::Net::HTTPServerResponse& response)
{
    cout << "handling request: " << request.getURI() << endl;

    std::shared_ptr<AbstractSession> session = get(request);

    if (!session)
    {
        cout << "creating new sessionid: " << request.getURI() << endl;

        // Create a new session id.
        std::shared_ptr<AbstractSession> session = create();

        // Create a cookie with the session id.
        Poco::Net::HTTPCookie cookie(_sessionKeyName, session->getSessionId().toString());

        // Send our cookie with the response.
        response.addCookie(cookie);
    }
}


DefaultSessionStore::DefaultSessionStore()
{
}


DefaultSessionStore::DefaultSessionStore(const std::string& sessionKeyName):
    BaseSessionStore(sessionKeyName)
{
}


DefaultSessionStore::~DefaultSessionStore()
{
}


void DefaultSessionStore::clear()
{
    _sessionMap.clear();
}


std::shared_ptr<AbstractSession> DefaultSessionStore::create()
{
    std::shared_ptr<DefaultSession> session = std::shared_ptr<DefaultSession>(new DefaultSession());
    _sessionMap[session->getSessionId()] = session;
    return session;
}


std::shared_ptr<AbstractSession> DefaultSessionStore::get(const Poco::UUID& sessionId)
{
    cout << "DefaultSessionStore::: " << sessionId.toString() << endl;

    SessionMap::iterator sessionsIter = _sessionMap.find(sessionId);

    if (sessionsIter != _sessionMap.end())
    {
        return sessionsIter->second;
    }
    else
    {
        // Return an null ptr if it the session id doesn't exist.
        std::shared_ptr<AbstractSession> ptr;
        return ptr;
    }
}


void DefaultSessionStore::remove(const Poco::UUID& sessionId)
{
    _sessionMap.erase(sessionId);
}


} } // namespace ofx::HTTP
