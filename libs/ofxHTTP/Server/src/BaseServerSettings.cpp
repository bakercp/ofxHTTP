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


#include "ofx/HTTP/Server/BaseServerSettings.h"


namespace ofx {
namespace HTTP {


const std::string BaseServerSettings::DEFAULT_HOST = "127.0.0.1";
const unsigned short BaseServerSettings::DEFAULT_PORT = 8080;
const bool BaseServerSettings::DEFAULT_USE_SSL = false;


//------------------------------------------------------------------------------
BaseServerSettings::BaseServerSettings(const std::string& host,
                                       const unsigned short port,
                                       bool useSSL):
    _host(host),
    _port(port),
    _useSSL(useSSL)
{
}

//------------------------------------------------------------------------------
BaseServerSettings::~BaseServerSettings()
{
}

//------------------------------------------------------------------------------
void BaseServerSettings::setHost(const std::string& host)
{
    _host = host;
}

//------------------------------------------------------------------------------
std::string BaseServerSettings::getHost() const
{
    return _host;
}

//------------------------------------------------------------------------------
void BaseServerSettings::setPort(const unsigned short port)
{
    _port = port;
}

//------------------------------------------------------------------------------
unsigned short BaseServerSettings::getPort() const
{
    return _port;
}

//------------------------------------------------------------------------------
void BaseServerSettings::setUseSSL(bool useSSL)
{
    _useSSL = useSSL;
}

//------------------------------------------------------------------------------
bool BaseServerSettings::getUseSSL() const
{
    return _useSSL;
}

//------------------------------------------------------------------------------
Poco::URI BaseServerSettings::getURI() const
{
    Poco::URI uri;

    uri.setScheme(_useSSL ? "https" : "http");
    uri.setHost(_host);
    uri.setPort(_port);

    return uri;
}    


} } // namespace ofx::HTTP
