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


#include "ResponseStream.h"


namespace ofx {
namespace HTTP {
        

//------------------------------------------------------------------------------
ResponseStream::ResponseStream(Poco::Net::HTTPResponse&       pResponse,
                               Poco::Net::HTTPResponseStream* responseStream,
                               Poco::Exception*               exception) :
    _httpResponseStream(responseStream),
    _contentLength(pResponse.getContentLength()),
    _version(pResponse.getVersion()),
    _status(pResponse.getStatus()),
    _reason(pResponse.getReason()),
    _bIsKeepAlive(pResponse.getKeepAlive()),
    _contentType(pResponse.getContentType()),
    _transferEncoding(pResponse.getTransferEncoding()),
    _bChunkedTransferEncoding(pResponse.getChunkedTransferEncoding()),
    _headers(pResponse),
    _fieldLimit(pResponse.getFieldLimit()),
    _exception(exception)
{
    // fill cookies
    pResponse.getCookies(_cookies);
    // date(_pResponse.getDate()),
}

//------------------------------------------------------------------------------
ResponseStream::~ResponseStream()
{
    // deleting a null pointer is a noop
    delete _httpResponseStream; // cleans up the stream and the backing session
    delete _exception;
}

//------------------------------------------------------------------------------
Poco::Net::HTTPResponse::HTTPStatus ResponseStream::getStatus() const
{
    return _status;
}

//------------------------------------------------------------------------------
std::string ResponseStream::getReasonForStatus() const
{
    return _reason;
}

//------------------------------------------------------------------------------
vector<Poco::Net::HTTPCookie> ResponseStream::getCookies() const
{
    return _cookies;
}

//------------------------------------------------------------------------------
std::string ResponseStream::getVersion() const
{
    return _version;
}

//------------------------------------------------------------------------------
bool ResponseStream::getKeepAlive() const {
    return _bIsKeepAlive;
}

//------------------------------------------------------------------------------
std::string ResponseStream::getContentType() const
{
    return _contentType;
}
    
//------------------------------------------------------------------------------
std::string ResponseStream::getTransferEncoding() const
{
    return _transferEncoding;
}

//------------------------------------------------------------------------------
bool ResponseStream::getChunkedTransferEncoding() const
{
    return _bChunkedTransferEncoding;
}

//------------------------------------------------------------------------------
bool ResponseStream::hasResponseStream() const
{
    return _httpResponseStream != NULL;
}

//------------------------------------------------------------------------------
bool ResponseStream::hasException() const
{
    return _exception != NULL;
}

//------------------------------------------------------------------------------
Poco::Exception* ResponseStream::getException() const
{
    return _exception;
}

//------------------------------------------------------------------------------
std::istream* ResponseStream::getResponseStream() const
{
    return _httpResponseStream;
}

//------------------------------------------------------------------------------
Poco::Net::NameValueCollection ResponseStream::getHeaders() const
{
    return _headers;
}
    
//------------------------------------------------------------------------------
int ResponseStream::getFieldLimit() const
{
    return _fieldLimit;
}

    
} }
