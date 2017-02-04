//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Response.h"
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
                                               Request& request):
    ClientContextEventArgs(context),
    _request(request)
{
}


ClientRequestEventArgs::~ClientRequestEventArgs()
{
}


const Request& ClientRequestEventArgs::request() const
{
    return _request;
}


ClientResponseEventArgs::ClientResponseEventArgs(Context& context,
                                                 Request& request,
                                                 Response& response):
    ClientRequestEventArgs(context, request),
    _response(response)
{
}


ClientResponseEventArgs::~ClientResponseEventArgs()
{
}


const Response& ClientResponseEventArgs::response() const
{
    return _response;
}


ClientRequestProgressEventArgs::ClientRequestProgressEventArgs(Context& context,
                                                               Request& request,
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
                                                                 Request& request,
                                                                 Response& response,
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
                                           Request& request,
                                           Response* response,
                                           const Poco::Exception& exception):
    ClientRequestEventArgs(context, request),
    _response(response),
    _exception(exception)
{
}

        
ClientErrorEventArgs::~ClientErrorEventArgs()
{
}


const Response* ClientErrorEventArgs::response() const
{
    return _response;
}

        
const Poco::Exception& ClientErrorEventArgs::exception() const
{
    return _exception;
}


} } // namespace ofx::HTTP
