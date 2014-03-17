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


#pragma once


#include <string>
//#include "Poco/Exception.h"
//#include "Poco/URI.h"
//#include "Poco/Net/HTTPClientSession.h"
//#include "Poco/Net/HTTPCredentials.h"
//#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPRequest.h"
//#include "Poco/Net/HTTPResponse.h"
//#include "Poco/Net/NameValueCollection.h"
//#include "ofUtils.h"
//#include "ofx/HTTP/Client/Cookie.h"


namespace ofx {
namespace HTTP {
namespace Client {


class BaseRequest: public Poco::Net::HTTPRequest
{
public:
	BaseRequest(const std::string& method,
                const std::string& uri,
                const std::string& version);

    virtual ~BaseRequest();

protected:
    virtual void sendRequestBody(std::ostream& requestStream) const;

    friend class ResponseStream;
};


} } } // namespace ofx::HTTP::Client
