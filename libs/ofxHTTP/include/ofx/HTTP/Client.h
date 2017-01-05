//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofJson.h"
#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


template<typename ReturnType>
class ResponseHandler
{
public:
    virtual ReturnType handleResponse(BaseResponse& response) = 0;
};


class PixelResponseHandler: public ResponseHandler<ofPixels>
{
public:
    PixelResponseHandler();
    virtual ~PixelResponseHandler();
    virtual ofPixels handleResponse(BaseResponse& response) override;
    
};


class JSONResponseHandler: public ResponseHandler<ofJson>
{
public:
    JSONResponseHandler();
    virtual ~JSONResponseHandler();
    virtual ofJson handleResponse(BaseResponse& response) override;

};


/// \brief A base HTTP cient for executing HTTP client requests.
class Client
{
public:
    /// \brief Create a generic BaseClient.
    Client();

    /// \brief Destroy the BaseClient.
    virtual ~Client();

    // throws exceptions
    std::unique_ptr<BaseResponse> execute(Context& context,
                                          BaseRequest& request);


    // does not throw exceptions, rather sends everything to listeners
    void submit(Context& context, BaseRequest& request);


    // throws exceptions
    template<typename ReturnType>
    ReturnType execute(Context& context,
                       BaseRequest& request,
                       ResponseHandler<ReturnType>& handler)
    {
        auto response = execute(context, request);
        auto result = handler.handleResponse(*response);
        return result;
    }


protected:
    void _doRequest(Context& context, BaseRequest& request);


private:
    std::unique_ptr<AbstractClientSessionProvider> _sessionProvider = nullptr;
    std::unique_ptr<AbstractRequestFilter> _defaultRequestHeaders = nullptr;
    std::unique_ptr<AbstractRequestResponseFilter> _redirectHandler = nullptr;
    std::unique_ptr<AbstractRequestResponseFilter> _proxyHandler = nullptr;
    std::unique_ptr<AbstractResponseStreamFilter> _responseEncodingFilter = nullptr;
    std::unique_ptr<AbstractResponseStreamFilter> _cacheProvider = nullptr;

    /// \brief A list of request filters.
    ///
    /// These filters are automatically applied to the outgoing request headers.
    std::vector<std::unique_ptr<AbstractRequestFilter>> _requestFilters;

    /// \brief A list of response filters.
    ///
    /// These filters are automatically applied to the incoming response headers.
    std::vector<std::unique_ptr<AbstractResponseFilter>> _responseFilters;

    /// \brief A list of request stream filters.
    std::vector<std::unique_ptr<AbstractRequestStreamFilter>> _requestStreamFilters;

    /// \brief A list of response stream filters.
    std::vector<std::unique_ptr<AbstractResponseStreamFilter>> _responseStreamFilters;

};


} } // namespace ofx::HTTP
