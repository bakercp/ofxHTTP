//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
