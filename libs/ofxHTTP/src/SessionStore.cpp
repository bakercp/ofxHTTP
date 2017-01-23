//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SessionStore.h"
#include "ofx/HTTP/Session.h"


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


AbstractSession& BaseSessionStore::getSession(Poco::Net::HTTPServerRequest& request,
                                              Poco::Net::HTTPServerResponse& response)
{
    // Get the cookies from the client.
    Poco::Net::NameValueCollection cookies;

    // Get the cookies
    request.getCookies(cookies);

    // Try to find a cookie with our session key name.
    Poco::Net::NameValueCollection::ConstIterator cookieIter = cookies.begin();

    std::string sessionId;

    while (cookieIter != cookies.end())
    {
        if (0 == cookieIter->first.compare(_sessionKeyName))
        {
            if (sessionId.empty())
            {
                sessionId = cookieIter->second;
            }
            else
            {
                Poco::Net::HTTPCookie cookie(_sessionKeyName);
                cookie.setMaxAge(0); // Invalidate the cookie.
                response.addCookie(cookie);
            }
        }

        ++cookieIter;
    }

    if (hasSession(sessionId))
    {
        return getSession(sessionId);
    }
    else
    {
        // Create a new sesssion and return a reference.
        AbstractSession& session = createSession();

        // Create a cookie with the session id.
        Poco::Net::HTTPCookie cookie(_sessionKeyName, session.getId());

        // Send our cookie with the response.
        response.addCookie(cookie);

        return session;
    }
}


void BaseSessionStore::destroySession(Poco::Net::HTTPServerRequest& request,
                                      Poco::Net::HTTPServerResponse& response)
{
    // Get the cookies from the client.
    Poco::Net::NameValueCollection cookies;

    // Get the cookies
    request.getCookies(cookies);

    // Try to find a cookie with our session key name.
    Poco::Net::NameValueCollection::ConstIterator cookieIter = cookies.begin();

    while (cookieIter != cookies.end())
    {
        if (0 == cookieIter->first.compare(_sessionKeyName))
        {
            // Destroy the session data.
            destroySession(cookieIter->second);

            // Invalidate the cookies.
            Poco::Net::HTTPCookie cookie(_sessionKeyName, cookieIter->second);

            cookie.setMaxAge(0);
            response.addCookie(cookie);
        }

        ++cookieIter;
    }
}


SimpleSessionStore::SimpleSessionStore()
{
}


SimpleSessionStore::SimpleSessionStore(const std::string& sessionKeyName):
    BaseSessionStore(sessionKeyName)
{
}


SimpleSessionStore::~SimpleSessionStore()
{
}


bool SimpleSessionStore::hasSession(const std::string& sessionId) const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _sessionMap.find(sessionId) != _sessionMap.end();
}


AbstractSession& SimpleSessionStore::getSession(const std::string& sessionId)
{
    std::unique_lock<std::mutex> lock(_mutex);

    SessionMap::iterator sessionsIter = _sessionMap.find(sessionId);

    if (sessionsIter != _sessionMap.end())
    {
        // This must be valid b/c it's designed that way and we
        // need to return a reference.
        poco_assert(sessionsIter->second);

        return (*sessionsIter->second);
    }
    else
    {

        throw Poco::InvalidAccessException("Session " + sessionId + " not found.");
    }
}


AbstractSession& SimpleSessionStore::createSession()
{
    std::unique_lock<std::mutex> lock(_mutex);
    std::shared_ptr<SimpleSession> session = std::make_shared<SimpleSession>();
    _sessionMap[session->getId()] = session;
    return *session;
}


void SimpleSessionStore::destroySession(const std::string& sessionId)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _sessionMap.erase(_sessionMap.find(sessionId));
}


} } // namespace ofx::HTTP
