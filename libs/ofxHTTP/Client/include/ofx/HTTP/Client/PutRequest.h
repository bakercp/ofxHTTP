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


#include "BaseRequest.h"


namespace ofx {
namespace HTTP {
namespace Client {


class PutRequest: public BaseRequest
{
public:
    PutRequest(const Poco::URI& uri);
    PutRequest(const Poco::URI& uri, const std::string& httpVersion);
    
    virtual ~PutRequest();
    
    void addFile(const std::string& filename);
    
    void setContentRange(std::size_t startByte);

    void setContentRange(std::size_t startByte,
                         std::size_t endByte);

    void setContentType(const std::string& contentType);
    
protected:
    virtual void prepareRequest(Poco::Net::HTTPRequest& request) const;

    std::size_t _startByte;
    std::size_t _endByte;

    std::string _contentType;
    
    ofFile _file;

    friend class BaseClient;

};

    
} } } // ofx::HTTP::Client
