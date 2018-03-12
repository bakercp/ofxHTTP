//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/DefaultRedirectProcessor.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Response.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


DefaultRedirectProcessor::DefaultRedirectProcessor()
{
}


DefaultRedirectProcessor::~DefaultRedirectProcessor()
{
}


void DefaultRedirectProcessor::requestFilter(Context& context, Request&) const
{
    // We set resubmit false because we are executing a new request.
    context.setResubmit(false);
}


void DefaultRedirectProcessor::responseFilter(Context& context,
                                              Request& request,
                                              Response& response) const
{
    if (Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY   == response.getStatus() || // 301
        Poco::Net::HTTPResponse::HTTP_FOUND               == response.getStatus() || // 302
        Poco::Net::HTTPResponse::HTTP_SEE_OTHER           == response.getStatus() || // 303
        Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT  == response.getStatus())   // 307
    {
        if (context.getRedirects().size() < context.getClientSessionSettings().getMaxRedirects())
        {
            Poco::URI lastURI(request.getURI());

            Poco::URI targetURI(lastURI);

            if (response.has("Location"))
            {
                targetURI.resolve(response.get("Location"));
            }
            else
            {
                throw Poco::Net::HTTPException("No location header specified in redirect.");
            }

            ofLogVerbose("DefaultRedirectProcessor::requestFilter") << "Processing " << response.getStatus() << " to " << response.get("Location");
            ofLogVerbose("DefaultRedirectProcessor::requestFilter") << "Last URI: " << lastURI.toString();
            ofLogVerbose("DefaultRedirectProcessor::requestFilter") << "Target URI: " << targetURI.toString();

            // Set referrer header.
            request.set("Referrer", lastURI.toString());

            // Save the information to the context.
            context.addRedirect(targetURI);

            // Erase the request host that may have been set by the client session.
            request.erase(Poco::Net::HTTPRequest::HOST);

            // Set the new URI according to the redirection.
            request.setURI(targetURI.toString());

            // Clear the form fields for all redirects. Query parameters will
            // be kept in the URI after a redirect and are no longer available
            // for manipulation during futher redirects.  Request signing
            // authorization standards such as OAuth do not allow redirects to
            // forward query parameters as the request would require a new
            // signature to be computed for the redirected domain / path.
            // This is to avoid security holes.  For cusom redirect handling,
            // create a custom implementation of RequestResponseProcessor.
            //
            // Additionally, by clearing the form fields, we allows the client
            // to respect server query re-writes such as those re-written by
            // permalinks on blogs.
            ///
            // TODO: not sure about this ...
            // request.getForm().clear();

            // If the method is entity containing method (e.g. POST or PUT), we
            // will not redirect with the entity or parameters (which might
            // contain sensitive information), but rather convert it to a GET
            // request.  Some browsers redirect POST / PUT, while others do not.
            // Users desiring special behaviour should create a custom
            // implementation of RequestResponseProcessor.
            const std::string& method = request.getMethod();

            if (method == Poco::Net::HTTPRequest::HTTP_POST ||
                method == Poco::Net::HTTPRequest::HTTP_PUT)
            {
                ofLogVerbose("DefaultRedirectProcessor::requestFilter") << "Converting POST to GET during redirect.";
                request.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
            }

            // Set the context to resubmit.
            context.setResubmit(true);
        }
        else
        {
            throw Poco::Net::HTTPException("Maximum redirects exceeded.");
        }
    }
}


} } // namespace ofx::HTTP
