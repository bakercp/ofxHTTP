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


#include "ServerUploadRoute.h"


namespace ofx {
namespace HTTP {

    
//------------------------------------------------------------------------------
ServerUploadRoute::ServerUploadRoute(const Settings& settings) :
    _settings(settings)
{
    ofDirectory uploadsDirectory(_settings.uploadFolder);
    if(!uploadsDirectory.exists()) {
        uploadsDirectory.create();
    }
}

//------------------------------------------------------------------------------
ServerUploadRoute::~ServerUploadRoute()
{
}

//------------------------------------------------------------------------------
bool ServerUploadRoute::canHandleRequest(const HTTPServerRequest& request,
                                         bool bIsSecurePort)
{
    // require HTTP_POST
    if(request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST) {
        return false;
    }

    // require a valid path
    Poco::URI uri;
    try {
        uri = Poco::URI(request.getURI());
    } catch(const Poco::SyntaxException& exc) {
        ofLogError("ServerUploadRoute::canHandleRequest") << exc.what();
        return false;
    }
    
    // just get the path
    std::string path = uri.getPath();

    // make paths absolute
    if(path.empty()) {
        path = "/";
    }
    
    return Poco::RegularExpression(_settings.getRoute()).match(path);
}

HTTPRequestHandler* ServerUploadRoute::createRequestHandler(const HTTPServerRequest& request) {
    return new ServerUploadRouteHandler(_settings);
}


} }
