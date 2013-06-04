/*==============================================================================

 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 =============================================================================*/


#include "ResponseStream.h"


namespace ofx {
namespace HTTP {
        

//------------------------------------------------------------------------------
ResponseStream::ResponseStream(Poco::Net::HTTPResponse& _pResponse,
                               Poco::Net::HTTPResponseStream* _responseStream,
                               Poco::Exception* _exception)
: httpResponseStream(_responseStream)
, contentLength(_pResponse.getContentLength())
, version(_pResponse.getVersion())
, status(_pResponse.getStatus())
, reason(_pResponse.getReason())
, bIsKeepAlive(_pResponse.getKeepAlive())
, contentType(_pResponse.getContentType())
, transferEncoding(_pResponse.getTransferEncoding())
, bChunkedTransferEncoding(_pResponse.getChunkedTransferEncoding())
, headers(_pResponse)
, fieldLimit(_pResponse.getFieldLimit())
, exception(_exception)
{
    _pResponse.getCookies(cookies);
    
    // date(_pResponse.getDate()),

    
}

//------------------------------------------------------------------------------
ResponseStream::~ResponseStream() {
    // deleting a null pointer is a noop
    delete httpResponseStream; // cleans up the stream and the backing session
    delete exception;
}

//------------------------------------------------------------------------------
Poco::Net::HTTPResponse::HTTPStatus ResponseStream::getStatus() const {
    return status;
}

//------------------------------------------------------------------------------
string ResponseStream::getReasonForStatus() const {
    return reason;
}

//------------------------------------------------------------------------------
vector<Poco::Net::HTTPCookie> ResponseStream::getCookies() const {
    return cookies;
}

//------------------------------------------------------------------------------
string ResponseStream::getVersion() const {
    return version;
}

//------------------------------------------------------------------------------
bool ResponseStream::getKeepAlive() const {
    return bIsKeepAlive;
}

//------------------------------------------------------------------------------
string ResponseStream::getContentType() const {
    return contentType;
}
//------------------------------------------------------------------------------
string ResponseStream::getTransferEncoding() const {
    return transferEncoding;
}

//------------------------------------------------------------------------------
bool ResponseStream::getChunkedTransferEncoding() const {
    return bChunkedTransferEncoding;
}

//------------------------------------------------------------------------------
bool ResponseStream::hasResponseStream() const {
    return httpResponseStream != NULL;
}

//------------------------------------------------------------------------------
bool ResponseStream::hasException() const {
    return exception != NULL;
}

//------------------------------------------------------------------------------
Poco::Exception* ResponseStream::getException() const {
    return exception;
}

//------------------------------------------------------------------------------
istream* ResponseStream::getResponseStream() const {
    return httpResponseStream;
}

//------------------------------------------------------------------------------
Poco::Net::NameValueCollection ResponseStream::getHeaders() const {
    return headers;
}
    
//------------------------------------------------------------------------------
int ResponseStream::getFieldLimit() const {
    return fieldLimit;
}

    
} }
