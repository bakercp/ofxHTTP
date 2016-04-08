// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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


BaseClient::BaseClient(std::vector<AbstractRequestFilter*> requestFilters,
                       std::vector<AbstractResponseFilter*> responseFilters,
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

    _filterRequest(request, context);

    request.prepareRequest();

    std::ostream& requestStream = _send(request, context);

    request.writeRequestBody(requestStream);

    std::istream& responseStream = _receive(request, response, context);

    _filterResponse(request, response, context);

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

std::istream& BaseClient::execute(BaseRequest& request,
                                  BaseResponse& response)
{
    Context context;
    return execute(request, response, context);
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
    catch (const std::exception& exception)
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


void BaseClient::submit(BaseRequest& request, BaseResponse& response)
{
    Context context;
    submit(request, response, context);
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
    _requestFilters.erase(std::remove(_requestFilters.begin(),
                                      _requestFilters.end(),
                                      filter),
                           _requestFilters.end());
}


void BaseClient::removeResponseFilter(AbstractResponseFilter* filter)
{
    _responseFilters.erase(std::remove(_responseFilters.begin(),
                                       _responseFilters.end(),
                                       filter),
                           _responseFilters.end());
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

    _pRequestStreamFilter = nullptr;
}


void BaseClient::removeResponseStreamFilter()
{
    if (_pResponseStreamFilter)
    {
        removeRequestFilter(_pResponseStreamFilter);
        removeResponseFilter(_pResponseStreamFilter);
    }

    _pResponseStreamFilter = nullptr;
}


void BaseClient::setBytesPerProgressUpdate(std::streamsize bytesPerProgressUpdate)
{
    _bytesPerProgressUpdate = bytesPerProgressUpdate;
}


std::streamsize BaseClient::getBytesPerProgressUpdate() const
{
    return _bytesPerProgressUpdate;
}


void BaseClient::_filterRequest(BaseRequest& request, Context& context)
{
    for (auto& filter: _requestFilters)
    {
        filter->requestFilter(request, context);
    }

    MutableClientRequestArgs requestFilterEvent(request, context);
    ofNotifyEvent(events.onHTTPClientRequestFilterEvent, requestFilterEvent);
}


void BaseClient::_filterResponse(BaseRequest& request,
                                 BaseResponse& response,
                                 Context& context)
{
    for (auto& filter: _responseFilters)
    {
        filter->responseFilter(request, response, context);
    }

    // Apply event based filters.
    MutableClientResponseArgs responseFilterEvent(request, response, context);
    ofNotifyEvent(events.onHTTPClientResponseFilterEvent, responseFilterEvent);
}


std::ostream& BaseClient::_send(BaseRequest& request, Context& context)
{
    auto clientSession = context.getClientSession();

    if (clientSession == nullptr)
    {
        throw Poco::Exception("No session available for request.");
    }

    std::ostream& rawRequestStream = clientSession->sendRequest(request);


    _pClientProgressRequestStream = std::make_unique<ClientProgressRequestStream>(rawRequestStream,
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


std::istream& BaseClient::_receive(BaseRequest& request,
                                   BaseResponse& response,
                                   Context& context)
{
    auto clientSession = context.getClientSession();

    if (!clientSession)
    {
        throw Poco::Exception("No session available for request.");
    }

    std::istream& rawResponseStream = clientSession->receiveResponse(response);

    _pClientProgressResponseStream = std::make_unique<ClientProgressResponseStream>(rawResponseStream,
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
