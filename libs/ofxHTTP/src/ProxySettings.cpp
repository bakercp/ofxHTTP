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
