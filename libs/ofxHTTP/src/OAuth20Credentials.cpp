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


#include "ofx/HTTP/OAuth20Credentials.h"


namespace ofx {
namespace HTTP {


const std::string OAuth20Credentials::SCHEME = "Bearer";


OAuth20Credentials::OAuth20Credentials():
	_scheme(SCHEME)
{
}


OAuth20Credentials::OAuth20Credentials(const std::string& bearerToken):
	_bearerToken(bearerToken),
	_scheme(SCHEME)
{
}


OAuth20Credentials::OAuth20Credentials(const std::string& bearerToken,
                                       const std::string& scheme):
	_bearerToken(bearerToken),
	_scheme(scheme)
{
}


OAuth20Credentials::~OAuth20Credentials()
{
}


void OAuth20Credentials::setBearerToken(const std::string& bearerToken)
{
	_bearerToken = bearerToken;
}


const std::string& OAuth20Credentials::getScheme() const
{
    return _scheme;
}


void OAuth20Credentials::setScheme(const std::string& scheme)
{
	_scheme = scheme;
}

	
const std::string& OAuth20Credentials::getBearerToken() const
{
    return _bearerToken;
}


} } // namespace Poco::Net
