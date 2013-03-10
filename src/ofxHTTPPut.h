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

#include "ofxHTTPBaseRequest.h"

using std::numeric_limits;

class ofxHTTPPut : public ofxHTTPBaseRequest {
public:
    
    ofxHTTPPut(const string& _url, const string& _httpVersion = HTTPRequest::HTTP_1_1);
    ofxHTTPPut(const URI& _uri,    const string& _httpVersion = HTTPRequest::HTTP_1_1);
    
    virtual ~ofxHTTPPut();
    
    void addFile(const string& filename);
    
    void setContentRange(size_t startByte, size_t endByte = numeric_limits<size_t>::max());
    void setContentType(const string& contentType);
    
protected:
    
    //------------------------------------------------------------------------------
    virtual void prepareRequest(HTTPRequest& request) const;

    size_t startByte;
    size_t endByte;
    
    string contentType;
    
    ofFile file;
    
};