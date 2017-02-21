//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/Client.h"
#include "ofx/HTTP/OAuth20Credentials.h"
#include "ofx/HTTP/OAuth20RequestFilter.h"


namespace ofx {
namespace HTTP {


/// \brief An OAuth 2.0 compatible base client.
class OAuth20HTTPClient: public Client
{
public:
    /// \brief Create a default client.
    OAuth20HTTPClient();

    /// \brief Create a client with the provided credentials.
    /// \param credentials OAuth 2.0 credentials.
    OAuth20HTTPClient(const OAuth20Credentials& credentials);

    /// \breif Destroy the client.
    virtual ~OAuth20HTTPClient();

    /// \brief Set the OAuth 2.0 credentials.
    /// \param credentials OAuth 2.0 credentials.
    void setCredentials(const OAuth20Credentials& credentials);

    /// \returns the OAuth 2.0 credentials.
    OAuth20Credentials getCredentials() const;

protected:
    virtual void requestFilter(Context& context,
                               Request& request) const override;


private:
    /// \brief The request filter.
    OAuth20RequestFilter _oAuth20RequestFilter;

};


} } // namespace ofx::HTTP
