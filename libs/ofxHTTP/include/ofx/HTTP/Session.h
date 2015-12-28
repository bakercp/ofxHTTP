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


#pragma once


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
