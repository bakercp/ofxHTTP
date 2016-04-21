//
// OAuth20Credentials.cpp
//
// $Id$
//
// Library: Net
// Package: OAuth
// Module:	OAuth20Credentials
//
// Copyright (c) 2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ofx/HTTP/OAuth20Credentials.h"
#include "Poco/Net/OAuth20Credentials.h"
#include "Poco/Net/NetException.h"
#include "Poco/String.h"


namespace ofx {
namespace HTTP {


const std::string OAuth20Credentials::SCHEME = Poco::Net::OAuth20Credentials::SCHEME;


OAuth20Credentials::OAuth20Credentials():
	_scheme(SCHEME)
{
}


OAuth20Credentials::OAuth20Credentials(const std::string& bearerToken):
	_bearerToken(bearerToken),
	_scheme(SCHEME)
{
}


OAuth20Credentials::OAuth20Credentials(const std::string& bearerToken, const std::string& scheme):
	_bearerToken(bearerToken),
	_scheme(scheme)
{
}


OAuth20Credentials::~OAuth20Credentials()
{
}


void OAuth20Credentials::setBearerToken(const std::string& bearerToken)
{
	_bearerToken = bearerToken;
}


const std::string& OAuth20Credentials::getScheme() const
{
    return _scheme;
}


void OAuth20Credentials::setScheme(const std::string& scheme)
{
	_scheme = scheme;
}

	
const std::string& OAuth20Credentials::getBearerToken() const
{
    return _bearerToken;
}


} } // namespace Poco::Net
