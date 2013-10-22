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


#pragma once


#include <string>
#include "Poco/Thread.h"
#include "Poco/Timespan.h"
#include "Poco/URI.h"


namespace ofx {
namespace HTTP {


// This class mirrors Poco::Net::TCPServerParams, without ref counting.
class TCPServerParams
{
public:
    TCPServerParams():
        _threadIdleTime(10000000),
        _maxThreads(0),
        _maxQueued(64),
        _threadPriority(Poco::Thread::PRIO_NORMAL)
    {
    }

    virtual ~TCPServerParams()
    {
    }

    void setThreadIdleTime(const Poco::Timespan& idleTime)
    {
        _threadIdleTime = idleTime;
    }
    
    const Poco::Timespan& getThreadIdleTime() const
    {
        return _threadIdleTime;
    }

    void setMaxQueued(int count)
    {
        _maxQueued = count;
    }

    int getMaxQueued() const
    {
        return _maxQueued;
    }

    void setMaxThreads(int count)
    {
        // TODO: currently a bug (?) in void TCPServerParams::setMaxThreads(int count).
        // Poco::Net::HTTPServerParams::setMaxThreads() should be able to handle
        // a value of 0 (according to the documentation), but it is currently asserting
        // that the value must be > 0.
        _maxThreads = count;
    }

    int getMaxThreads() const
    {
        return _maxThreads;
    }

    void setThreadPriority(Poco::Thread::Priority prio)
    {
        _threadPriority = prio;
    }

    Poco::Thread::Priority getThreadPriority() const
    {
        return _threadPriority;
    }

private:
    Poco::Timespan _threadIdleTime;
    int _maxThreads;
    int _maxQueued;
    Poco::Thread::Priority _threadPriority;
    
};


// This class mirrors Poco::Net::HTTPServerParams, without ref counting.
class HTTPServerParams: public TCPServerParams
{
public:
    HTTPServerParams():
        _timeout(60000000),
        _keepAlive(true),
        _maxKeepAliveRequests(0),
        _keepAliveTimeout(15000000)
    {
    }

    virtual ~HTTPServerParams()
    {
    }

    void setServerName(const std::string& serverName)
    {
        _serverName = serverName;
    }

    const std::string& getServerName() const
    {
        return _serverName;
    }

    void setSoftwareVersion(const std::string& softwareVersion)
    {
        _softwareVersion = softwareVersion;
    }

    const std::string& getSoftwareVersion() const
    {
        return _softwareVersion;
    }

    void setTimeout(const Poco::Timespan& timeout)
    {
        _timeout = timeout;
    }

    const Poco::Timespan& getTimeout() const
    {
        return _timeout;
    }

    void setKeepAlive(bool keepAlive)
    {
        _keepAlive = keepAlive;
    }

    bool getKeepAlive() const
    {
        return _keepAlive;
    }

    void setKeepAliveTimeout(const Poco::Timespan& timeout)
    {
        _keepAliveTimeout = timeout;
    }

    const Poco::Timespan& getKeepAliveTimeout() const
    {
        return _keepAliveTimeout;
    }

    void setMaxKeepAliveRequests(int maxKeepAliveRequests)
    {
        _maxKeepAliveRequests = maxKeepAliveRequests;
    }

    int getMaxKeepAliveRequests() const
    {
        return _maxKeepAliveRequests;
    }

private:
    std::string    _serverName;
    std::string    _softwareVersion;
    Poco::Timespan _timeout;
    bool           _keepAlive;
    int            _maxKeepAliveRequests;
    Poco::Timespan _keepAliveTimeout;

};


class BaseServerSettings: public HTTPServerParams
{
public:
    BaseServerSettings(const std::string& host = DEFAULT_HOST,
                       const unsigned short port = DEFAULT_PORT,
                       bool useSSL = DEFAULT_USE_SSL);
    virtual ~BaseServerSettings();

    void setHost(const std::string& host);
    std::string getHost() const;

    void setPort(const unsigned short port);
    unsigned short getPort() const;

    void setUseSSL(bool useSSL);
    bool getUseSSL() const;

    Poco::URI getURI() const;

    const static std::string    DEFAULT_HOST;
    const static unsigned short DEFAULT_PORT;
    const static bool           DEFAULT_USE_SSL;

private:
    std::string      _host;
    unsigned short   _port;
    bool             _useSSL;

};


} } // namespace ofx::HTTP
