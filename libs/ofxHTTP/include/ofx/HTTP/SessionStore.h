//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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

    bool hasSession(const std::string& sessionId) const override;
    AbstractSession& getSession(const std::string& sessionId) override;
    AbstractSession& createSession() override;
    void destroySession(const std::string& sessionId) override;

    typedef std::map<std::string, std::shared_ptr<AbstractSession>> SessionMap;

    SessionMap _sessionMap;

    mutable std::mutex _mutex;

};


} } // namespace ofx::HTTP
