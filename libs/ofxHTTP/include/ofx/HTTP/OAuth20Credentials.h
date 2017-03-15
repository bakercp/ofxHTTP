//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "ofConstants.h"
#include "json.hpp"


namespace ofx {
namespace HTTP {


class OAuth20Credentials
{
public:
	OAuth20Credentials();

	OAuth20Credentials(const std::string& bearerToken);

	OAuth20Credentials(const std::string& bearerToken,
                       const std::string& scheme);

	virtual ~OAuth20Credentials();

	void setBearerToken(const std::string& bearerToken);

	const std::string& getBearerToken() const;

	void setScheme(const std::string& scheme);

	const std::string& getScheme() const;

	static const std::string SCHEME;

//    static OAuth20Credentials fromJSON(const ofJson& value);
//
//    static ofJson toJSON(const OAuth20Credentials& credentials);
//
//    static OAuth20Credentials fromFile(const std::filesystem::path& credentialsFile);
//
//    static bool toFile(const OAuth20Credentials& credentials,
//                       const std::string& credentialsFile);


private:
    /// \brief The OAuth 2.0 bearer token.
	std::string _bearerToken;

    /// \brief The OAuth 1.0 scheme.
	std::string _scheme;
};


} } // namespace ofx::HTTP

