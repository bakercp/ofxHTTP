// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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
