// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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
#include "ofx/HTTP/AbstractClientTypes.h"


namespace ofx {


class TwitterRequestFilter: public HTTP::AbstractRequestFilter
{
public:
	TwitterRequestFilter();
	~TwitterRequestFilter();

	void setup(const std::string& consumerKey,
               const std::string& consumerSecret,
               const std::string& token,
               const std::string& tokenSecret);

	static const std::string TWITTER_URI;

    void requestFilter(HTTP::BaseRequest& request,
                       HTTP::Context& context);

    /// Signs the given HTTP request according to OAuth 1.0a as used by the Twitter API.
    ///
    /// See <https://dev.twitter.com/docs/auth/authorizing-request> and
    /// <https://dev.twitter.com/docs/auth/creating-signature> for more information.
	void sign(Poco::Net::HTTPRequest& request,
              const Poco::Net::HTMLForm& params,
              const std::string& uri) const;

    /// Creates a nonce, which is basically a Base64-encoded 32 character random
    /// string, with non-alphanumeric characters removed.
	std::string createNonce() const;

    /// Creates a OAuth signature for the given request and its parameters, according
    /// to <https://dev.twitter.com/docs/auth/creating-signature>.
	std::string createSignature(Poco::Net::HTTPRequest& request,
                                const Poco::Net::HTMLForm& params,
                                const std::string& uri,
                                const std::string& nonce,
                                const std::string& timestamp) const;

    /// \brief Percent-encodes the given string according to Twitter API's rules,
    /// given in <https://dev.twitter.com/docs/auth/percent-encoding-parameters>.
	static std::string percentEncode(const std::string& str);

private:
	std::string _consumerKey;
	std::string _consumerSecret;
	std::string _token;
	std::string _tokenSecret;
};


} // namespace ofx
