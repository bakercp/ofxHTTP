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
// #include "ofx/Net/IPAddressRange.h"


namespace ofx {
namespace HTTP {


/// \brief This class mirrors Poco::Net::TCPServerParams, without ref counting.
class TCPServerParams
{
public:
    TCPServerParams();

    virtual ~TCPServerParams();

    void setThreadIdleTime(const Poco::Timespan& idleTime);
    const Poco::Timespan& getThreadIdleTime() const;
    void setMaxQueued(int count);
    int getMaxQueued() const;
    void setMaxThreads(int count);
    int getMaxThreads() const;
    void setThreadPriority(Poco::Thread::Priority prio);
    Poco::Thread::Priority getThreadPriority() const;

private:
    Poco::Timespan _threadIdleTime;
    int _maxThreads;
    int _maxQueued;
    Poco::Thread::Priority _threadPriority;
    
};

//        Poco::Net::HTTPServerParams


/// \brief This class mirrors Poco::Net::HTTPServerParams, without ref counting.
class HTTPServerParams: public TCPServerParams
{
public:
    HTTPServerParams();
    virtual ~HTTPServerParams();

    void setServerName(const std::string& serverName);
    const std::string& getServerName() const;
    void setSoftwareVersion(const std::string& softwareVersion);
    const std::string& getSoftwareVersion() const;
    void setTimeout(const Poco::Timespan& timeout);
    const Poco::Timespan& getTimeout() const;
    void setKeepAlive(bool keepAlive);
    bool getKeepAlive() const;
    void setKeepAliveTimeout(const Poco::Timespan& timeout);
    const Poco::Timespan& getKeepAliveTimeout() const;
    void setMaxKeepAliveRequests(int maxKeepAliveRequests);
    int getMaxKeepAliveRequests() const;

private:
    std::string _serverName;
    std::string _softwareVersion;
    Poco::Timespan _timeout;
    bool _keepAlive;
    int _maxKeepAliveRequests;
    Poco::Timespan _keepAliveTimeout;

};


class BaseServerSettings: public HTTPServerParams
{
public:
    BaseServerSettings(const std::string& host = DEFAULT_HOST,
                       unsigned short port = DEFAULT_PORT,
                       bool useSSL = DEFAULT_USE_SSL,
                       bool useSessionCache = DEFAULT_USE_SESSION_CACHE);

    virtual ~BaseServerSettings();

    void setHost(const std::string& host);
    std::string getHost() const;

    void setPort(unsigned short port);
    unsigned short getPort() const;

    void setUseSSL(bool useSSL);
    bool getUseSSL() const;

    void setUseSessionCache(bool useSessionCache);
    bool getUseSessionCache() const;

    Poco::URI getURI() const;

    /*
    const Net::IPAddressRange::List& getWhitelist() const;
    void setWhitelist(const Net::IPAddressRange::List& whitelist);

    const Net::IPAddressRange::List& getBlacklist() const;
    void setBlacklist(const Net::IPAddressRange::List& blacklist);
    */

    const static std::string DEFAULT_HOST;
    const static unsigned short DEFAULT_PORT;
    const static bool DEFAULT_USE_SSL;
    const static bool DEFAULT_USE_SESSION_CACHE;

private:
    std::string _host;
    unsigned short _port;
    bool _useSSL;
    bool _useSessionCache;

    /*
    Net::IPAddressRange::List _whitelist;
    Net::IPAddressRange::List _blacklist;
    */
};


} } // namespace ofx::HTTP
