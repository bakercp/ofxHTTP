//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/RequestHandlerAdapter.h"


namespace ofx {
namespace HTTP {


RequestHandlerAdapter::RequestHandlerAdapter(Poco::Net::HTTPRequestHandler& handler):
    _handler(handler)
{
//    std::cout << "CREATE RequestHandlerAdapter handling ..." << std::endl;
}


RequestHandlerAdapter::~RequestHandlerAdapter()
{
//    std::cout << "DESTROY RequestHandlerAdapter handling ..." << std::endl;
}


void RequestHandlerAdapter::handleRequest(Poco::Net::HTTPServerRequest& request,
                                          Poco::Net::HTTPServerResponse& response)
{
//    std::cout << "RequestHandlerAdapter handling ..." << std::endl;

    try
    {
        _handler.handleRequest(request, response);
    }
    catch (...)
    {
//        std::cout << "CAUGHT AN EXEPTION HERE!" << std::endl;
    }
}


} } // namespace ofx::HTTP
