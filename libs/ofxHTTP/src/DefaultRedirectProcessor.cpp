// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/DefaultRedirectProcessor.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


DefaultRedirectProcessor::DefaultRedirectProcessor()
{
}


DefaultRedirectProcessor::~DefaultRedirectProcessor()
{
}


void DefaultRedirectProcessor::requestFilter(BaseRequest&,
                                             Context&)
{
}

void DefaultRedirectProcessor::responseFilter(BaseRequest& request,
                                              BaseResponse& response,
                                              Context& context)
{
    if (canFilterResponse(request, response, context))
    {
        if (context.getRedirects().size() < context.getClientSessionSettings().getMaxRedirects())
        {
            Poco::URI currentURI(request.getURI());

            Poco::URI redirectedURI(currentURI);

            if (response.has("Location"))
            {
                redirectedURI.resolve(response.get("Location"));
            }
            else
            {
                throw Poco::Net::HTTPException("No Location Header Specified in Redirect.");
            }

            // Set referrer header.
            request.set("Referrer", currentURI.toString());

            // Save the information to the context.
            context.addRedirect(redirectedURI);

            // Reset the session if the scheme (e.g. http:// vs. https:// )
            // or authority (e.g. userInfo, host and port) differ.
            if (0 != currentURI.getScheme().compare(redirectedURI.getScheme()) ||
                0 != currentURI.getAuthority().compare(redirectedURI.getAuthority()))
            {
                // Delete the session since this is a
                // redirect to a different authority.
                // NOTE: ->reset() and .reset() are not
                // the same thing.
                context.getClientSession().reset();

                // Strip out any host header files that were set.
                request.erase(Poco::Net::HTTPRequest::HOST);
            }

            // Set the new URI according to the redirection.
            request.setURI(redirectedURI.toString());

            // Clear the form fields for all redirects. Query parameters will
            // be kept in the URI after a redirect and are no longer available
            // for manipulation during futher redirects.  Request signing
            // authorization standards such as OAuth do not allow redirects to
            // forward query parameters as the request would require a new
            // signature to be computed for the redirected domain / path.
            // This is to avoid security holes.  For cusom redirect handling,
            // create a custom implementation of BaseRequestResponseProcessor.
            //
            // Additionally, by clearing the form fields, we allows the client
            // to respect server query re-writes such as those re-written by
            // permalinks on blogs.
            // TODO: not sure about this ...
            // request.getFormRef().clear();

            // If the method is entity containing method (e.g. POST or PUT), we
            // will not redirect with the entity or parameters (which might
            // contain sensitive information), but rather convert it to a GET
            // request.  Some browsers redirect POST / PUT, while others do not.
            // Users desiring special behaviour should create a custom
            // implementation of BaseRequestResponseProcessor.
            const std::string& method = request.getMethod();

            if (method == Poco::Net::HTTPRequest::HTTP_POST ||
                method == Poco::Net::HTTPRequest::HTTP_PUT)
            {
                request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
            }

            // Set the context to resubmit.
            context.setResubmit(true);

            return;
        }
        else
        {
            throw Poco::Net::HTTPException("Maximum redirects exceeded.");
        }
    }
    else
    {
        return;
    }
}


bool DefaultRedirectProcessor::canFilterResponse(BaseRequest&,
                                                 BaseResponse& response,
                                                 Context&) const
{
    Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();

    return Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY   == status || // 301
           Poco::Net::HTTPResponse::HTTP_FOUND               == status || // 302
           Poco::Net::HTTPResponse::HTTP_SEE_OTHER           == status || // 303
           Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT  == status;   // 307
}
    



} } // namespace ofx::HTTP
