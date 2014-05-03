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


#include "ofx/HTTP/Client/DefaultAuthenticationProcessor.h"
#include "ofx/HTTP/Client/BaseRequest.h"
#include "ofx/HTTP/Client/BaseResponse.h"
#include "ofx/HTTP/Client/Context.h"


namespace ofx {
namespace HTTP {
namespace Client {


DefaultAuthenticationProcessor::DefaultAuthenticationProcessor()
{
}


DefaultAuthenticationProcessor::~DefaultAuthenticationProcessor()
{
}


void DefaultAuthenticationProcessor::processRequest(BaseRequest& request,
                                                    Context& context)
{
    Poco::URI uri(request.getURI());

    if (!uri.getUserInfo())
    {
        std::string userInfo
    }

    CredentialStore::SharedPtr store(context.getCredentialStore().lock());

    if (store && store->updateAuthentication(*context.getSession(), request))
    {
    }

    

//    Poco::URI uri(request.getURI());
//
//    std::string userInfo = uri.getUserInfo();
//
//    if (!userInfo.empty())
//    {
//        request.setCredentials("Basic", userInfo);
//    }
}


bool DefaultAuthenticationProcessor::handleResponse(BaseRequest& request,
                                                    BaseResponse& response,
                                                    Context& context)
{
    if (canHandleResponse(request, response, context))
    {
        CredentialStore::SharedPtr store(context.getCredentialStore().lock());

        if (store && store->authenticate(*context.getSession(),
                                         request,
                                         response))
        {
            context.setResubmit(true);
        }
    }
}


bool DefaultAuthenticationProcessor::canHandleResponse(BaseRequest& request,
                                                       BaseResponse& response,
                                                       Context& context) const
{
    return Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED == response.getStatus() &&
            !request.has(Poco::Net::HTTPRequest::AUTHORIZATION);
}



} } } // namespace ofx::HTTP::Client
