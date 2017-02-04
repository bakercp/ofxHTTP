//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ClientProgressStream.h"


namespace ofx {
namespace HTTP {


ClientProgressStreamBuf::ClientProgressStreamBuf(Context& context,
                                                 Request& request,
                                                 std::ostream& ostr):
    _pContext(&context),
    _pRequest(&request),
    _pRequestStream(&ostr),
    _requestStreamProgress(_pRequest->estimatedContentLength()),
    _bytesPerUpdate(_pContext->getClientSessionSettings().getMinimumBytesPerProgressUpdate()),
    _maxUpdateInterval(_pContext->getClientSessionSettings().getMaximumProgressUpdateInterval().totalMicroseconds())
{
    ClientRequestProgressEventArgs args(*_pContext, *_pRequest, _requestStreamProgress);
    ofNotifyEvent(_pContext->events.onHTTPClientRequestProgress, args, this);
}


ClientProgressStreamBuf::ClientProgressStreamBuf(Context& context,
                                                 Request& request,
                                                 Response& response,
                                                 std::istream& istr):
    _pContext(&context),
    _pRequest(&request),
    _pResponse(&response),
    _pResponseStream(&istr),
    _responseStreamProgress(_pResponse->estimatedContentLength()),
    _bytesPerUpdate(_pContext->getClientSessionSettings().getMinimumBytesPerProgressUpdate()),
    _maxUpdateInterval(_pContext->getClientSessionSettings().getMaximumProgressUpdateInterval().totalMicroseconds())
{
    ClientResponseProgressEventArgs args(*_pContext, *_pRequest, *_pResponse, _responseStreamProgress);
    ofNotifyEvent(_pContext->events.onHTTPClientResponseProgress, args, this);
}


ClientProgressStreamBuf::~ClientProgressStreamBuf()
{
    // We do not own any pointers.
}


int ClientProgressStreamBuf::writeToDevice(char c)
{
    if (_pRequestStream != nullptr)
    {
        _pRequestStream->put(c);

        ++_requestStreamBytesTransferred;

        if (_bytesPerUpdate > 0)
        {
            if ((_requestStreamBytesTransferred - _requestStreamProgress.getTotalBytesTranferred()) >= _bytesPerUpdate
                || _requestStreamProgress.lastUpdateTime().elapsed() >= _maxUpdateInterval
                || _requestStreamBytesTransferred == _requestStreamProgress.getTotalBytes())
            {
                _requestStreamProgress.setTotalBytesTransferred(_requestStreamBytesTransferred);

                ClientRequestProgressEventArgs args(*_pContext,
                                                    *_pRequest,
                                                    _requestStreamProgress);

                ofNotifyEvent(_pContext->events.onHTTPClientRequestProgress,
                              args,
                              this);
            }
        }
    }

    return charToInt(c);
}



int ClientProgressStreamBuf::readFromDevice()
{
    if (_pResponseStream != nullptr)
    {
        auto c = _pResponseStream->get();

        if (c != -1)
        {
            ++_responseStreamBytesTransferred;
            
            if (_bytesPerUpdate > 0)
            {
                if ((_responseStreamBytesTransferred - _responseStreamProgress.getTotalBytesTranferred()) >= _bytesPerUpdate
                  || _responseStreamProgress.lastUpdateTime().elapsed() >= _maxUpdateInterval
                  || _responseStreamBytesTransferred == _responseStreamProgress.getTotalBytes())
                {
                    _responseStreamProgress.setTotalBytesTransferred(_responseStreamBytesTransferred);

                    ClientResponseProgressEventArgs args(*_pContext,
                                                         *_pRequest,
                                                         *_pResponse,
                                                         _responseStreamProgress);

                    ofNotifyEvent(_pContext->events.onHTTPClientResponseProgress,
                                  args,
                                  this);
                }
            }
        }

        return c;
    }

    return -1;
}


ClientProgressIOS::ClientProgressIOS(Context& context,
                                     Request& request,
                                     std::ostream& ostr):
    _buf(context, request, ostr)
{
    poco_ios_init(&_buf);
}


ClientProgressIOS::ClientProgressIOS(Context& context,
                                     Request& request,
                                     Response& response,
                                     std::istream& istr):
    _buf(context, request, response, istr)
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


ClientProgressRequestStream::ClientProgressRequestStream(Context& context,
                                                         Request& request,
                                                         std::ostream& ostr):
    ClientProgressIOS(context, request, ostr),
    std::ostream(&_buf)
{
}


ClientProgressRequestStream::~ClientProgressRequestStream()
{
}


ClientProgressResponseStream::ClientProgressResponseStream(Context& context,
                                                           Request& request,
                                                           Response& response,
                                                           std::istream& istr):
    ClientProgressIOS(context, request, response, istr),
    std::istream(&_buf)
{
}


ClientProgressResponseStream::~ClientProgressResponseStream()
{
}


ClientProgressRequestStreamFilter::ClientProgressRequestStreamFilter(Context& context,
                                                                     Request& request):
    _context(context),
    _request(request)
{
}


ClientProgressRequestStreamFilter::~ClientProgressRequestStreamFilter()
{
}


std::ostream& ClientProgressRequestStreamFilter::filter(std::ostream& stream)
{
    _stream = std::make_unique<ClientProgressRequestStream>(_context,
                                                            _request,
                                                            stream);
    return *_stream;
}


ClientProgressResponseStreamFilter::ClientProgressResponseStreamFilter(Context& context,
                                                                       Request& request,
                                                                       Response& response):
    _context(context),
    _request(request),
    _response(response)
{
}


ClientProgressResponseStreamFilter::~ClientProgressResponseStreamFilter()
{
}


std::istream& ClientProgressResponseStreamFilter::filter(std::istream& stream)
{
    _stream = std::make_unique<ClientProgressResponseStream>(_context,
                                                             _request,
                                                             _response,
                                                             stream);
    return *_stream;
}


} } // namespace ofx::HTTP
