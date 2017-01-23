//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/BaseServer.h"


namespace ofx {
namespace HTTP {


BaseServerHandle::BaseServerHandle(Poco::Net::HTTPRequestHandlerFactory& factory):
    _factory(factory)
{
}


BaseServerHandle::~BaseServerHandle()
{
}


Poco::Net::HTTPRequestHandler* BaseServerHandle::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return _factory.createRequestHandler(request);
}


TCPServerParams::TCPServerParams():
    _threadIdleTime(10000000),
    _maxThreads(0),
    _maxQueued(64),
    _threadPriority(Poco::Thread::PRIO_NORMAL)
{
}


TCPServerParams::~TCPServerParams()
{
}


void TCPServerParams::setThreadIdleTime(const Poco::Timespan& idleTime)
{
    _threadIdleTime = idleTime;
}


const Poco::Timespan& TCPServerParams::getThreadIdleTime() const
{
    return _threadIdleTime;
}


void TCPServerParams::setMaxQueued(int count)
{
    _maxQueued = count;
}


int TCPServerParams::getMaxQueued() const
{
    return _maxQueued;
}


void TCPServerParams::setMaxThreads(int count)
{
    // TODO: currently a bug (?) in void TCPServerParams::setMaxThreads(int count).
    // Poco::Net::HTTPServerParams::setMaxThreads() should be able to handle
    // a value of 0 (according to the documentation), but it is currently asserting
    // that the value must be > 0.
    _maxThreads = count;
}


int TCPServerParams::getMaxThreads() const
{
    return _maxThreads;
}


void TCPServerParams::setThreadPriority(Poco::Thread::Priority prio)
{
    _threadPriority = prio;
}


Poco::Thread::Priority TCPServerParams::getThreadPriority() const
{
    return _threadPriority;
}


HTTPServerParams::HTTPServerParams():
    _timeout(60000000),
    _keepAlive(true),
    _maxKeepAliveRequests(0),
    _keepAliveTimeout(15000000)
{
}


HTTPServerParams::~HTTPServerParams()
{
}


void HTTPServerParams::setServerName(const std::string& serverName)
{
    _serverName = serverName;
}


const std::string& HTTPServerParams::getServerName() const
{
    return _serverName;
}


void HTTPServerParams::setSoftwareVersion(const std::string& softwareVersion)
{
    _softwareVersion = softwareVersion;
}


const std::string& HTTPServerParams::getSoftwareVersion() const
{
    return _softwareVersion;
}


void HTTPServerParams::setTimeout(const Poco::Timespan& timeout)
{
    _timeout = timeout;
}


const Poco::Timespan& HTTPServerParams::getTimeout() const
{
    return _timeout;
}


void HTTPServerParams::setKeepAlive(bool keepAlive)
{
    _keepAlive = keepAlive;
}


bool HTTPServerParams::getKeepAlive() const
{
    return _keepAlive;
}


void HTTPServerParams::setKeepAliveTimeout(const Poco::Timespan& timeout)
{
    _keepAliveTimeout = timeout;
}


const Poco::Timespan& HTTPServerParams::getKeepAliveTimeout() const
{
    return _keepAliveTimeout;
}


void HTTPServerParams::setMaxKeepAliveRequests(int maxKeepAliveRequests)
{
    _maxKeepAliveRequests = maxKeepAliveRequests;
}


int HTTPServerParams::getMaxKeepAliveRequests() const
{
    return _maxKeepAliveRequests;
}


const std::string BaseServerSettings::DEFAULT_HOST = "127.0.0.1";
const uint16_t BaseServerSettings::DEFAULT_PORT = 8998;
const bool BaseServerSettings::DEFAULT_USE_SSL = false;
const bool BaseServerSettings::DEFAULT_USE_SESSIONS = true;


BaseServerSettings::BaseServerSettings(const std::string& host,
                                       uint16_t port,
                                       bool useSSL,
                                       bool useSessions):
    _host(host),
    _port(port),
    _useSSL(useSSL),
    _useSessions(useSessions)
{
}


BaseServerSettings::~BaseServerSettings()
{
}


void BaseServerSettings::setHost(const std::string& host)
{
    _host = host;
}


std::string BaseServerSettings::getHost() const
{
    return _host;
}


void BaseServerSettings::setPort(uint16_t port)
{
    _port = port;
}


uint16_t BaseServerSettings::getPort() const
{
    return _port;
}


void BaseServerSettings::setUseSSL(bool useSSL)
{
    _useSSL = useSSL;
}


bool BaseServerSettings::useSSL() const
{
    return _useSSL;
}


void BaseServerSettings::setUseSessions(bool useSessions)
{
    _useSessions = useSessions;
}


bool BaseServerSettings::useSessions() const
{
    return _useSessions;
}


Poco::URI BaseServerSettings::uri() const
{
    Poco::URI uri;
    
    uri.setScheme(_useSSL ? "https" : "http");
    uri.setHost(_host);
    uri.setPort(_port);
    
    return uri;
}    


const Net::IPAddressRange::List& BaseServerSettings::getWhitelist() const
{
    return _whitelist;
}


void BaseServerSettings::setWhitelist(const Net::IPAddressRange::List& whitelist)
{
    _whitelist = whitelist;
}


const Net::IPAddressRange::List& BaseServerSettings::getBlacklist() const
{
    return _blacklist;
}


void BaseServerSettings::setBlacklist(const Net::IPAddressRange::List& blacklist)
{
    _blacklist = blacklist;
}


} } // namespace ofx::HTTP
