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


#include <istream>
#include "Poco/Exception.h"
#include "Poco/NullStream.h"
#include "Poco/Runnable.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPIOStream.h"
#include "ofSSLManager.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/Context.h"
#include "ofTypes.h"


namespace ofx {
namespace HTTP {


class BaseResponse: public Poco::Net::HTTPResponse
{
public:
//    typedef std::shared_ptr<BaseResponse> SharedPtr;

    BaseResponse();
    virtual ~BaseResponse();

//    bool hasResponseStream() const;
//    std::istream& getResponseStream();

//    bool hasException() const;
//    const Poco::Exception* getException() const;

private:
//    void setResponseStream(std::istream* pResponseStream);
//    void setException(Poco::Exception* pException);
//
//    std::istream* _pResponseStream;
//    Poco::Exception* _pException;

    friend class BaseClient;

};


} } // namespace ofx::HTTP
