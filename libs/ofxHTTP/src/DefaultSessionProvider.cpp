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


#include "ofx/HTTP/DefaultSessionProvider.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


DefaultSessionProvider::DefaultSessionProvider()
{
}


DefaultSessionProvider::~DefaultSessionProvider()
{
}


void DefaultSessionProvider::filter(BaseRequest& request, Context& context)
{
    Context::Session session = context.getSession();

    if (!session)
    {
        Poco::URI uri(request.getURI());

        if(0 == uri.getScheme().compare("https"))
        {
            ofSSLManager::initializeClient(); // Initialize SSL context if needed.

            session = Context::Session(new Poco::Net::HTTPSClientSession(uri.getHost(),
                                                                         uri.getPort()));
        }
        else if (0 == uri.getScheme().compare("http"))
        {
            session = Context::Session(new Poco::Net::HTTPClientSession(uri.getHost(),
                                                                        uri.getPort()));
        }
        else
        {
            throw Poco::Exception("Unable to create session for the scheme: " + uri.getScheme());
        }

        session->setKeepAlive(context.getSessionSettings().getKeepAlive());
        session->setKeepAliveTimeout(context.getSessionSettings().getKeepAliveTimeout());

        context.setSession(session);
    }
}


} } // namespace ofx::HTTP
