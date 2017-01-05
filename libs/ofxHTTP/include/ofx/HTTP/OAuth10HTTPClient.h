//
// Copyright (c) 2009 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//
//
//
//#pragma once
//
//
//#include "ofx/HTTP/HTTPClient.h"
//#include "ofx/HTTP/OAuth10Credentials.h"
//#include "ofx/HTTP/OAuth10RequestFilter.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
///// \brief An OAuth 1.0 compatible base client.
//class OAuth10HTTPClient: public HTTPClient
//{
//public:
//    /// \brief Create a default client.
//    OAuth10HTTPClient();
//
//    /// \brief Create a client with the provided credentials.
//    /// \param credentials OAuth 1.0 credentials.
//    OAuth10HTTPClient(const OAuth10Credentials& credentials);
//
//    /// \breif Destroy the client.
//    virtual ~OAuth10HTTPClient();
//
//    /// \brief Set the OAuth 1.0 credentials.
//    /// \param credentials OAuth 1.0 credentials.
//    void setCredentials(const OAuth10Credentials& credentials);
//
//    /// \returns the OAuth 1.0 credentials.
//    OAuth10Credentials getCredentials() const;
//
//private:
//    /// \brief The request filter.
//    OAuth10RequestFilter _oAuth10RequestFilter;
//
//};
//
//
//} } // namespace ofx::HTTP
