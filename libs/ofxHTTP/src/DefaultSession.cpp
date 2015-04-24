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


#include "ofx/HTTP/DefaultSession.h"


namespace ofx {
namespace HTTP {


DefaultSession::DefaultSession(const Poco::UUID& uuid,
                               const Poco::Timestamp& lastModified):
    _uuid(uuid),
    _lastModified(lastModified)
{
}


DefaultSession::~DefaultSession()
{
}


Poco::UUID DefaultSession::getSessionId() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _uuid;
}


const Poco::Timestamp DefaultSession::getLastModified() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _lastModified;
}


bool DefaultSession::has(const std::string& hashKey) const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _sessionDict.find(hashKey) != _sessionDict.end();
}


void DefaultSession::put(const std::string& hashKey, const Poco::Any& hashValue)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _lastModified.update();
    _sessionDict[hashKey] = hashValue;
}


Poco::Any DefaultSession::get(const std::string& hashKey,
                           const Poco::Any& defaultValue) const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    SessionDict::const_iterator iter = _sessionDict.find(hashKey);

    if (iter != _sessionDict.end())
    {
        return iter->second;
    }
    else
    {
        return defaultValue;
    }
}


} } // namespace ofx::HTTP
