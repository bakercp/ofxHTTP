//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <map>
#include <string>
#include "ofx/HTTP/AbstractServerTypes.h"
#include "Poco/Mutex.h"
#include "ofEvents.h"


namespace ofx {
namespace HTTP {


class BaseSession: public AbstractSession
{
public:
    BaseSession(const std::string sessionId = generateId());

    virtual ~BaseSession();

    std::string getId() const override;

    /// \brief A utility function for generating unique session ids.
    /// \returns A unique session id string.
    static std::string generateId();

    static const std::string KEY_LAST_MODIFIED;

protected:
    BaseSession(const BaseSession&);
    BaseSession& operator = (const BaseSession&);

    std::string _sessionId;

    mutable std::mutex _mutex;

};



/// \brief A client cookie-based session store for servers.
///
/// Client sessions are tracked via cookie. Server routes can access the session
/// store and associate any information with the client's session.
class SimpleSession: public BaseSession
{
public:
    SimpleSession(const std::string& sessionId = generateId());

    virtual ~SimpleSession();

    bool has(const std::string& key) const override;

    void put(const std::string& key, const std::string& value) override;

    std::string get(const std::string& key,
                    const std::string& defaultValue) const override;

    std::string get(const std::string& key) const override;

    void remove(const std::string& key) override;

    void clear() override;

protected:
    SimpleSession(const SimpleSession&);
    SimpleSession& operator = (const SimpleSession&);

    std::map<std::string, std::string> _sessionDict;

};


} } // namespace ofx::HTTP
