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


#pragma once


#include <string>
#include <stdint.h>
#include "ofx/HTTP/Credentials.h"


namespace ofx {
namespace HTTP {


class ProxySettings: public Credentials
{
public:
    ProxySettings();

    ProxySettings(const std::string& host,
                  uint16_t port);

    ProxySettings(const std::string& username,
                  const std::string& password,
                  const std::string& host,
                  uint16_t port);

    virtual ~ProxySettings();
    
    virtual void clear();
    
    std::string getHost() const;
    void setHost(const std::string& host);

    uint16_t getPort() const;
    void setPort(uint16_t port);

    static const std::string DEFAULT_PROXY_HOST;
    static const uint16_t DEFAULT_PROXY_PORT;
    
protected:
    std::string _host;
    uint16_t _port;

};


} } // namespace ofx::HTTP

