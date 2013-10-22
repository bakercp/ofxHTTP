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


#include <istream>
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
#include "ofLog.h"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/Server/FileUploadRouteSettings.h"
#include "ofx/HTTP/Server/FileUploadRouteEvents.h"
#include "ofx/HTTP/Server/FileUploadRouteInterface.h"


namespace ofx {
namespace HTTP {


class FileUploadRouteHandler:
    public BaseRouteHandler,
    public Poco::Net::PartHandler
{
public:
    typedef FileUploadRouteSettings Settings;

    FileUploadRouteHandler(FileUploadRouteInterface& parent);
    virtual ~FileUploadRouteHandler();
    
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

    bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                          bool isSecurePort) const;

    void handlePart(const Poco::Net::MessageHeader& header,
                    std::istream& stream);

    virtual bool isContentTypeValid(const std::string& contentType) const;
    
private:
    FileUploadRouteInterface& _parent;

    std::size_t contentLength;
};

    
} } // namespace ofx::HTTP
