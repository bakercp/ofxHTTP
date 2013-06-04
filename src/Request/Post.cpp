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
Post::Post(const string& _url, const string& _httpVersion)
: BaseRequest(Poco::Net::HTTPRequest::HTTP_POST,_url,_httpVersion)
{

}

//------------------------------------------------------------------------------
Post::Post(const Poco::URI& _uri, const string& _httpVersion)
: BaseRequest(Poco::Net::HTTPRequest::HTTP_POST,_uri,_httpVersion)
{

}

//------------------------------------------------------------------------------
Post::~Post()
{

}

//------------------------------------------------------------------------------
void Post::addFormFile(const string& fieldName, const string& filePath)
{
    formFiles.add(fieldName, ofToDataPath(filePath));
}

//------------------------------------------------------------------------------
void Post::addFormFiles(const Poco::Net::NameValueCollection& nvc) {
    Poco::Net::NameValueCollection::ConstIterator iter = nvc.begin();
    while(iter != nvc.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void Post::addFormFiles(const map<string,string>& nvc) {
    map<string,string>::const_iterator iter = nvc.begin();
    while(iter != nvc.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void Post::addFormFiles(const multimap<string,string>&  nvc) {
    multimap<string,string>::const_iterator iter = nvc.begin();
    while(iter != nvc.end()) {
        addFormFile((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool Post::hasFormFiles() const {
    return !formFiles.empty();
}

//------------------------------------------------------------------------------
void Post::clearFormFiles() {
    formFiles.clear();
}

//------------------------------------------------------------------------------
const Poco::Net::NameValueCollection& Post::getFormFiles() const {
    return formFiles;
}

//------------------------------------------------------------------------------
void Post::prepareRequest(Poco::Net::HTTPClientSession& session,
                                 Poco::Net::HTTPRequest& request,
                                 Poco::Net::HTTPResponse& response) {
    
}


} } }
