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
 
 ==============================================================================*/

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

#include "ofxHTTPBaseRequest.h"

using std::streamsize;
using std::vector;

using Poco::AutoPtr;
using Poco::Exception;
using Poco::RefCountedObject;
using Poco::NullOutputStream;
using Poco::StreamCopier;
using Poco::URI;
using Poco::Timestamp;
using Poco::Net::HTTPCookie;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPResponseStream;

class ofxHTTPResponseStream : public RefCountedObject {
public:
        
    typedef AutoPtr<ofxHTTPResponseStream> Ptr;
    
    ofxHTTPResponseStream(HTTPResponse& _pResponse, HTTPResponseStream* _responseStream, Exception* _exception = NULL);
    virtual ~ofxHTTPResponseStream();

    HTTPResponse::HTTPStatus getStatus() const;
    string getReasonForStatus() const;
    
    vector<HTTPCookie> getCookies() const;
    
    string getVersion() const;
    
    bool getKeepAlive() const;
    
    string getContentType() const;
    string getTransferEncoding() const;
    bool   getChunkedTransferEncoding() const;
    
    bool hasResponseStream() const;
    istream* getResponseStream() const;
    
    bool hasException() const;
    Exception* getException() const;
    
    NameValueCollection getHeaders() const;
    int getFieldLimit() const;
    
protected:
    
    Exception* exception;
    
    HTTPResponseStream* httpResponseStream;
    streamsize contentLength;
    
    string version;
    HTTPResponse::HTTPStatus status;
	string reason;
    
    vector<HTTPCookie> cookies;
    
    Timestamp date;
    
    bool bIsKeepAlive;
    
    string contentType;
    string transferEncoding;
    bool   bChunkedTransferEncoding;
    
    NameValueCollection headers;
    int fieldLimit;
    
};