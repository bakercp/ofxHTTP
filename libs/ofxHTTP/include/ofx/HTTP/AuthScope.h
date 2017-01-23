//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <map>
#include <sstream>
#include <string>
#include <stdint.h>
#include "Poco/URI.h"
#include "ofx/HTTP/Authentication.h"


namespace ofx {
namespace HTTP {


class AuthScope
{
public:
    AuthScope();

    AuthScope(const std::string& host);

    AuthScope(const std::string& host, uint16_t port);

    AuthScope(const std::string& scheme,
              const std::string& host,
              uint16_t port);

    AuthScope(const std::string& host,
              AuthenticationType authType);

    AuthScope(const std::string& host,
              uint16_t port,
              AuthenticationType authType);

    AuthScope(const std::string& scheme,
              const std::string& host,
              uint16_t port,
              const std::string& realm,
              AuthenticationType authType);

    AuthScope(const Poco::URI& uri);
    
    bool hasScheme() const;
    void clearScheme();
    void setScheme(const std::string& scheme);
    std::string getScheme() const;

    bool hasHost() const;
    void clearHost();
    void setHost(const std::string& host);
    std::string getHost() const;

    bool hasPort() const;
    void clearPort();
    void setPort(uint16_t port);
    uint16_t getPort() const;

    bool hasRealm() const;
    void clearRealm();
    void setRealm(const std::string& realm);
    std::string getRealm() const;

    bool hasAuthType() const;
    void clearAuthType();
    void setAuthType(AuthenticationType authType);
    AuthenticationType getAuthType() const;

    int match(const AuthScope& scope) const;
    
    bool operator == (const AuthScope& scope) const;
    bool operator != (const AuthScope& scope) const;
    bool operator <  (const AuthScope& scope) const;

    std::string toString() const;
    
private:
    bool _hasScheme;
    std::string _scheme;

    bool _hasHost;
    std::string _host;
    
    bool _hasPort;
    uint16_t _port;
    
    bool _hasRealm;
    std::string _realm;
    
    bool _hasAuthType;
    AuthenticationType _authType;

};


} } // namespace ofx::HTTP
