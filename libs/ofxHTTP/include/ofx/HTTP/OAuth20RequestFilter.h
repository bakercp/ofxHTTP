//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/OAuth20Credentials.h"


namespace ofx {
namespace HTTP {


/// \brief An OAuth 2.0 request filter.
class OAuth20RequestFilter: public AbstractRequestFilter
{
public:
    /// \brief Create a default OAuth 2.0 request filter.
    OAuth20RequestFilter();

    /// \brief Create a default OAuth 2.0 request filter with credentials.
    /// \param credentials The OAuth 2.0 credentials.
    OAuth20RequestFilter(const OAuth20Credentials& credentials);

    /// \brief Destroy the request filter.
    virtual ~OAuth20RequestFilter();

    /// \brief Set the 0Auth 2.0 credentials.
    /// \param credentials The OAuth 2.0 credentials.
    void setCredentials(const OAuth20Credentials& credentials);

    /// \returns The OAuth 2.0 credentials.
    OAuth20Credentials getCredentials() const;
    
    virtual void requestFilter(Context& context,
                               Request& request) const override;

private:
    /// \brief The OAuth 2.0 credentials used for authentication.
    OAuth20Credentials _credentials;

};


} } // namespace ofx::HTTP
