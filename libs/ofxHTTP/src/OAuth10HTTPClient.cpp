//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/OAuth10HTTPClient.h"


namespace ofx {
namespace HTTP {


OAuth10HTTPClient::OAuth10HTTPClient(): OAuth10HTTPClient(OAuth10Credentials())
{
}


OAuth10HTTPClient::OAuth10HTTPClient(const OAuth10Credentials& credentials)
{
    setCredentials(credentials);
}


OAuth10HTTPClient::~OAuth10HTTPClient()
{
}


void OAuth10HTTPClient::setCredentials(const OAuth10Credentials& credentials)
{
    _oAuth10RequestFilter.setCredentials(credentials);
}


OAuth10Credentials OAuth10HTTPClient::getCredentials() const
{
    return _oAuth10RequestFilter.getCredentials();
}


void OAuth10HTTPClient::requestFilter(Context& context, Request& request) const
{
    _oAuth10RequestFilter.requestFilter(context, request);
}


} } // namespace ofx::HTTP
