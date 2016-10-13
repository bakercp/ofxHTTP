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


#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/OAuth10Credentials.h"


namespace ofx {
namespace HTTP {


/// \brief An OAuth 1.0 request filter.
class OAuth10RequestFilter: public AbstractRequestFilter
{
public:
    /// \brief Create a default OAuth 1.0 request filter.
    OAuth10RequestFilter();

    /// \brief Create a default OAuth 1.0 request filter.
    /// \param credentials The OAuth 1.0 credentials.
    OAuth10RequestFilter(const OAuth10Credentials& credentials);

    /// \brief Destroy the request filter.
    virtual ~OAuth10RequestFilter();

    /// \brief Set the 0Auth 1.0 credentials.
    /// \param credentials The OAuth 1.0 credentials.
    void setCredentials(const OAuth10Credentials& credentials);

    /// \returns The OAuth 1.0 credentials.
    OAuth10Credentials getCredentials() const;
    
    virtual void requestFilter(Context& context,
                               BaseRequest& request) const override;

private:
    /// \brief The OAuth 1.0 credentials used for authentication.
    OAuth10Credentials _credentials;

};


} } // namespace ofx::HTTP
