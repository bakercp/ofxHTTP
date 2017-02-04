//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/AbstractClientTypes.h"


namespace ofx {
namespace HTTP {


/// \brief A simple default client redirect policy.
///
/// This redirect policy will attempt to redirect response codes:
///
///     - 301 Moved Permanently
///     - 302 Found
///     - 303 See Other
///     - 307 Temporary Redirect
///
/// Additionally, this policy will will redirect content containing requests
/// such as POST and PUT by converting their request type to a GET.
class DefaultRedirectProcessor: public AbstractRequestResponseFilter
{
public:
    DefaultRedirectProcessor();
    
    virtual ~DefaultRedirectProcessor();

    virtual void requestFilter(Context& context,
                               Request& request) const override;

    virtual void responseFilter(Context& context,
                                Request& request,
                                Response& response) const override;

};


} } // namespace ofx::HTTP
