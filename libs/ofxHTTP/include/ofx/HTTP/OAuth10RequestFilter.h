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


#pragma once


#include "Poco/Net/HTMLForm.h"
#include "ofx/HTTP/OAuth10Credentials.h"
#include "ofx/HTTP/AbstractClientTypes.h"


namespace ofx {
namespace HTTP {


class OAuth10RequestFilter: public AbstractRequestFilter
{
public:
    OAuth10RequestFilter();

    OAuth10RequestFilter(const std::string& consumerKey,
                         const std::string& consumerSecret,
                         const std::string& token,
                         const std::string& tokenSecret);

	virtual ~OAuth10RequestFilter();

    void requestFilter(BaseRequest& request, Context& context) override;

    OAuth10Credentials& credentials();

    const OAuth10Credentials& credentials() const;

private:
    OAuth10Credentials _credentials;

};


} } // namespace ofx::HTTP
