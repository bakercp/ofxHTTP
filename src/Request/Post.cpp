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


#include "Post.h"


namespace ofx {
namespace HTTP {
namespace Request {


//------------------------------------------------------------------------------
Post::Post(const std::string& url, const std::string& httpVersion)
: BaseRequest(Poco::Net::HTTPRequest::HTTP_POST,url,httpVersion)
{

}

//------------------------------------------------------------------------------
Post::Post(const Poco::URI& uri, const std::string& httpVersion)
: BaseRequest(Poco::Net::HTTPRequest::HTTP_POST,uri,httpVersion)
{

}

//------------------------------------------------------------------------------
Post::~Post()
{

}

//------------------------------------------------------------------------------
void Post::addFormFile(const std::string& fieldName, const std::string& filePath)
{
    _formFiles.add(fieldName, ofToDataPath(filePath));
}

//------------------------------------------------------------------------------
void Post::addFormFiles(const Poco::Net::NameValueCollection& nameValueMap)
{
    Poco::Net::NameValueCollection::ConstIterator iter = nameValueMap.begin();
    while(iter != nameValueMap.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void Post::addFormFiles(const std::map<std::string,std::string>& nameValueMap)
{
    std::map<std::string,std::string>::const_iterator iter = nameValueMap.begin();

    while(iter != nameValueMap.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void Post::addFormFiles(const multimap<string,string>&  nameValueMap)
{
    std::multimap<std::string,std::string>::const_iterator iter = nameValueMap.begin();

    while(iter != nameValueMap.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool Post::hasFormFiles() const
{
    return !_formFiles.empty();
}

//------------------------------------------------------------------------------
void Post::clearFormFiles()
{
    _formFiles.clear();
}

//------------------------------------------------------------------------------
const Poco::Net::NameValueCollection& Post::getFormFiles() const
{
    return _formFiles;
}

//------------------------------------------------------------------------------
void Post::prepareRequest(Poco::Net::HTTPClientSession& session,
                          Poco::Net::HTTPRequest& request,
                          Poco::Net::HTTPResponse& response)
{
    
}


} } }
