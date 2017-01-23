//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/GetRequest.h"


namespace ofx {
namespace HTTP {


GetRequest::GetRequest(const std::string& uri,
                       const std::string& httpVersion):
    FormRequest(Poco::Net::HTTPRequest::HTTP_GET,
                uri,
                httpVersion)
{
}


GetRequest::~GetRequest()
{
}


} } // namespace ofx::HTTP
