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


#include <istream>
#include <vector>
#include "Poco/AutoPtr.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/NullStream.h"
#include "Poco/RefCountedObject.h"
#include "Poco/StreamCopier.h"
#include "Poco/Timestamp.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPIOStream.h"
#include "BaseRequest.h"


namespace ofx {
namespace HTTP {


class ResponseStream : public Poco::RefCountedObject {
public:
    typedef Poco::AutoPtr<ResponseStream> Ptr;
    
    ResponseStream(Poco::Net::HTTPResponse& pResponse,
                   Poco::Net::HTTPResponseStream* responseStream,
                   Poco::Exception* exception = NULL);

    virtual ~ResponseStream();

    Poco::Net::HTTPResponse::HTTPStatus getStatus() const;
    string getReasonForStatus() const;
    
    std::vector<Poco::Net::HTTPCookie> getCookies() const;
    
    std::string getVersion() const;
    
    bool getKeepAlive() const;
    
    std::string getContentType() const;
    std::string getTransferEncoding() const;
    bool   getChunkedTransferEncoding() const;
    
    bool hasResponseStream() const;
    std::istream* getResponseStream() const;
    
    bool hasException() const;
    Poco::Exception* getException() const;
    
    Poco::Net::NameValueCollection getHeaders() const;
    int getFieldLimit() const;
    
protected:
    Poco::Exception* _exception;
    
    Poco::Net::HTTPResponseStream* _httpResponseStream;
    std::streamsize _contentLength;
    
    std::string _version;
    Poco::Net::HTTPResponse::HTTPStatus _status;
    std::string _reason;
    
    std::vector<Poco::Net::HTTPCookie> _cookies;
    
    Poco::Timestamp _date;
    
    bool _bIsKeepAlive;
    
    std::string _contentType;
    std::string _transferEncoding;
    bool        _bChunkedTransferEncoding;
    
    Poco::Net::NameValueCollection _headers;
    int _fieldLimit;

};


} }
