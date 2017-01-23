//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


class DefaultEncodingResponseStreamFilter: public AbstractResponseStreamFilter
{
public:
    DefaultEncodingResponseStreamFilter();
    virtual ~DefaultEncodingResponseStreamFilter();
    
    void requestFilter(Context& context, BaseRequest& request) const override;

    void responseFilter(Context& context,
                        BaseRequest& request,
                        BaseResponse& response) const override;

    void responseStreamFilter(Context& context,
                              const BaseRequest& request,
                              const BaseResponse& response,
                              IO::FilteredInputStream& responseStream) const override;

    /// \brief The "Accept-Encoding" header key.
    static const std::string ACCEPT_ENCODING_HEADER;

    /// \brief The "Content-Encoding" header key.
    static const std::string CONTENT_ENCODING_HEADER;
    
};


} } // namespace ofx::HTTP
