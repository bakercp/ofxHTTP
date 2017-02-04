//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/AbstractClientTypes.h"


namespace ofx {
namespace HTTP {


class DefaultProxyProcessor: public AbstractRequestResponseFilter
{
public:
    DefaultProxyProcessor();

    virtual ~DefaultProxyProcessor();

    virtual void requestFilter(Context& context, Request& request) const override;

    virtual void responseFilter(Context& context,
                                Request& request,
                                Response& response) const override;

};


} } // namespace ofx::HTTP
