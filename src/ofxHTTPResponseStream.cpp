#include "ofxHTTPResponseStream.h"


//------------------------------------------------------------------------------
ofxHTTPResponseStream::ofxHTTPResponseStream(HTTPResponse& _pResponse, HTTPResponseStream* _responseStream, Exception* _exception) :
httpResponseStream(_responseStream),
contentLength(_pResponse.getContentLength()),
version(_pResponse.getVersion()),
status(_pResponse.getStatus()),
reason(_pResponse.getReason()),
bIsKeepAlive(_pResponse.getKeepAlive()),
contentType(_pResponse.getContentType()),
transferEncoding(_pResponse.getTransferEncoding()),
bChunkedTransferEncoding(_pResponse.getChunkedTransferEncoding()),
headers(_pResponse),
fieldLimit(_pResponse.getFieldLimit()),
exception(_exception)
{
    _pResponse.getCookies(cookies);
    
    // date(_pResponse.getDate()),

    
}

//------------------------------------------------------------------------------
ofxHTTPResponseStream::~ofxHTTPResponseStream() {
    // deleting a null pointer is a noop
    delete httpResponseStream; // cleans up the stream and the backing session
    delete exception;
}

//------------------------------------------------------------------------------
HTTPResponse::HTTPStatus ofxHTTPResponseStream::getStatus() const {
    return status;
}

//------------------------------------------------------------------------------
string ofxHTTPResponseStream::getReasonForStatus() const {
    return reason;
}

//------------------------------------------------------------------------------
vector<HTTPCookie> ofxHTTPResponseStream::getCookies() const {
    return cookies;
}

//------------------------------------------------------------------------------
string ofxHTTPResponseStream::getVersion() const {
    return version;
}

//------------------------------------------------------------------------------
bool ofxHTTPResponseStream::getKeepAlive() const {
    return bIsKeepAlive;
}

//------------------------------------------------------------------------------
string ofxHTTPResponseStream::getContentType() const {
    return contentType;
}
//------------------------------------------------------------------------------
string ofxHTTPResponseStream::getTransferEncoding() const {
    return transferEncoding;
}

//------------------------------------------------------------------------------
bool ofxHTTPResponseStream::getChunkedTransferEncoding() const {
    return bChunkedTransferEncoding;
}

//------------------------------------------------------------------------------
bool ofxHTTPResponseStream::hasResponseStream() const {
    return httpResponseStream != NULL;
}

//------------------------------------------------------------------------------
bool ofxHTTPResponseStream::hasException() const {
    return exception != NULL;
}

//------------------------------------------------------------------------------
Exception* ofxHTTPResponseStream::getException() const {
    return exception;
}

//------------------------------------------------------------------------------
istream* ofxHTTPResponseStream::getResponseStream() const {
    return httpResponseStream;
}

//------------------------------------------------------------------------------
NameValueCollection ofxHTTPResponseStream::getHeaders() const {
    return headers;
}
    
//------------------------------------------------------------------------------
int ofxHTTPResponseStream::getFieldLimit() const {
    return fieldLimit;
}
