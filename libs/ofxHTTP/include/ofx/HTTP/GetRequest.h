//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/FormRequest.h"


namespace ofx {
namespace HTTP {

    
class GetRequest: public FormRequest
{
public:
    GetRequest(const std::string& uri,
               const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1);

    virtual ~GetRequest();

};

    
} } // namespace ofx::HTTP
