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


namespace ofx {
namespace HTTP {


MutableClientRequestArgs::MutableClientRequestArgs(BaseRequest& request,
                                                   Context& context):
    _request(request),
    _context(context)
{
}


MutableClientRequestArgs::~MutableClientRequestArgs()
{
}


BaseRequest& MutableClientRequestArgs::request() const
{
    return _request;
}


Context& MutableClientRequestArgs::context()
{
    return _context;
}


MutableClientResponseArgs::MutableClientResponseArgs(BaseRequest& request,
                                                     BaseResponse& response,
                                                     Context& context):
    MutableClientRequestArgs(request, context),
    _response(response)
{
}


MutableClientResponseArgs::~MutableClientResponseArgs()
{
}


BaseResponse& MutableClientResponseArgs::response()
{
    return _response;
}


BaseClientRequestArgs::BaseClientRequestArgs(const BaseRequest& request,
                                             Context& context):
    _request(request),
    _context(context)
{
}


BaseClientRequestArgs::~BaseClientRequestArgs()
{
}


const BaseRequest& BaseClientRequestArgs::request() const
{
    return _request;
}


Context& BaseClientRequestArgs::context()
{
    return _context;
}


std::string BaseClientRequestArgs::requestId() const
{
    return _request.getRequestId();
}


const int BaseProgressArgs::UNKNOWN_CONTENT_LENGTH = Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH;


BaseProgressArgs::BaseProgressArgs(std::streamsize totalBytesTransferred):
    _totalBytesTransferred(totalBytesTransferred)
{
}


BaseProgressArgs::~BaseProgressArgs()
{
}


std::streamsize BaseProgressArgs::totalBytesTransferred() const
{
    return _totalBytesTransferred;
}


float BaseProgressArgs::progress() const
{
    std::streamsize _contentLength = contentLength();

    if (UNKNOWN_CONTENT_LENGTH == _contentLength)
    {
        return UNKNOWN_CONTENT_LENGTH;
    }
    else
    {
        return _totalBytesTransferred / static_cast<float>(_contentLength);
    }
}


ClientRequestProgressArgs::ClientRequestProgressArgs(const BaseRequest& request,
                                                     Context& context,
                                                     std::streamsize totalBytesTransferred):
    BaseClientRequestArgs(request, context),
    BaseProgressArgs(totalBytesTransferred)
{
}


ClientRequestProgressArgs::~ClientRequestProgressArgs()
{
}


std::streamsize ClientRequestProgressArgs::contentLength() const
{
    return _request.getContentLength();
}


BaseClientResponseArgs::BaseClientResponseArgs(const BaseRequest& request,
                                               const BaseResponse& response,
                                               Context& context):
    BaseClientRequestArgs(request, context),
    _response(response)
{
}


BaseClientResponseArgs::~BaseClientResponseArgs()
{
}


const BaseResponse& BaseClientResponseArgs::response() const
{
    return _response;
}


ClientResponseProgressArgs::ClientResponseProgressArgs(const BaseRequest& request,
                                                       const BaseResponse& response,
                                                       Context& context,
                                                       std::streamsize bytesTransferred):
    BaseClientResponseArgs(request, response, context),
    BaseProgressArgs(bytesTransferred)
{
}


ClientResponseProgressArgs::~ClientResponseProgressArgs()
{
}


std::streamsize ClientResponseProgressArgs::contentLength() const
{
    return _response.getContentLength();
}



ClientResponseEventArgs::ClientResponseEventArgs(std::istream& responseStream,
                                                 const BaseRequest& request,
                                                 const BaseResponse& response,
                                                 Context& context):
    BaseClientResponseArgs(request, response, context),
    _responseStream(responseStream)
{
}


ClientResponseEventArgs::~ClientResponseEventArgs()
{
}


std::istream& ClientResponseEventArgs::responseStream() const
{
    return _responseStream;
}



ClientResponseBufferEventArgs::ClientResponseBufferEventArgs(const IO::ByteBuffer& buffer,
                                                             const BaseRequest& request,
                                                             const BaseResponse& response,
                                                             Context& context):
    BaseClientResponseArgs(request, response, context),
    _buffer(buffer)
{
}


ClientResponseBufferEventArgs::~ClientResponseBufferEventArgs()
{
}


const IO::ByteBuffer& ClientResponseBufferEventArgs::buffer() const
{
    return _buffer;
}


ClientErrorEventArgs::ClientErrorEventArgs(const BaseRequest& request,
                                           const BaseResponse& response,
                                           Context& context,
                                           const Poco::Exception& exception):
    BaseClientResponseArgs(request, response, context),
    _exception(exception)
{
}

        
ClientErrorEventArgs::~ClientErrorEventArgs()
{
}
        
        
const Poco::Exception& ClientErrorEventArgs::exception() const
{
    return _exception;
}


} } // namespace ofx::HTTP
