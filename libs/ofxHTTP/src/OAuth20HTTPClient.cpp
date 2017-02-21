//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/OAuth20HTTPClient.h"


namespace ofx {
namespace HTTP {


OAuth20HTTPClient::OAuth20HTTPClient(): OAuth20HTTPClient(OAuth20Credentials())
{
}


OAuth20HTTPClient::OAuth20HTTPClient(const OAuth20Credentials& credentials)
{
    setCredentials(credentials);
}


OAuth20HTTPClient::~OAuth20HTTPClient()
{
}


void OAuth20HTTPClient::setCredentials(const OAuth20Credentials& credentials)
{
    _oAuth20RequestFilter.setCredentials(credentials);
}


OAuth20Credentials OAuth20HTTPClient::getCredentials() const
{
    return _oAuth20RequestFilter.getCredentials();
}


void OAuth20HTTPClient::requestFilter(Context& context, Request& request) const
{
    _oAuth20RequestFilter.requestFilter(context, request);
}


} } // namespace ofx::HTTP
