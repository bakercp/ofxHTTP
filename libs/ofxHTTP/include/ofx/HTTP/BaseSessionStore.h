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


class BaseSessionStore: public AbstractSessionStore
{
public:
    BaseSessionStore();

    BaseSessionStore(const std::string& sessionKeyName);

    virtual ~BaseSessionStore();

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

    virtual void clear() = 0;

    virtual void remove(const Poco::UUID& sessionId) = 0;

    virtual std::shared_ptr<AbstractSession> create() = 0;

    virtual std::shared_ptr<AbstractSession> get(const Poco::UUID& sessionId) = 0;

    virtual std::shared_ptr<AbstractSession> get(const Poco::Net::HTTPServerRequest& request);

    virtual std::shared_ptr<AbstractSession> get(const void* p);

    static const std::string DEFAULT_SESSION_KEY_NAME;

protected:
    const std::string _sessionKeyName;

    mutable Poco::FastMutex _mutex;

private:
    BaseSessionStore(const BaseSessionStore&);
    BaseSessionStore& operator = (const BaseSessionStore&);

};


/// \brief An in-memory session store.
class DefaultSessionStore: public BaseSessionStore
{
public:
    DefaultSessionStore();

    DefaultSessionStore(const std::string& sessionKeyName);

    virtual ~DefaultSessionStore();

    void clear();

    std::shared_ptr<AbstractSession> create();

    std::shared_ptr<AbstractSession> get(const Poco::UUID& sessionId);

    void remove(const Poco::UUID& sessionId);

protected:
    typedef std::map<Poco::UUID, std::shared_ptr<AbstractSession> > SessionMap;

    SessionMap _sessionMap;

};



} } // namespace ofx::HTTP
