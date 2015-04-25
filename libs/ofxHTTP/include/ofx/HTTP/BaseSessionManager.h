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


#pragma once


#include <string>
#include <map>
#include "Poco/Timestamp.h"
#include "Poco/UUID.h"
#include "Poco/Any.h"
#include "ofUtils.h"
#include "ofTypes.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/HTTPUtils.h"
#include "ofx/HTTP/DefaultSession.h"


namespace ofx {
namespace HTTP {


template<typename SessionType>
class BaseSessionManager_: public AbstractSessionManager
{
public:
    BaseSessionManager_();

    BaseSessionManager_(const std::string& sessionKeyName);

    virtual ~BaseSessionManager_();

    void clear();

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response);

    virtual Poco::UUID getSessionId(const Poco::Net::HTTPServerRequest& request) const;

    virtual std::shared_ptr<AbstractSession> getSession(const Poco::UUID& sessionId);

    virtual std::shared_ptr<AbstractSession> getSession(const void* p);

    static const std::string DEFAULT_SESSION_KEY_NAME;

private:
    BaseSessionManager_(const BaseSessionManager_&);
	BaseSessionManager_& operator = (const BaseSessionManager_&);

    typedef std::map<Poco::UUID, std::shared_ptr<AbstractSession> > SessionMap;

    SessionMap _sessionMap;

    const std::string _sessionKeyName;

    mutable Poco::FastMutex _mutex;

};


typedef BaseSessionManager_<DefaultSession> DefaultSessionManager;


template<typename SessionType>
const std::string BaseSessionManager_<SessionType>::DEFAULT_SESSION_KEY_NAME = "session_key";


template<typename SessionType>
BaseSessionManager_<SessionType>::BaseSessionManager_(): _sessionKeyName(DEFAULT_SESSION_KEY_NAME)
{
}


template<typename SessionType>
BaseSessionManager_<SessionType>::BaseSessionManager_(const std::string& sessionKeyName):
    _sessionKeyName(sessionKeyName)
{
}


template<typename SessionType>
BaseSessionManager_<SessionType>::~BaseSessionManager_()
{
}


template<typename SessionType>
void BaseSessionManager_<SessionType>::clear()
{
    _sessionMap.clear();
}


template<typename SessionType>
Poco::UUID BaseSessionManager_<SessionType>::getSessionId(const Poco::Net::HTTPServerRequest& request) const
{
    Poco::UUID sessionId;

    // Get the cookies from the client.
    Poco::Net::NameValueCollection cookies;

    // Get the cookies.
    request.getCookies(cookies);

    // Try to find a cookie with our session key name.
    Poco::Net::NameValueCollection::ConstIterator cookieIter = cookies.find(_sessionKeyName);

    if (cookieIter != cookies.end() && sessionId.tryParse(cookieIter->second))
    {
        return sessionId;
    }
    else
    {
        // Return a null session id if none is found.
        return Poco::UUID::null();
    }
}


template<typename SessionType>
std::shared_ptr<AbstractSession> BaseSessionManager_<SessionType>::getSession(const Poco::UUID& sessionId)
{
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


template<typename SessionType>
std::shared_ptr<AbstractSession> BaseSessionManager_<SessionType>::getSession(const void* p)
{
    const AbstractSessionId* pSessionId = static_cast<const AbstractSessionId*>(p);

    if (pSessionId)
    {
        return getSession(pSessionId->getSessionId());
    }
    else
    {
        std::shared_ptr<AbstractSession> ptr;
        return ptr;
    }
}


template<typename SessionType>
void BaseSessionManager_<SessionType>::handleRequest(Poco::Net::HTTPServerRequest& request,
                                                     Poco::Net::HTTPServerResponse& response)
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

    if (cookieIter == cookies.end() || // The client sent no cookie.
        !sessionId.tryParse(cookieIter->second) || // The client sent a cookie, but it couldn't be parsed.
        _sessionMap.find(sessionId) == _sessionMap.end()) // The session id was not in memory.
    {
        // Rebuild a new session id.
        sessionId = Poco::UUIDGenerator::defaultGenerator().createRandom();

        // Create a new session id and add session to our map.
        _sessionMap[sessionId] = std::shared_ptr<AbstractSession>(new DefaultSession(sessionId));;

        // Create a cookie with the session id.
        Poco::Net::HTTPCookie cookie(_sessionKeyName, sessionId.toString());
        
        // Send our cookie with the response.
        response.addCookie(cookie);
    }
}


} } // namespace ofx::HTTP
