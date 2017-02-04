//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/OAuth20RequestFilter.h"
#include "Poco/Net/OAuth20Credentials.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


OAuth20RequestFilter::OAuth20RequestFilter()
{
}


OAuth20RequestFilter::OAuth20RequestFilter(const OAuth20Credentials& credentials):
    _credentials(credentials)
{
}


OAuth20RequestFilter::~OAuth20RequestFilter()
{
}


void OAuth20RequestFilter::setCredentials(const OAuth20Credentials& credentials)
{
    _credentials = credentials;
}


OAuth20Credentials OAuth20RequestFilter::getCredentials() const
{
    return _credentials;
}


void OAuth20RequestFilter::requestFilter(Context& context,
                                         Request& request) const
{
    Poco::Net::OAuth20Credentials credentials(_credentials.getBearerToken(),
                                              _credentials.getScheme());

    credentials.authenticate(request);
}


} } // namespace ofx::OAuth
