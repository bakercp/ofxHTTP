//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofJson.h"
#include "ofx/HTTP/Client.h"


namespace ofx {
namespace HTTP {


/// \brief A Simple HTTP cient.
class SimpleClient: public Client
{
public:
    using Client::Client;

    /// \brief Destroy the SimpleClient.
    virtual ~SimpleClient();

    /// \brief GET data from the given URI.
    /// \param uri The endpoint URI.
    /// \returns a buffered response.
//    std::unique_ptr<BufferedResponse<GetRequest>> get(const std::string& uri);

};


} } // namespace ofx::HTTP
