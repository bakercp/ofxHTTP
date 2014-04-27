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


namespace ofx {
namespace HTTP {


DefaultAuthenticationProcessor::DefaultAuthenticationProcessor()
{
}


DefaultAuthenticationProcessor::~DefaultAuthenticationProcessor()
{
}


void DefaultAuthenticationProcessor::processRequest(Poco::Net::HTTPRequest& request,
                                                    Context& context)
{
    CredentialStore::SharedPtr store(context.getCredentialStore().lock());

    if(store && store->updateAuthentication(*context.getSession(), request))
    {
    }
}


void DefaultAuthenticationProcessor::processResponse(Poco::Net::HTTPRequest& request,
                                                     Poco::Net::HTTPResponse& response,
                                                     Context& context)
{
    CredentialStore::SharedPtr store(context.getCredentialStore().lock());

    Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();

    if (status == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
    {
        if (request.has(Poco::Net::HTTPRequest::AUTHORIZATION)
        || !store
        || !store->authenticate(*context.getSession(), request, response))
        {
            throw Poco::Net::NotAuthenticatedException(response.getReasonForStatus(status), status);
        }
    }
}


} } // namespace ofx::HTTP
