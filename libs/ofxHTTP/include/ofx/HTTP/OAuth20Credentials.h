// =============================================================================
//
// Copyright (c) 2009-2016 Christopher Baker <http://christopherbaker.net>
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


#include <string>
#include "json.hpp"


namespace ofx {
namespace HTTP {


class OAuth20Credentials
{
public:
	OAuth20Credentials();

	OAuth20Credentials(const std::string& bearerToken);

	OAuth20Credentials(const std::string& bearerToken,
                       const std::string& scheme);

	virtual ~OAuth20Credentials();

	void setBearerToken(const std::string& bearerToken);

	const std::string& getBearerToken() const;

	void setScheme(const std::string& scheme);

	const std::string& getScheme() const;

	static const std::string SCHEME;

//    static OAuth20Credentials fromJSON(const ofJson& value);
//
//    static ofJson toJSON(const OAuth20Credentials& credentials);
//
//    static OAuth20Credentials fromFile(const std::string& credentialsFile);
//
//    static bool toFile(const OAuth20Credentials& credentials,
//                       const std::string& credentialsFile);


private:
    /// \brief The OAuth 2.0 bearer token.
	std::string _bearerToken;

    /// \brief The OAuth 1.0 scheme.
	std::string _scheme;
};


} } // namespace ofx::HTTP

