//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/HTTPHost.h"


namespace ofx {
namespace HTTP {


HTTPHost::HTTPHost(const std::string& scheme,
                   const std::string& host,
                   uint16_t port):
    _scheme(scheme),
    _host(host),
    _port(port)
{

}

HTTPHost::~HTTPHost()
{
}


std::string HTTPHost::scheme() const
{
    return _scheme;
}


std::string HTTPHost::host() const
{
    return _host;
}


uint16_t HTTPHost::port() const
{
    return _port;
}


bool HTTPHost::secure() const
{
    return _scheme == "https" || _scheme == "wss";
}


std::string HTTPHost::toString() const
{
    return "[" + _scheme + "," + _host + "," + std::to_string(_port) + "]";
    //    std::stringstream ss;
//    ss << "[" << _scheme << "," << _host << "," << _port << "]";
//    return ss.str();
}


} } // namespace ofx::HTTP
