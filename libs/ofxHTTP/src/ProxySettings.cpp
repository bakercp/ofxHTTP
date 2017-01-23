//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ProxySettings.h"


namespace ofx {
namespace HTTP {


const std::string ProxySettings::DEFAULT_PROXY_HOST = "";
const uint16_t ProxySettings::DEFAULT_PROXY_PORT = 0;


ProxySettings::ProxySettings():
    _host(DEFAULT_PROXY_HOST),
    _port(DEFAULT_PROXY_PORT)
{
}


ProxySettings::ProxySettings(const std::string& host,
                             uint16_t port):
    _host(host),
    _port(port)
{
}


ProxySettings::ProxySettings(const std::string& username,
                             const std::string& password,
                             const std::string& host,
                             uint16_t port):
    Credentials(username, password),
    _host(host),
    _port(port)
{
}


ProxySettings::~ProxySettings()
{
}


void ProxySettings::clear()
{
    Credentials::clear();
    _host = DEFAULT_PROXY_HOST;
    _port = DEFAULT_PROXY_PORT;
}


std::string ProxySettings::getHost() const
{
    return _host;
}


void ProxySettings::setHost(const std::string& host)
{
    _host = host;
}


uint16_t ProxySettings::getPort() const
{
    return _port;
}


void ProxySettings::setPort(uint16_t port)
{
    _port = port;
}


} } // namespace ofx::HTTP
