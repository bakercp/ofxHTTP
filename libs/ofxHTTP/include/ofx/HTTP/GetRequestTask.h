//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Task.h"
#include "Poco/TaskNotification.h"
#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/Client.h"
#include "ofx/HTTP/ThreadSettings.h"


namespace ofx {
namespace HTTP {


class GetRequestTask: public Poco::Task
{
public:
    GetRequestTask(const std::string& uri, const std::string& cachePath = "");

    virtual ~GetRequestTask();

    void runTask();

    bool onHTTPClientStateChange(ClientStateChangeEventArgs& args);

    bool onHTTPClientRequestProgress(ClientRequestProgressEventArgs& args);
    bool onHTTPClientResponseProgress(ClientResponseProgressEventArgs& args);

    bool onHTTPClientResponseStream(ClientResponseStreamEventArgs& args);
    bool onHTTPClientError(ClientErrorEventArgs& args);

private:
    std::string _uri;
    std::string _cachePath;
};


} } // namespace ofx::HTTP
