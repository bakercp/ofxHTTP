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


#include "ofx/HTTP/BaseClient.h"


namespace ofx {
namespace HTTP {


BaseClient::BaseClient():
    _bytesPerProgressUpdate(DEFAULT_BYTES_PER_PROGRESS_UPDATE),
    _pRequestStreamFilter(nullptr),
    _pResponseStreamFilter(nullptr)
{
}


BaseClient::BaseClient(RequestFilters requestFilters,
                       ResponseFilters responseFilters,
                       streamsize bytesPerProgressUpdate):
    _requestFilters(requestFilters),
    _responseFilters(responseFilters),
    _bytesPerProgressUpdate(bytesPerProgressUpdate),
    _pRequestStreamFilter(nullptr),
    _pResponseStreamFilter(nullptr)
{
}


BaseClient::~BaseClient()
{
    // We do not own the filter pointers.
}


std::istream& BaseClient::execute(BaseRequest& request,
                                  BaseResponse& response,
                                  Context& context)
{
    context.setResubmit(false);

    requestFilter(request, context);

    request.prepareRequest();

    std::ostream& requestStream = send(request, context);

    request.writeRequestBody(requestStream);

    std::istream& responseStream = receive(request, response, context);

    responseFilter(request, response, context);

    if (context.getResubmit())
    {
        // If a response handler did not reset the session, then we must
        // consume the stream in order to reuse the HTTP session.
        if (context.getClientSession())
        {
            HTTPUtils::consume(responseStream);
        }

        return execute(request, response, context);
    }
    else
    {
        return responseStream;
    }
}


void BaseClient::submit(BaseRequest& request,
                        BaseResponse& response,
                        Context& context)
{
    try
    {
        std::istream& responseStream = execute(request,
                                               response,
                                               context);

        ClientResponseEventArgs evt(responseStream,
                                    request,
                                    response,
                                    context);

        ofNotifyEvent(events.onHTTPClientResponseEvent, evt);
    }
    catch (const Poco::SyntaxException& exc)
    {
        ClientErrorEventArgs evt(request, response, context, exc);
        ofNotifyEvent(events.onHTTPClientErrorEvent, evt);
    }
    catch (const Poco::Net::HostNotFoundException& exc)
    {
        ClientErrorEventArgs evt(request, response, context, exc);
        ofNotifyEvent(events.onHTTPClientErrorEvent, evt);
    }
    catch (const Poco::Net::HTTPException& exc)
    {
        ClientErrorEventArgs evt(request, response, context, exc);
        ofNotifyEvent(events.onHTTPClientErrorEvent, evt);
    }
    catch (const Poco::Exception& exc)
    {
        ClientErrorEventArgs evt(request, response, context, exc);
        ofNotifyEvent(events.onHTTPClientErrorEvent, evt);
    }
    catch (std::exception& exception)
    {
        Poco::Exception exc(exception.what());
        ClientErrorEventArgs evt(request, response, context, exc);
        ofNotifyEvent(events.onHTTPClientErrorEvent, evt);
    }
    catch (...)
    {
        Poco::Exception exc("Completely Unknown Exception");
        ClientErrorEventArgs evt(request, response, context, exc);
        ofNotifyEvent(events.onHTTPClientErrorEvent, evt);
    }
}


void BaseClient::addRequestFilter(AbstractRequestFilter* filter)
{
    _requestFilters.push_back(filter);
}


void BaseClient::addResponseFilter(AbstractResponseFilter* filter)
{
    _responseFilters.push_back(filter);
}


void BaseClient::removeRequestFilter(AbstractRequestFilter* filter)
{
    RequestFilters::iterator iter = std::find(_requestFilters.begin(),
                                              _requestFilters.end(),
                                              filter);

    if (iter != _requestFilters.end())
    {
        _requestFilters.erase(iter);
    }
}


void BaseClient::removeResponseFilter(AbstractResponseFilter* handler)
{
    ResponseFilters::iterator iter = std::find(_responseFilters.begin(),
                                               _responseFilters.end(),
                                               handler);
    
    if (iter != _responseFilters.end())
    {
        _responseFilters.erase(iter);
    }
}


void BaseClient::setRequestStreamFilter(AbstractRequestStreamFilter* pRequestStreamFilter)
{
    _pRequestStreamFilter = pRequestStreamFilter;
    addRequestFilter(_pRequestStreamFilter);
}


void BaseClient::setResponseStreamFilter(AbstractResponseStreamFilter* pResponseStreamFilter)
{
    _pResponseStreamFilter = pResponseStreamFilter;
    addRequestFilter(_pResponseStreamFilter);
    addResponseFilter(_pResponseStreamFilter);
}


void BaseClient::removeRequestStreamFilter()
{
    if (_pRequestStreamFilter)
    {
        removeRequestFilter(_pRequestStreamFilter);
    }

    _pRequestStreamFilter = 0;
}


void BaseClient::removeResponseStreamFilter()
{
    if (_pResponseStreamFilter)
    {
        removeRequestFilter(_pResponseStreamFilter);
        removeResponseFilter(_pResponseStreamFilter);
    }

    _pResponseStreamFilter = 0;
}


void BaseClient::setBytesPerProgressUpdate(std::streamsize bytesPerProgressUpdate)
{
    _bytesPerProgressUpdate = bytesPerProgressUpdate;
}


std::streamsize BaseClient::getBytesPerProgressUpdate() const
{
    return _bytesPerProgressUpdate;
}


void BaseClient::requestFilter(BaseRequest& request, Context& context)
{
    RequestFilters::iterator requestFilterIter = _requestFilters.begin();

    while (requestFilterIter != _requestFilters.end())
    {
        (*requestFilterIter)->requestFilter(request, context);
        ++requestFilterIter;
    }

    MutableClientRequestArgs requestFilterEvent(request, context);
    ofNotifyEvent(events.onHTTPClientRequestFilterEvent, requestFilterEvent);
}


void BaseClient::responseFilter(BaseRequest& request,
                                BaseResponse& response,
                                Context& context)
{
    // Apply attached filters.
    ResponseFilters::iterator responseFilterIter = _responseFilters.begin();

    while (responseFilterIter != _responseFilters.end())
    {
        (*responseFilterIter)->responseFilter(request, response, context);
        ++responseFilterIter;
    }

    // Apply event based filters.
    MutableClientResponseArgs responseFilterEvent(request, response, context);
    ofNotifyEvent(events.onHTTPClientResponseFilterEvent, responseFilterEvent);
}


std::ostream& BaseClient::send(BaseRequest& request, Context& context)
{
    Context::ClientSession clientSession = context.getClientSession();

    if (clientSession == nullptr)
    {
        throw Poco::Exception("No session available for request.");
    }

    std::ostream& rawRequestStream = clientSession->sendRequest(request);


    _pClientProgressRequestStream = std::make_shared<ClientProgressRequestStream>(rawRequestStream,
                                                                                  request,
                                                                                  context,
                                                                                  *this,
                                                                                  _bytesPerProgressUpdate);

    if (_pRequestStreamFilter)
    {
        return _pRequestStreamFilter->requestStreamFilter(*_pClientProgressRequestStream,
                                                          request,
                                                          context);
    }
    else
    {
        return *_pClientProgressRequestStream;
    }
}


std::istream& BaseClient::receive(BaseRequest& request,
                                  BaseResponse& response,
                                  Context& context)
{
    Context::ClientSession clientSession = context.getClientSession();

    if (!clientSession)
    {
        throw Poco::Exception("No session available for request.");
    }

    std::istream& rawResponseStream = clientSession->receiveResponse(response);

    _pClientProgressResponseStream = std::make_shared<ClientProgressResponseStream>(rawResponseStream,
                                                                                    request,
                                                                                    response,
                                                                                    context,
                                                                                    *this,
                                                                                    _bytesPerProgressUpdate);

    if (_pResponseStreamFilter)
    {
        return _pResponseStreamFilter->responseStreamFilter(*_pClientProgressResponseStream,
                                                            request,
                                                            response,
                                                            context);
    }
    else
    {

        return *_pClientProgressResponseStream;
    }
}


void BaseClient::progress(const BaseRequest& request,
                          Context& context,
                          std::streamsize totalBytesTransferred)
{
    ClientRequestProgressArgs evt(request, context, totalBytesTransferred);

    ofNotifyEvent(events.onHTTPClientRequestProgress, evt);
}


void BaseClient::progress(const BaseRequest& request,
                          const BaseResponse& response,
                          Context& context,
                          std::streamsize totalBytesTransferred)
{
    ClientResponseProgressArgs evt(request,
                                   response,
                                   context,
                                   totalBytesTransferred);

    ofNotifyEvent(events.onHTTPClientResponseProgress, evt);
}


} } // namespace ofx::HTTP
