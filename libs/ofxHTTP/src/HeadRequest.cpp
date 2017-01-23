//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/HeadRequest.h"


namespace ofx {
namespace HTTP {


HeadRequest::HeadRequest(const std::string& uri,
                       const std::string& httpVersion):
    FormRequest(Poco::Net::HTTPRequest::HTTP_HEAD,
                uri,
                httpVersion)
{
}


HeadRequest::~HeadRequest()
{
}


} } // namespace ofx::HTTP
