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


#include "Poco/Net/PartHandler.h"
#include "ofx/HTTP/Server/PostRouteEvents.h"
#include "ofx/HTTP/Server/PostRouteSettings.h"


namespace ofx {
namespace HTTP {


class PostRoute;


class PostRouteFileHandler: public Poco::Net::PartHandler
{
public:
    PostRouteFileHandler(const Poco::Net::HTTPServerRequest& request,
                         PostRoute& parent,
                         const std::string& formUUID);

    virtual ~PostRouteFileHandler();

    void handlePart(const Poco::Net::MessageHeader& header,
                    std::istream& stream);

    bool isContentTypeValid(const std::string& contentType) const;

private:
    const Poco::Net::HTTPServerRequest& _request;
    PostRoute& _parent;
    const std::string& _formUUID;

};


} } // namespace ofx::HTTP
