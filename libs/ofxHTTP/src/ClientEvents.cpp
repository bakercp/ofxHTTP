// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


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
