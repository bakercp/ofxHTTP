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

#pragma once

#include "ofxHTTPBaseRequest.h"

class ofxHTTPPost : public ofxHTTPBaseRequest {
public:
    
    ofxHTTPPost(const string& _url, const string& _httpVersion = HTTPRequest::HTTP_1_1);
    ofxHTTPPost(const URI& _uri,    const string& _httpVersion = HTTPRequest::HTTP_1_1);
    
    virtual ~ofxHTTPPost();
    
    void addFormFile(const string& fieldName, const string& filePath);
    void addFormFiles(const NameValueCollection& nvc);
    void addFormFiles(const map<string,string>&  nvc);
    void addFormFiles(const multimap<string,string>&  nvc);

    bool hasFormFiles() const;

    void clearFormFiles();
    
    const NameValueCollection& getFormFiles() const;

protected:
    
    //------------------------------------------------------------------------------
    virtual void prepareRequest(HTTPClientSession& session, HTTPRequest& request, HTTPResponse& response);

    NameValueCollection formFiles;

};