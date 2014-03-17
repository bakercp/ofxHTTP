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


#include "ofx/HTTP/Client/PostRequest.h"


namespace ofx {
namespace HTTP {
namespace Client {

    

PostRequest::PostRequest(const Poco::URI& uri):
    BaseRequest(Poco::Net::HTTPRequest::HTTP_POST,uri)
{
}


PostRequest::PostRequest(const Poco::URI& uri, const std::string& httpVersion):
    BaseRequest(Poco::Net::HTTPRequest::HTTP_POST,uri,httpVersion)
{
}


PostRequest::~PostRequest()
{
}


void PostRequest::addFormFile(const std::string& fieldName, const std::string& filePath)
{
    _formFiles.add(fieldName, ofToDataPath(filePath));
}


void PostRequest::addFormFiles(const Poco::Net::NameValueCollection& nameValueMap)
{
    Poco::Net::NameValueCollection::ConstIterator iter = nameValueMap.begin();

    while(iter != nameValueMap.end())
    {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}


void PostRequest::addFormFiles(const std::map<std::string,std::string>& nameValueMap)
{
    std::map<std::string,std::string>::const_iterator iter = nameValueMap.begin();

    while(iter != nameValueMap.end())
    {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}


void PostRequest::addFormFiles(const multimap<string,string>&  nameValueMap)
{
    std::multimap<std::string,std::string>::const_iterator iter = nameValueMap.begin();

    while(iter != nameValueMap.end())
    {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}


bool PostRequest::hasFormFiles() const
{
    return !_formFiles.empty();
}


void PostRequest::clearFormFiles()
{
    _formFiles.clear();
}


const Poco::Net::NameValueCollection& PostRequest::getFormFiles() const
{
    return _formFiles;
}


void PostRequest::prepareRequest(Poco::Net::HTTPRequest& request) const
{
    // TODO: this is not the right place to do this.
    Poco::Net::NameValueCollection::ConstIterator iter = _headers.begin();

    while (iter != _headers.end())
    {
        request.set((*iter).first, (*iter).second);
        ++iter;
    }
}


} } } // ofx::HTTP::Request
