//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/Credentials.h"


namespace ofx {
namespace HTTP {


Credentials::Credentials()
{
}


Credentials::Credentials(const std::string& username,
                         const std::string& password):
    _username(username),
    _password(password)
{
}


Credentials::~Credentials()
{
}


void Credentials::clear()
{
    _username.clear();
    _password.clear();
}


std::string Credentials::getUsername() const
{
    return _username;
}


std::string Credentials::getPassword() const
{
    return _password;
}


void Credentials::setUsername(const std::string& username)
{
    _username = username;
}


void Credentials::setPassword(const std::string& password)
{
    _password = password;
}


bool Credentials::hasCredentials() const
{
    return !_username.empty() || !_password.empty();
}


std::string Credentials::toString() const
{
    return _username + ":" + _password;
}

    
} } // namespace ofx::HTTP
