//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/OAuth10Credentials.h"


namespace ofx {
namespace HTTP {


/// \brief An OAuth 1.0 request filter.
class OAuth10RequestFilter: public AbstractRequestFilter
{
public:
    /// \brief Create a default OAuth 1.0 request filter.
    OAuth10RequestFilter();

    /// \brief Create a default OAuth 1.0 request filter with credentials.
    /// \param credentials The OAuth 1.0 credentials.
    OAuth10RequestFilter(const OAuth10Credentials& credentials);

    /// \brief Destroy the request filter.
    virtual ~OAuth10RequestFilter();

    /// \brief Set the 0Auth 1.0 credentials.
    /// \param credentials The OAuth 1.0 credentials.
    void setCredentials(const OAuth10Credentials& credentials);

    /// \returns The OAuth 1.0 credentials.
    OAuth10Credentials getCredentials() const;
    
    virtual void requestFilter(Context& context,
                               Request& request) const override;

private:
    /// \brief The OAuth 1.0 credentials used for authentication.
    OAuth10Credentials _credentials;

};


} } // namespace ofx::HTTP
