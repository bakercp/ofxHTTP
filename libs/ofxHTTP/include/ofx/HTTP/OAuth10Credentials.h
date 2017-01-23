//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "ofConstants.h"
#include "ofJson.h"


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
