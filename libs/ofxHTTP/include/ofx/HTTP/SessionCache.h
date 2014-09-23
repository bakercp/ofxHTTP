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
#include "ofx/HTTP/Session.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


class SessionCache
{
public:
    typedef std::shared_ptr<SessionCache> SharedPtr;

    SessionCache();
    SessionCache(const std::string& sessionKeyName);

    virtual ~SessionCache();

    Session::SharedPtr getSession(Poco::Net::HTTPServerRequest& request,
                                  Poco::Net::HTTPServerResponse& response);

    static SharedPtr makeShared()
    {
        return SharedPtr(new SessionCache());
    }

    static const std::string DEFAULT_SESSION_KEY_NAME;

private:
    SessionCache(const SessionCache&);
	SessionCache& operator = (const SessionCache&);

    typedef std::map<Poco::UUID, Session::SharedPtr> SessionHash;

    SessionHash _sessionHash;

    const std::string _sessionKeyName;

    mutable Poco::FastMutex _mutex;

};


} } // namespace ofx::HTTP
