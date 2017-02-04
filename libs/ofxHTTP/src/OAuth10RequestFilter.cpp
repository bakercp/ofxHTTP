//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/OAuth10RequestFilter.h"
#include "Poco/Net/OAuth10Credentials.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


OAuth10RequestFilter::OAuth10RequestFilter()
{
}


OAuth10RequestFilter::OAuth10RequestFilter(const OAuth10Credentials& credentials):
    _credentials(credentials)
{
}


OAuth10RequestFilter::~OAuth10RequestFilter()
{
}


void OAuth10RequestFilter::setCredentials(const OAuth10Credentials& credentials)
{
    _credentials = credentials;
}


OAuth10Credentials OAuth10RequestFilter::getCredentials() const
{
    return _credentials;
}


void OAuth10RequestFilter::requestFilter(Context& context,
                                         Request& request) const
{
    Poco::Net::OAuth10Credentials credentials(_credentials.consumerKey(),
                                              _credentials.consumerSecret(),
                                              _credentials.accessToken(),
                                              _credentials.accessTokenSecret());

    credentials.authenticate(request,
                             Poco::URI(request.getURI()),
                             request.form(),
                             Poco::Net::OAuth10Credentials::SIGN_HMAC_SHA1);
}


} } // namespace ofx::OAuth
