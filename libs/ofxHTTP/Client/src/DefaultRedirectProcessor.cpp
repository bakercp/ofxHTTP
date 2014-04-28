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


#include "ofx/HTTP/Client/DefaultRedirectProcessor.h"


namespace ofx {
namespace HTTP {


DefaultRedirectProcessor::DefaultRedirectProcessor()
{
    _handledStatues.push_back(Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY);  // 301
    _handledStatues.push_back(Poco::Net::HTTPResponse::HTTP_FOUND);              // 302
    _handledStatues.push_back(Poco::Net::HTTPResponse::HTTP_SEE_OTHER);          // 303
    _handledStatues.push_back(Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT); // 307
}


DefaultRedirectProcessor::~DefaultRedirectProcessor()
{
}


void DefaultRedirectProcessor::processRequest(Client::BaseRequest& request,
                                              Context& context)
{
}

bool DefaultRedirectProcessor::handleResponse(Client::BaseRequest& request,
                                              Client::BaseResponse& response,
                                              Context& context)
{
    // TODO: handle HTMLForm correctly
    Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();

    if (isStatusHandled(status))
    {
        if (context.getRedirects().size() < context.getSessionSettings().getMaxRedirects())
        {
            Poco::URI resolvedURI = context.getResolvedURI();

            request.add("Referrer", resolvedURI.toString());
            request.setContentType("");

            // Get new location.
            resolvedURI.resolve(response.get("Location"));

            // Set it for the next go-around.
            request.setURI(resolvedURI.toString());

            const std::string& method = request.getMethod();

            if (method == Poco::Net::HTTPRequest::HTTP_POST
                ||  method == Poco::Net::HTTPRequest::HTTP_PUT)
            {
                request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
            }

            context.setResolvedURI(resolvedURI);
            
            context.addRedirect(resolvedURI);
            
            context.getSession().reset();

            cout << "Redirecting = " << resolvedURI.toString() << endl;
        }
        else
        {
            throw Poco::Net::HTTPException("Maximum redirects exceeded.");
        }
    }
}


} } // namespace ofx::HTTP
