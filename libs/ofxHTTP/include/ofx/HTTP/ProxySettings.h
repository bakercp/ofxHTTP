//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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

