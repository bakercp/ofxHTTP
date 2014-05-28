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
#include "ofLog.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/PostRouteEvents.h"
#include "ofx/HTTP/PostRouteFileHandler.h"
#include "ofx/HTTP/PostRouteSettings.h"


namespace ofx {
namespace HTTP {


class PostRoute;


/// \brief A flexible POST route handler.
///
/// Form data must be encoded with "multipart/form-data" or
/// "application/x-www-form-urlencoded".  "text/plain" form
/// encoding is supported, but not parsed.
class PostRouteHandler: public AbstractRouteHandler
{
public:
    /// \brief A typedef for PostRouteSettings
    typedef PostRouteSettings Settings;

    /// \brief Create a PostRouteHandler.
    /// \param parent The parent PostRoute.
    PostRouteHandler(PostRoute& parent);

    /// \brief Destroy the PostRouteHandler.
    virtual ~PostRouteHandler();

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

    /// \brief A constant defining "text/plain".
    static const Poco::Net::MediaType POST_CONTENT_TYPE_TEXT_PLAIN;

    /// \brief A constant defining "multipart/form-data".
    static const Poco::Net::MediaType POST_CONTENT_TYPE_MULTIPART;

    /// \brief A constant defining "application/x-www-form-urlencoded".
    static const Poco::Net::MediaType POST_CONTENT_TYPE_URLENCODED;

    /// \brief A constant defining "application/json".
    static const Poco::Net::MediaType POST_CONTENT_TYPE_JSON;


private:
    /// \brief The parent PostRoute reference.
    PostRoute& _parent;

};

    
} } // namespace ofx::HTTP
