//
// Copyright (c) 2009 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//
//
//
//#include "ofx/HTTP/OAuth10HTTPClient.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
//OAuth10HTTPClient::OAuth10HTTPClient(): OAuth10HTTPClient(OAuth10Credentials())
//{
//}
//
//
//OAuth10HTTPClient::OAuth10HTTPClient(const OAuth10Credentials& credentials)
//{
//    setCredentials(credentials);
//    addRequestFilter(&_oAuth10RequestFilter);
//}
//
//
//OAuth10HTTPClient::~OAuth10HTTPClient()
//{
//}
//
//
//void OAuth10HTTPClient::setCredentials(const OAuth10Credentials& credentials)
//{
//    _oAuth10RequestFilter.setCredentials(credentials);
//}
//
//
//OAuth10Credentials OAuth10HTTPClient::getCredentials() const
//{
//    return _oAuth10RequestFilter.getCredentials();
//}
//
//
//} } // namespace ofx::HTTP
