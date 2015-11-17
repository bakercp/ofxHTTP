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


#include "ofx/HTTP/Session.h"
#include "Poco/UUIDGenerator.h"


namespace ofx {
namespace HTTP {


const std::string BaseSession::KEY_LAST_MODIFIED = "last_modified";


BaseSession::BaseSession(const std::string sessionId): _sessionId(sessionId)
{
}


BaseSession::~BaseSession()
{
}


std::string BaseSession::getId() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _sessionId;
}


std::string BaseSession::generateId()
{
    return Poco::UUIDGenerator::defaultGenerator().createRandom().toString();
}


SimpleSession::SimpleSession(const std::string& sessionId):
    BaseSession(sessionId)
{
}


SimpleSession::~SimpleSession()
{
}


bool SimpleSession::has(const std::string& key) const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _sessionDict.find(key) != _sessionDict.end();
}


void SimpleSession::put(const std::string& key, const std::string& value)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _sessionDict[key] = value;
}


std::string SimpleSession::get(const std::string& key,
                               const std::string& defaultValue) const
{
    std::unique_lock<std::mutex> lock(_mutex);

    std::map<std::string, std::string>::const_iterator iter = _sessionDict.find(key);

    if (iter != _sessionDict.end())
    {
        return iter->second;
    }
    else
    {
        return defaultValue;
    }
}


std::string SimpleSession::get(const std::string& key) const
{
    std::unique_lock<std::mutex> lock(_mutex);

    std::map<std::string, std::string>::const_iterator iter = _sessionDict.find(key);

    if (iter != _sessionDict.end())
    {
        return iter->second;
    }
    else
    {
        throw Poco::InvalidAccessException("No key: " + key);
    }
}


void SimpleSession::remove(const std::string& key)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _sessionDict.erase(_sessionDict.find(key));
}


void SimpleSession::clear()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _sessionDict.clear();
}


} } // namespace ofx::HTTP
