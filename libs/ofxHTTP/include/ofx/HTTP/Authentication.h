//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


namespace ofx {
namespace HTTP {


enum AuthenticationType
{
    NONE,
    BASIC,
    DIGEST
};

enum AuthenticationStatus
{
    OK,
    UNAUTHORIZED,
    NO_CREDENTIALS
};


} } // namespace ofx::HTTP
