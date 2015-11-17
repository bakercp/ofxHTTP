// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/ClientProgressStream.h"


namespace ofx {
namespace HTTP {


ClientProgressStreamBuf::ClientProgressStreamBuf(std::ostream& ostr,
                                                 const BaseRequest& request,
                                                 Context& context,
                                                 AbstractRequestStreamListener& listener,
                                                 std::streamsize bytesPerProgressUpdate):
    _pRequest(&request),
    _pResponse(0),
    _pContext(&context),
    _pRequestStream(&ostr),
    _pResponseStream(0),
    _pRequestStreamListener(&listener),
    _pResponseStreamListener(0),
    _requestStreamBytes(0),
    _responseStreamBytes(0),
    _bytesPerProgressUpdate(bytesPerProgressUpdate)
{
}


ClientProgressStreamBuf::ClientProgressStreamBuf(std::istream& istr,
                                                 const BaseRequest&,
                                                 const BaseResponse& response,
                                                 Context& context,
                                                 AbstractResponseStreamListener& listener,
                                                 std::streamsize bytesPerProgressUpdate):
    _pRequest(0),
    _pResponse(&response),
    _pContext(&context),
    _pRequestStream(0),
    _pResponseStream(&istr),
    _pRequestStreamListener(0),
    _pResponseStreamListener(&listener),
    _requestStreamBytes(0),
    _responseStreamBytes(0),
    _bytesPerProgressUpdate(bytesPerProgressUpdate)
{
}


ClientProgressStreamBuf::~ClientProgressStreamBuf()
{
}


int ClientProgressStreamBuf::readFromDevice()
{
    if (_pResponseStream)
    {
        int c = _pResponseStream->get();

        if (c != -1)
        {
            ++_responseStreamBytes;

            if (0 == (_responseStreamBytes % _bytesPerProgressUpdate))
            {
                _pResponseStreamListener->progress(*_pRequest,
                                                   *_pResponse,
                                                   *_pContext,
                                                   _responseStreamBytes);
            }
        }

        return c;
    }

    return -1;
}


int ClientProgressStreamBuf::writeToDevice(char c)
{
    ++_requestStreamBytes;

    _pRequestStreamListener->progress(*_pRequest,
                                      *_pContext,
                                      _requestStreamBytes);

    if (_pRequestStream) _pRequestStream->put(c);
    return charToInt(c);
    return c;
}


void ClientProgressStreamBuf::reset()
{
    _requestStreamBytes = 0;
    _responseStreamBytes = 0;
}


ClientProgressIOS::ClientProgressIOS(std::ostream& ostr,
                                     const BaseRequest& request,
                                     Context& context,
                                     AbstractRequestStreamListener& listener,
                                     std::streamsize bytesPerProgressUpdate):
    _buf(ostr, request, context, listener, bytesPerProgressUpdate)
{
    poco_ios_init(&_buf);
}


ClientProgressIOS::ClientProgressIOS(std::istream& istr,
                                     const BaseRequest& request,
                                     const BaseResponse& response,
                                     Context& context,
                                     AbstractResponseStreamListener& listener,
                                     std::streamsize bytesPerProgressUpdate):
    _buf(istr, request, response, context, listener, bytesPerProgressUpdate)
{
    poco_ios_init(&_buf);
}


ClientProgressIOS::~ClientProgressIOS()
{
}


ClientProgressStreamBuf* ClientProgressIOS::rdbuf()
{
    return &_buf;
}


ClientProgressRequestStream::ClientProgressRequestStream(std::ostream& ostr,
                                                         const BaseRequest& request,
                                                         Context& context,
                                                         AbstractRequestStreamListener& listener,
                                                         std::streamsize bytesPerProgressUpdate):
    ClientProgressIOS(ostr, request, context, listener, bytesPerProgressUpdate),
    std::ostream(&_buf)
{
}


ClientProgressRequestStream::~ClientProgressRequestStream()
{
}


ClientProgressResponseStream::ClientProgressResponseStream(std::istream& istr,
                                                           const BaseRequest& request,
                                                           const BaseResponse& response,
                                                           Context& context,
                                                           AbstractResponseStreamListener& listener,
                                                           std::streamsize bytesPerProgressUpdate):
    ClientProgressIOS(istr, request, response, context, listener, bytesPerProgressUpdate),
    std::istream(&_buf)
{
}


ClientProgressResponseStream::~ClientProgressResponseStream()
{
}


} } // namespace ofx::HTTP
