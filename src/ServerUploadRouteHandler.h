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

#include <istream>

#include "ofLog.h"

#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/PartHandler.h"

#include "BaseTypes.h"
#include "ServerRouteHandler.h"

using Poco::Buffer;
using Poco::Exception;
using Poco::StreamCopier;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::MessageHeader;
using Poco::Net::NameValueCollection;
using Poco::Net::PartHandler;


namespace ofx {
namespace HTTP {


class ServerUploadRouteSettings : public BaseRouteSettings {
public:
    ServerUploadRouteSettings(const string& _route = "/upload") :
    BaseRouteSettings(_route),
    bRequireUploadFolderInDataFolder(true),
    uploadFolder("uploads"),
    bAutoCreateUploadFolder(false),
    uploadRedirect("uploaded.html"),
    writeBufferSize(8192)
    { }
    
    virtual ~ServerUploadRouteSettings() { }
    
    bool bRequireUploadFolderInDataFolder;
    string uploadFolder;
    bool bAutoCreateUploadFolder;
    string uploadRedirect;
    size_t writeBufferSize;
};

//------------------------------------------------------------------------------
class ServerUploadRouteHandler : public ServerRouteHandler, public PartHandler {
public:
    
    typedef ServerUploadRouteSettings Settings;
    
    ServerUploadRouteHandler(const Settings& _settings);
    virtual ~ServerUploadRouteHandler();
    
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
    void handlePart(const MessageHeader& header, istream& stream);

    virtual bool isContentTypeValid(const string& contentType) const;
    
protected:
    Settings settings;
    
};

    
} }

