//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>


namespace ofx {
namespace HTTP {


class Credentials
{
public:
    Credentials();
    Credentials(const std::string& username, const std::string& password);

    virtual ~Credentials();
    
    virtual void clear();

    std::string getUsername() const;
    std::string getPassword() const;
    
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    
    bool hasCredentials() const;
    
    std::string toString() const;
    
protected:
    std::string _username;
    std::string _password;
    
};


} } // namespace ofx::HTTP
