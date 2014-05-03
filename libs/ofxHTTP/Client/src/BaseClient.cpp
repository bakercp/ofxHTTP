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

    RequestFilters::iterator requestFilterIter = _requestFilters.begin();

    while (requestFilterIter != _requestFilters.end())
    {
        (*requestFilterIter)->filter(request, context);
        ++requestFilterIter;
    }

    request.prepareRequest();

    Context::Session session = context.getSession();

    if (!session)
    {
        throw Poco::Exception("No session available for request.");
    }

    std::ostream& requestStream = context.getSession()->sendRequest(request);

//    Poco::TeeOutputStream requestStream(requestStream);
//    outputStream.addStream(pos);

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

    ResponseFilters::iterator responseFilterIter = _responseFilters.begin();

    while (responseFilterIter != _responseFilters.end())
    {
        (*responseFilterIter)->filter(request, response, context);
        ++responseFilterIter;
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

void BaseClient::setRequestStreamFilter(AbstractRequestStreamFilter* requestStreamFilter)
{
    _requestStreamFilter = requestStreamFilter;
    addRequestFilter(_requestStreamFilter);
}


void BaseClient::setResponseStreamFilter(AbstractResponseStreamFilter* responseStreamFilter)
{
    _responseStreamFilter = responseStreamFilter;
    addRequestFilter(_responseStreamFilter);
    addResponseFilter(_responseStreamFilter);
}


void BaseClient::removeRequestStreamFilter()
{
    if (_requestStreamFilter)
    {
        removeRequestFilter(_requestStreamFilter);
    }

    _requestStreamFilter = 0;
}


void BaseClient::removeResponseStreamFilter()
{
    if (_responseStreamFilter)
    {
        removeRequestFilter(_responseStreamFilter);
        removeResponseFilter(_responseStreamFilter);
    }

    _responseStreamFilter = 0;
}


void BaseClient::progress(const BaseRequest& request,
                          Context& context,
                          std::streamsize total)
{
    // NOOP
}

void BaseClient::progress(const BaseRequest& request,
                          const BaseResponse& response,
                          Context& context,
                          std::streamsize total)
{
    // NOOP
}


} } } // namespace ofx::HTTP::Client
