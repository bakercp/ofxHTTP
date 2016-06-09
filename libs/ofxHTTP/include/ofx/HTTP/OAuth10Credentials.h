// =============================================================================
//
// Copyright (c) 2009-2016 Christopher Baker <http://christopherbaker.net>
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
#include "ofConstants.h"
#include "json.hpp"


namespace ofx {
namespace HTTP {


class OAuth10Credentials
{
public:
    OAuth10Credentials();

    OAuth10Credentials(const std::string& consumerKey,
                       const std::string& consumerSecret);

    OAuth10Credentials(const std::string& consumerKey,
                       const std::string& consumerSecret,
                       const std::string& accessToken,
                       const std::string& accessTokentSecret);

    /// \brief Destroy the OAuth10Credentials.
    virtual ~OAuth10Credentials();

    std::string consumerKey() const;

    std::string consumerSecret() const;

    std::string accessToken() const;

    std::string accessTokenSecret() const;

    static OAuth10Credentials fromJSON(const ofJson& value);

    static ofJson toJSON(const OAuth10Credentials& credentials);

    static OAuth10Credentials fromFile(const std::string& credentialsFile);

    static bool toFile(const OAuth10Credentials& credentials,
                       const std::string& credentialsFile);

private:
    /// \brief The OAuth 1.0 consumer key.
    std::string _consumerKey;

    /// \brief The OAuth 1.0 consumer secret.
    std::string _consumerSecret;

    /// \brief The OAuth 1.0 access token.
    std::string _accessToken;

    /// \brief The OAuth 1.0 access token secret.
    std::string _accessTokenSecret;

};


} } // namespace ofx::Twitter
