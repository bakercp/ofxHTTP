//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/FormRequest.h"


namespace ofx {
namespace HTTP {

    
class HeadRequest: public FormRequest
{
public:
    HeadRequest(const std::string& uri,
                const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1);

    virtual ~HeadRequest();

};

    
} } // namespace ofx::HTTP
