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


#include "ofx/HTTP/Credentials.h"


namespace ofx {
namespace HTTP {


Credentials::Credentials() : _username(""), _password("")
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
