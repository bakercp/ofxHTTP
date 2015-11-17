// =============================================================================
//
// Copyright (c) 2014-2015 Christopher Baker <http://christopherbaker.net>
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
#include "Poco/Mutex.h"
#include "ofTypes.h"
#include "ofx/HTTP/AbstractServerTypes.h"


namespace ofx {
namespace HTTP {


class BaseSessionStore: public AbstractSessionStore
{
public:
    BaseSessionStore();

    BaseSessionStore(const std::string& sessionKeyName);

    virtual ~BaseSessionStore();

    AbstractSession& getSession(Poco::Net::HTTPServerRequest& request,
                                Poco::Net::HTTPServerResponse& response);

    void destroySession(Poco::Net::HTTPServerRequest& request,
                        Poco::Net::HTTPServerResponse& response);

    static const std::string DEFAULT_SESSION_KEY_NAME;

protected:
    virtual bool hasSession(const std::string& sessionId) const = 0;
    virtual AbstractSession& getSession(const std::string& sessionId) = 0;
    virtual AbstractSession& createSession() = 0;
    virtual void destroySession(const std::string& sessionId) = 0;

    const std::string _sessionKeyName;

private:
    BaseSessionStore(const BaseSessionStore&);
    BaseSessionStore& operator = (const BaseSessionStore&);

};


/// \brief An in-memory session store.
class SimpleSessionStore: public BaseSessionStore
{
public:
    SimpleSessionStore();

    SimpleSessionStore(const std::string& sessionKeyName);

    virtual ~SimpleSessionStore();

protected:
    SimpleSessionStore(const SimpleSessionStore&);
    SimpleSessionStore& operator = (const SimpleSessionStore&);

    bool hasSession(const std::string& sessionId) const;
    AbstractSession& getSession(const std::string& sessionId);
    AbstractSession& createSession();
    void destroySession(const std::string& sessionId);

    typedef std::map<std::string, std::shared_ptr<AbstractSession>> SessionMap;

    SessionMap _sessionMap;

    mutable std::mutex _mutex;

};


} } // namespace ofx::HTTP
