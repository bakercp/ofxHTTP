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


#pragma once


#include "ofConstants.h"
#include "json.hpp"
#include "ofx/HTTP/BaseClient.h"
#include "ofx/HTTP/CredentialStore.h"
#include "ofx/HTTP/DefaultClientHeaders.h"
#include "ofx/HTTP/DefaultClientSessionProvider.h"
#include "ofx/HTTP/DefaultProxyProcessor.h"
#include "ofx/HTTP/DefaultRedirectProcessor.h"
#include "ofx/HTTP/DefaultResponseStreamFilter.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/GetRequest.h"
#include "ofx/HTTP/JSONRequest.h"
#include "ofx/HTTP/PostRequest.h"


namespace ofx {
namespace HTTP {


class HTTPClient: public BaseClient
{
public:
    HTTPClient();
    
    virtual ~HTTPClient();

    /// \brief GET data from the given URI.
    /// \param uri The endpoint URI.
    /// \returns a buffered response.
    std::unique_ptr<BufferedResponse<GetRequest>> get(const std::string& uri);

    /// \brief POST JSON to the given URI using application/json type.
    /// \param uri The endpoint URI.
    /// \param json The JSON to post.
    /// \returns a buffered response.
    std::unique_ptr<BufferedResponse<JSONRequest>> post(const std::string& uri,
                                                        const ofJson& json);


    std::unique_ptr<BufferedResponse<PostRequest>> form(const std::string& uri,
                                                        const std::multimap<std::string, std::string> formFields = { },
                                                        const std::vector<FormPart>& formParts = { });

private:
    DefaultClientSessionProvider _defaultClientSessionProvider;
    DefaultClientHeaders _defaultClientHeaders;
    DefaultProxyProcessor _defaultProxyProcessor;
    DefaultCredentialStore _defaultAuthenticationProcessor;
    DefaultRedirectProcessor _defaultRedirectProcessor;
    DefaultResponseStreamFilter _responseStreamFilter;

};


} } // namespace ofx::HTTP
