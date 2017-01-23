//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include <iostream>


namespace ofx {
namespace HTTP {


class HTTPHost
{
public:
    HTTPHost(const std::string& scheme,
             const std::string& host,
             uint16_t port);

    ~HTTPHost();

    std::string scheme() const;
    std::string host() const;
    uint16_t port() const;

    /// \returns true of this host is secure.
    bool secure() const;

    std::string toString() const;

    /// \brief Stream output.
    /// \param os the std::ostream.
    /// \param host The HTTPHost to output.
    /// \returns the updated std::ostream reference.
    friend std::ostream& operator << (std::ostream& os, const HTTPHost& host);

private:
    std::string _scheme;
    std::string _host;
    uint16_t _port;

};


inline std::ostream& operator<<(std::ostream& os, const HTTPHost& host)
{
    os << host.toString();
    return os;
}


} } // namespace ofx::HTTP
