// =============================================================================
//
// Copyright (c) 2014-2015 Christopher Baker <http://christopherbaker.net>
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


BaseRequest& MutableClientRequestArgs::getRequest() const
{
    return _request;
}


Context& MutableClientRequestArgs::getContext()
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


BaseResponse& MutableClientResponseArgs::getResponse()
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


const BaseRequest& BaseClientRequestArgs::getRequest() const
{
    return _request;
}


Context& BaseClientRequestArgs::getContext()
{
    return _context;
}


const std::string BaseClientRequestArgs::getRequestId() const
{
    return _request.getRequestId();
}


BaseProgressArgs::BaseProgressArgs(std::streamsize totalBytesTransferred):
    _totalBytesTransferred(totalBytesTransferred)
{
}


BaseProgressArgs::~BaseProgressArgs()
{
}


std::streamsize BaseProgressArgs::getTotalBytesTransferred() const
{
    return _totalBytesTransferred;
}


float BaseProgressArgs::getProgress() const
{
    std::streamsize contentLength = getContentLength();

    if (Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH == contentLength)
    {
        return Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH;
    }
    else
    {
        return _totalBytesTransferred / (float)contentLength;
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


std::streamsize ClientRequestProgressArgs::getContentLength() const
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


const BaseResponse& BaseClientResponseArgs::getResponse() const
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


std::streamsize ClientResponseProgressArgs::getContentLength() const
{
    return _response.getContentLength();
}


} } // namespace ofx::HTTP
