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


#include <map>
#include <string>
#include "Poco/URI.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofx/HTTP/Client/BaseRequest.h"


namespace ofx {
namespace HTTP {
namespace Client {


class PostRequest: public BaseRequest
{
public:
    PostRequest(const Poco::URI& uri);
    PostRequest(const Poco::URI& uri, const std::string& httpVersion);
    
    virtual ~PostRequest();
    
    void addFormFile(const std::string& fieldName, const std::string& filePath);
    void addFormFiles(const Poco::Net::NameValueCollection& nameValueMap);
    void addFormFiles(const std::map<std::string,std::string>&  nameValueMap);
    void addFormFiles(const std::multimap<std::string,std::string>&  nameValueMap);

    bool hasFormFiles() const;

    void clearFormFiles();
    
    const Poco::Net::NameValueCollection& getFormFiles() const;

protected:
    virtual void prepareRequest(Poco::Net::HTTPRequest& request) const;

    Poco::Net::NameValueCollection _formFiles;

    friend class BaseClient;

};


} } } // ofx::HTTP::Request
