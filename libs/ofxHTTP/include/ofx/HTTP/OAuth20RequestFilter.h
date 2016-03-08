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


#include "Poco/Net/HTMLForm.h"
#include "ofx/HTTP/OAuth20Credentials.h"
#include "ofx/HTTP/AbstractClientTypes.h"


namespace ofx {
namespace HTTP {


class OAuth20RequestFilter: public AbstractRequestFilter
{
public:
    OAuth20RequestFilter();

    OAuth20RequestFilter(const std::string& bearerToken,
                         const std::string& scheme = OAuth20Credentials::SCHEME);

	virtual ~OAuth20RequestFilter();

    void requestFilter(BaseRequest& request, Context& context) override;

    OAuth20Credentials& credentials();

    const OAuth20Credentials& credentials() const;

private:
    OAuth20Credentials _credentials;

};


} } // namespace ofx::HTTP
