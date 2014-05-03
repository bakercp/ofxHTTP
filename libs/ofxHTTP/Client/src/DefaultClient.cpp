// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/Client/BaseClient.h"
#include "Poco/InflatingStream.h"


namespace ofx {
namespace HTTP {
namespace Client {


BaseClient::BaseClient():
    _requestStreamFilter(0),
    _responseStreamFilter(0)
{
}


BaseClient::~BaseClient()
{
    // we do not own the filter pointers.
}


std::istream& BaseClient::execute(Client::BaseRequest& request,
                                  Client::BaseResponse& response,
                                  Context& context)
{
    context.setResubmit(false);

    RequestProcessors::iterator requestHandlerIter = _requestProcessors.begin();

    while (requestHandlerIter != _requestProcessors.end())
    {
        (*requestHandlerIter)->processRequest(request, context);
        ++requestHandlerIter;
    }

    request.prepareRequest();

    Context::Session session = context.getSession();

    if (!session)
    {
        throw Poco::Exception("No session available for request.");
    }

    std::ostream& requestStream = context.getSession()->sendRequest(request);

    if (_requestStreamFilter)
    {
        request.writeRequestBody(_requestStreamFilter->filter(requestStream,
                                                              request,
                                                              context));
    }
    else
    {
        request.writeRequestBody(requestStream);
    }

    std::istream& responseStream = context.getSession()->receiveResponse(response);

    // apply response filters

    ResponseHandlers::iterator responseHandlerIter = _responseHandlers.begin();

    while (responseHandlerIter != _responseHandlers.end())
    {
        (*responseHandlerIter)->handleResponse(request, response, context);
        ++responseHandlerIter;
    }

    if (context.getResubmit())
    {
        // If a response handler did not reset the session, then we must
        // consume the stream in order to reuse the HTTP session.
        if (session)
        {
            Utils::consume(responseStream);
        }

        return execute(request, response, context);

    }
    else
    {
        if (_responseStreamFilter)
        {
            return _responseStreamFilter->filter(responseStream,
                                                 request,
                                                 response,
                                                 context);
        }
        else
        {

            return responseStream;
        }
    }
}


void BaseClient::addRequestProcessor(AbstractRequestProcessor* processor)
{
    _requestProcessors.push_back(processor);
}


void BaseClient::addResponseHandler(AbstractResponseHandler* handler)
{
    _responseHandlers.push_back(handler);
}


void BaseClient::removeRequestProcessor(AbstractRequestProcessor* processor)
{
    RequestProcessors::iterator iter = std::find(_requestProcessors.begin(),
                                                 _requestProcessors.end(),
                                                 processor);

    if (iter != _requestProcessors.end())
    {
        _requestProcessors.erase(iter);
    }
}


void BaseClient::removeResponseHandler(AbstractResponseHandler* handler)
{
    ResponseHandlers::iterator iter = std::find(_responseHandlers.begin(),
                                                _responseHandlers.end(),
                                                handler);
    
    if (iter != _responseHandlers.end())
    {
        _responseHandlers.erase(iter);
    }
}

void BaseClient::setRequestStreamFilter(AbstractRequestStreamFilter* requestStreamFilter)
{
    _requestStreamFilter = requestStreamFilter;
    addRequestProcessor(_requestStreamFilter);
}


void BaseClient::setResponseStreamFilter(AbstractResponseStreamFilter* responseStreamFilter)
{
    _responseStreamFilter = responseStreamFilter;
    addRequestProcessor(_responseStreamFilter);
    addResponseHandler(_responseStreamFilter);
}


void BaseClient::removeRequestStreamFilter()
{
    if (_requestStreamFilter)
    {
        removeRequestProcessor(_requestStreamFilter);
    }

    _requestStreamFilter = 0;
}


void BaseClient::removeResponseStreamFilter()
{
    if (_responseStreamFilter)
    {
        removeRequestProcessor(_responseStreamFilter);
        removeResponseHandler(_responseStreamFilter);
    }

    _responseStreamFilter = 0;
}


} } } // namespace ofx::HTTP::Client
