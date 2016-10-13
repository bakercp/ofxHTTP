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


#include "ofx/HTTP/Client.h"
#include "ofx/HTTP/ClientProgressStream.h"
#include "ofx/HTTP/ClientSessionProvider.h"
#include "ofx/HTTP/DefaultClientHeaders.h"
#include "ofx/HTTP/DefaultRedirectProcessor.h"
#include "ofx/HTTP/DefaultProxyProcessor.h"
#include "ofx/HTTP/DefaultEncodingResponseStreamFilter.h"


namespace ofx {
namespace HTTP {


PixelResponseHandler::PixelResponseHandler()
{
}


PixelResponseHandler::~PixelResponseHandler()
{
}


ofPixels PixelResponseHandler::handleResponse(BaseResponse& response)
{
    ofPixels pixels;
    ofBuffer buffer(response.stream());
    if (ofLoadImage(pixels, buffer))
    {
        return pixels;
    }
    else
    {
        throw Poco::IOException("Unable to parse pixels from buffer.");
    }
}



JSONResponseHandler::JSONResponseHandler()
{
}


JSONResponseHandler::~JSONResponseHandler()
{
}


ofJson JSONResponseHandler::handleResponse(BaseResponse& response)
{
    ofJson json;
    json << response.stream();
    return json;
}


Client::Client():
    _sessionProvider(std::make_unique<ClientSessionProvider>()),
    _defaultRequestHeaders(std::make_unique<DefaultClientHeaders>()),
    _redirectHandler(std::make_unique<DefaultRedirectProcessor>()),
    _proxyHandler(std::make_unique<DefaultProxyProcessor>()),
    _responseEncodingFilter(std::make_unique<DefaultEncodingResponseStreamFilter>())
{
}


Client::~Client()
{
}

// TODO CANCEL TOKEN ... how do we cancel this at various stages?
// during transfer, etc?
// Is it just a matter of closing the socket?
std::unique_ptr<BaseResponse> Client::execute(Context& context,
                                              BaseRequest& request)
{
    ofLogVerbose("Client::execute") << "==== Begin Client Execution ====";

    // Do the request potion of the transaction.
    _doRequest(context, request);

    ofLogVerbose("Client::execute") << "==== Begin Request Headers ====";
    HTTPUtils::dumpHeaders(request, OF_LOG_VERBOSE);
    ofLogVerbose("Client::execute") << "===== End Request Headers =====";

    auto response = std::make_unique<BaseResponse>();

    context.setState(ClientState::RECEIVING_HEADERS);

    context._responseStream = std::make_unique<IO::FilteredInputStream>(context.clientSession()->receiveResponse(*response));

    // Do redirect handling.
    _redirectHandler->responseFilter(context, request, *response);

    // Do proxy handling.
    _proxyHandler->responseFilter(context, request, *response);

    // Apply response filters.
    for (auto& filter: _responseFilters)
        filter->responseFilter(context, request, *response);

    // Make any updates to headers.
    _responseEncodingFilter->responseFilter(context, request, *response);

    context.setState(ClientState::RECEIVING_CONTENTS);

    // Apply response stream filters.
    for (auto& filter: _responseStreamFilters)
    {
        filter->responseStreamFilter(context,
                                     request,
                                     *response,
                                     *context._responseStream);
    }

    // Apply encoding stream filters.
    _responseEncodingFilter->responseStreamFilter(context,
                                                  request,
                                                  *response,
                                                  *context._responseStream);

    context._responseStream->push(std::make_unique<ClientProgressResponseStreamFilter>(context,
                                                                                       request,
                                                                                       *response));

    ofLogVerbose("Client::execute") << "==== Begin Response Headers ====";
    HTTPUtils::dumpHeaders(*response, OF_LOG_VERBOSE);
    ofLogVerbose("Client::execute") << "===== End Response Headers =====";

    if (context.getResubmit())
    {
        ofLogVerbose("Client::execute") << "Resubmitting request.";
        return execute(context, request);
    }
    else
    {
        ofLogVerbose("Client::execute") << "Returning response.";
        response->_responseStream = std::move(context._responseStream);
        return response;
    }
}


void Client::submit(Context& context, BaseRequest& request)
{
    auto response = nullptr;

    try
    {
        auto response = execute(context, request);

        ClientResponseStreamEventArgs args(context,
                                           request,
                                           *response);

        ofNotifyEvent(context.events.onHTTPClientResponseStream, args, this);
    }
    catch (const Poco::SyntaxException& exc)
    {
        ClientErrorEventArgs args(context, request, response, exc);
        ofNotifyEvent(context.events.onHTTPClientError, args, this);
    }
    catch (const Poco::Net::HostNotFoundException& exc)
    {
        ClientErrorEventArgs args(context, request, response, exc);
        ofNotifyEvent(context.events.onHTTPClientError, args, this);
    }
    catch (const Poco::Net::HTTPException& exc)
    {
        ClientErrorEventArgs args(context, request, response, exc);
        ofNotifyEvent(context.events.onHTTPClientError, args, this);
    }
    catch (const Poco::Exception& exc)
    {
        ClientErrorEventArgs args(context, request, response, exc);
        ofNotifyEvent(context.events.onHTTPClientError, args, this);
    }
    catch (const std::exception& exception)
    {
        Poco::Exception exc(exception.what());
        ClientErrorEventArgs args(context, request, response, exc);
        ofNotifyEvent(context.events.onHTTPClientError, args, this);
    }
    catch (...)
    {
        Poco::Exception exc("Completely Unknown Exception");
        ClientErrorEventArgs args(context, request, response, exc);
        ofNotifyEvent(context.events.onHTTPClientError, args, this);
    }
}


void Client::_doRequest(Context& context, BaseRequest& request)
{
    // Apply the request headers.
    _defaultRequestHeaders->requestFilter(context, request);

    // Apply accept-headers.
    _responseEncodingFilter->requestFilter(context, request);

    // Apply request filters.
    for (auto& filter: _requestFilters) filter->requestFilter(context, request);

    // Prepare the request. Depending on the request type, this may involve
    // signing a requests, counting Content-Bytes, etc.
    request.prepareRequest();

    // Set up an HTTPClientSession for this request.
    _sessionProvider->requestFilter(context, request);

    // Do proxy handling.
    _proxyHandler->requestFilter(context, request);

    // Do redirect handling.
    _redirectHandler->requestFilter(context, request);

    // Confirm a valid session.
    if (context.clientSession() == nullptr)
        throw Poco::Exception("No session available for request.");

    // Send the headers.
    context.setState(ClientState::SENDING_HEADERS);

    // Get the request stream.
    std::ostream& requestStream = context.clientSession()->sendRequest(request);

    // Send the request contents.
    context.setState(ClientState::SENDING_CONTENTS);

    ClientProgressRequestStream countingRequestStream(context,
                                                      request,
                                                      requestStream);

    // Write the request body to the output stream.
    request.writeRequestBody(countingRequestStream);
}


} } // namespace ofx::HTTP
