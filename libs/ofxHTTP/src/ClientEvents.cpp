//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


ClientContextEventArgs::ClientContextEventArgs(Context& context):
    _context(context)
{
}


ClientContextEventArgs::~ClientContextEventArgs()
{
}


const Context& ClientContextEventArgs::context() const
{
    return _context;
}


ClientStateChangeEventArgs::~ClientStateChangeEventArgs()
{
}


ClientState ClientStateChangeEventArgs::state() const
{
    return _context.getState();
}


ClientRequestEventArgs::ClientRequestEventArgs(Context& context,
                                               BaseRequest& request):
    ClientContextEventArgs(context),
    _request(request)
{
}


ClientRequestEventArgs::~ClientRequestEventArgs()
{
}


const BaseRequest& ClientRequestEventArgs::request() const
{
    return _request;
}


ClientResponseEventArgs::ClientResponseEventArgs(Context& context,
                                                 BaseRequest& request,
                                                 BaseResponse& response):
    ClientRequestEventArgs(context, request),
    _response(response)
{
}


ClientResponseEventArgs::~ClientResponseEventArgs()
{
}


const BaseResponse& ClientResponseEventArgs::response() const
{
    return _response;
}


ClientRequestProgressEventArgs::ClientRequestProgressEventArgs(Context& context,
                                                               BaseRequest& request,
                                                               Progress& progress):
    ClientRequestEventArgs(context, request),
    _progress(progress)
{
}


ClientRequestProgressEventArgs::~ClientRequestProgressEventArgs()
{
}


const Progress& ClientRequestProgressEventArgs::progress() const
{
    return _progress;
}


ClientResponseProgressEventArgs::ClientResponseProgressEventArgs(Context& context,
                                                                 BaseRequest& request,
                                                                 BaseResponse& response,
                                                                 Progress& progress):
    ClientResponseEventArgs(context, request, response),
    _progress(progress)
{
}


ClientResponseProgressEventArgs::~ClientResponseProgressEventArgs()
{
}


const Progress& ClientResponseProgressEventArgs::progress() const
{
    return _progress;
}


ClientResponseStreamEventArgs::~ClientResponseStreamEventArgs()
{
}


std::istream& ClientResponseStreamEventArgs::stream()
{
    return _response.stream();
}


ClientErrorEventArgs::ClientErrorEventArgs(Context& context,
                                           BaseRequest& request,
                                           BaseResponse* response,
                                           const Poco::Exception& exception):
    ClientRequestEventArgs(context, request),
    _response(response),
    _exception(exception)
{
}

        
ClientErrorEventArgs::~ClientErrorEventArgs()
{
}


const BaseResponse* ClientErrorEventArgs::response() const
{
    return _response;
}

        
const Poco::Exception& ClientErrorEventArgs::exception() const
{
    return _exception;
}


} } // namespace ofx::HTTP
