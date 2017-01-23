//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SimpleClient.h"


namespace ofx {
namespace HTTP {


SimpleClient::~SimpleClient()
{
}


//std::unique_ptr<BufferedResponse<GetRequest>> SimpleClient::get(const std::string& uri)
//{
//    return executeBuffered<GetRequest, BufferedResponse<GetRequest>>(std::make_unique<GetRequest>(uri));
//}


} } // namespace ofx::HTTP
