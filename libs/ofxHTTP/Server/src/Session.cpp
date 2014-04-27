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


#include "ofx/HTTP/Server/Session.h"


namespace ofx {
namespace HTTP {


Session::Session(const Poco::UUID& uuid, const Poco::Timestamp& lastModified):
    _uuid(uuid),
    _lastModified(lastModified)
{
}


Session::~Session()
{
}


const Poco::UUID& Session::getId() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _lastModified.update();
    return _uuid;
}


const Poco::Timestamp Session::getLastModified() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _lastModified;
}


bool Session::has(const std::string& hashKey) const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _lastModified.update();
    return _sessionData.find(hashKey) != _sessionData.end();
}


void Session::put(const std::string& hashKey, const Poco::Any& hashValue)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _lastModified.update();
    _sessionData[hashKey] = hashValue;
}


Poco::Any Session::get(const std::string& hashKey) const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _lastModified.update();

    SessionData::const_iterator iter = _sessionData.find(hashKey);

    if (iter != _sessionData.end())
    {
        return iter->second;
    }
    else
    {
//        Poco::Any any;
        return Poco::Any();
    }
}


} } // namespace ofx::HTTP
