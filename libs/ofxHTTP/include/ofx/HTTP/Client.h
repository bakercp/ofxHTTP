//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofJson.h"
#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/Response.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


template<typename ReturnType>
class ResponseHandler
{
public:
    virtual ~ResponseHandler()
    {
    }

    virtual ReturnType handleResponse(Response& response) = 0;

};


class PixelResponseHandler: public ResponseHandler<ofPixels>
{
public:
    PixelResponseHandler();
    virtual ~PixelResponseHandler();
    virtual ofPixels handleResponse(Response& response) override;
    
};


class JSONResponseHandler: public ResponseHandler<ofJson>
{
public:
    JSONResponseHandler();
    virtual ~JSONResponseHandler();
    virtual ofJson handleResponse(Response& response) override;

};


class BufferResponseHandler: public ResponseHandler<ofBuffer>
{
public:
    BufferResponseHandler();
    virtual ~BufferResponseHandler();
    virtual ofBuffer handleResponse(Response& response) override;
    
};

    
/// \brief A base HTTP client for executing HTTP client requests.
class Client: public AbstractRequestResponseFilter
{
public:
    /// \brief Create a Client.
    Client();

    /// \brief Destroy the Client.
    virtual ~Client();

    /// \brief Access the local context for the client.
    ///
    /// The context is used when a context is not provided to the executing
    /// function.
    ///
    /// \returns a reference to the default context.
    virtual Context& context();

    /// \brief Execute an HTTP Request with the default client context.
    /// \param request The request to execute.
    /// \returns a unique pointer with the response.
    /// \throws Various exceptions.
    std::unique_ptr<Response> execute(Request& request);

    /// \brief Execute an HTTP Request with the given context.
    /// \param context The execution context to use.
    /// \param request The request to execute.
    /// \returns a unique pointer with the response.
    /// \throws Various exceptions.
    std::unique_ptr<Response> execute(Context& context, Request& request);

    /// \brief Execute an HTTP Request with the default client context.
    ///
    /// This does not throw exceptions but sends everything to listeners
    /// in the default context. e.g.
    ///
    ///     context.events.onHTTPClientStateChange
    ///     context.events.onHTTPClientRequestProgress
    ///     context.events.onHTTPClientResponseProgress
    ///     context.events.onHTTPClientResponseStream
    ///     context.events.onHTTPClientError
    ///     ...
    ///
    /// \param request The request to execute.
    void submit(Request& request);

    /// \brief Execute an HTTP Request with the given context.
    ///
    /// This does not throw exceptions but sends everything to listeners
    /// in the given context. e.g.
    ///
    ///     context.events.onHTTPClientStateChange
    ///     context.events.onHTTPClientRequestProgress
    ///     context.events.onHTTPClientResponseProgress
    ///     context.events.onHTTPClientResponseStream
    ///     context.events.onHTTPClientError
    ///     ...
    ///
    /// \param context The execution context to use.
    /// \param request The request to execute.
    void submit(Context& context, Request& request);

    // throws exceptions
    template<typename ReturnType>
    ReturnType execute(Context& context,
                       Request& request,
                       ResponseHandler<ReturnType>& handler)
    {
        return handler.handleResponse(*execute(context, request));
    }

    template<typename ReturnType>
    ReturnType execute(Context& context,
                       Request& request,
                       std::function<ReturnType(Context&,
                                                Request&,
                                                Response&)> f)
    {
        return f(context, request, *execute(context, request));
    }

    template<typename ReturnType>
    ReturnType execute(Context& context,
                       Request& request,
                       std::function<ReturnType(Response&)> f)
    {
        return f(*execute(context, request));
    }
  

protected:
    virtual void requestFilter(Context& context,
                               Request& request) const override;

    virtual void responseFilter(Context& context,
                                Request& request,
                                Response& response) const override;

private:
    void _doRequest(Context& context, Request& request);

    std::unique_ptr<Context> _context = nullptr;

    std::unique_ptr<AbstractClientSessionProvider> _sessionProvider = nullptr;
    std::unique_ptr<AbstractRequestFilter> _defaultRequestHeaders = nullptr;
    std::unique_ptr<AbstractRequestResponseFilter> _redirectHandler = nullptr;
    std::unique_ptr<AbstractRequestResponseFilter> _proxyHandler = nullptr;
    std::unique_ptr<AbstractResponseStreamFilter> _responseEncodingFilter = nullptr;
    std::unique_ptr<AbstractResponseStreamFilter> _cacheProvider = nullptr;

    /// \brief A list of request stream filters.
    std::vector<std::unique_ptr<AbstractRequestStreamFilter>> _requestStreamFilters;

    /// \brief A list of response stream filters.
    std::vector<std::unique_ptr<AbstractResponseStreamFilter>> _responseStreamFilters;

};


} } // namespace ofx::HTTP
