//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//
//
//
//#include "ofx/HTTP/HTTPClient.h"
//#include "ofx/HTTP/GetRequest.h"
//#include "ofx/HTTP/JSONRequest.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
//HTTPClient::HTTPClient()
//{
//    addRequestFilter(&_clientSessionProvider);
//    addRequestFilter(&_defaultProxyProcessor);
//    addRequestFilter(&_defaultAuthenticationProcessor);
//    addRequestFilter(&_defaultRedirectProcessor);
//    addRequestFilter(&_defaultClientHeaders);
//
//    // response processors
//    addResponseFilter(&_defaultProxyProcessor);
//    addResponseFilter(&_defaultAuthenticationProcessor);
//    addResponseFilter(&_defaultRedirectProcessor);
//
//    addResponseStreamFilter(&_encodingResponseStreamFilter);
//}
//
//
//HTTPClient::~HTTPClient()
//{
//}
//
////
////std::unique_ptr<BufferedResponse<GetRequest>> HTTPClient::get(const std::string& uri)
////{
////    return executeBuffered<GetRequest, BufferedResponse<GetRequest>>(std::make_unique<GetRequest>(uri));
////}
////
////
////std::unique_ptr<BufferedResponse<JSONRequest>> HTTPClient::post(const std::string& uri,
////                                                                const ofJson& json)
////{
////    return executeBuffered<JSONRequest, BufferedResponse<JSONRequest>>(std::make_unique<JSONRequest>(uri, json));
////}
////
////
////std::unique_ptr<BufferedResponse<PostRequest>> HTTPClient::form(const std::string& uri,
////                                                                const std::multimap<std::string, std::string> formFields,
////                                                                const std::vector<FormPart>& formParts)
////{
////    auto request = std::make_unique<PostRequest>(uri);
////    request->addFormFields(formFields);
////    request->addFormParts(formParts);
////    return executeBuffered<PostRequest, BufferedResponse<PostRequest>>(std::move(request));
////}
//
//
//} } // namespace ofx::HTTP
