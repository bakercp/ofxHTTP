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


#include "ofx/HTTP/DefaultClientHeaders.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


DefaultClientHeaders::DefaultClientHeaders()
{
}


DefaultClientHeaders::~DefaultClientHeaders()
{
}


void DefaultClientHeaders::requestFilter(BaseRequest& request,
                                         Context& context)
{
    const ClientSessionSettings& settings = context.getClientSessionSettings();

    if (!settings.getUserAgent().empty())
    {
        request.set("User-Agent", settings.getUserAgent());
    }

//    request.set("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
//    request.set("Accept-Language", "en-US,en;q=0.8");

    const Poco::Net::NameValueCollection& defaultHeaders = settings.getDefaultHeaders();

    Poco::Net::NameValueCollection::ConstIterator iter = defaultHeaders.begin();

    while (iter != defaultHeaders.end())
    {
        request.set((*iter).first, (*iter).second);
        ++iter;
    }
}



} } // namespace ofx::HTTP
