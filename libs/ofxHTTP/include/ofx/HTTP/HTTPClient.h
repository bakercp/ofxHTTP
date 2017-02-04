//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//
//
//
//#pragma once
//
//
//#include "ofConstants.h"
//#include "json.hpp"
//#include "ofx/HTTP/BaseClient.h"
//#include "ofx/HTTP/ClientSessionProvider.h"
//#include "ofx/HTTP/CredentialStore.h"
//#include "ofx/HTTP/DefaultClientHeaders.h"
//#include "ofx/HTTP/DefaultProxyProcessor.h"
//#include "ofx/HTTP/DefaultRedirectProcessor.h"
//#include "ofx/HTTP/DefaultEncodingResponseStreamFilter.h"
//#include "ofx/HTTP/Response.h"
//#include "ofx/HTTP/GetRequest.h"
//#include "ofx/HTTP/JSONRequest.h"
//#include "ofx/HTTP/PostRequest.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
//class HTTPClient: public BaseClient
//{
//public:
//    HTTPClient();
//    
//    virtual ~HTTPClient();
//
//    /// \brief GET data from the given URI.
//    /// \param uri The endpoint URI.
//    /// \returns a buffered response.
////    std::unique_ptr<BufferedResponse<GetRequest>> get(const std::string& uri);
////
////    /// \brief POST JSON to the given URI using application/json type.
////    /// \param uri The endpoint URI.
////    /// \param json The JSON to post.
////    /// \returns a buffered response.
////    std::unique_ptr<BufferedResponse<JSONRequest>> post(const std::string& uri,
////                                                        const ofJson& json);
////
////
////    std::unique_ptr<BufferedResponse<PostRequest>> form(const std::string& uri,
////                                                        const std::multimap<std::string, std::string> formFields = { },
////                                                        const std::vector<FormPart>& formParts = { });
//
//private:
//    ClientSessionProvider _clientSessionProvider;
//    DefaultClientHeaders _defaultClientHeaders;
//    DefaultProxyProcessor _defaultProxyProcessor;
//    DefaultCredentialStore _defaultAuthenticationProcessor;
//    DefaultRedirectProcessor _defaultRedirectProcessor;
//    DefaultEncodingResponseStreamFilter _encodingResponseStreamFilter;
//
//};
//
//
//} } // namespace ofx::HTTP
