// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/OAuth10RequestFilter.h"
#include "Poco/Net/OAuth10Credentials.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


OAuth10RequestFilter::OAuth10RequestFilter()
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


void OAuth10RequestFilter::requestFilter(BaseRequest& request, Context& context)
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
