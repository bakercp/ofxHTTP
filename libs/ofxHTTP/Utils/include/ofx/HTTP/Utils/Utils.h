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


#include <string>
#include <vector>
#include "Poco/URI.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "ofLog.h"
#include "ofUtils.h"


namespace ofx {
namespace HTTP {


class Utils
    /// \brief A collection of HTTP utilities.
{
public:
    static Poco::Net::NameValueCollection getQueryMap(const Poco::URI& uri);
        ///< \brief Extracts a map of query parameter names and their values.
        ///< \param uri The URI from which to extract the parameter map.
        ///< \returns a map of query parameter names and their values.

    static void dumpHeaders(const Poco::Net::HTTPServerRequest& request,
                            const Poco::Net::HTTPServerResponse& response,
                            ofLogLevel logLevel = OF_LOG_VERBOSE);
        ///< \brief Dump headers from both server requests and responses.
        ///< \param request the HTTPServerRequest request.
        ///< \param response the HTTPServerResponse response.

    static void dumpHeaders(const Poco::Net::HTTPServerRequest& request,
                            ofLogLevel logLevel = OF_LOG_VERBOSE);
        ///< \brief Dump headers from a server request.
        ///< \param request the HTTPServerRequest request.

    static void dumpHeaders(const Poco::Net::HTTPServerResponse& response,
                            ofLogLevel logLevel = OF_LOG_VERBOSE);
        ///< \brief Dump headers from a server response.
        ///< \param response the HTTPServerResponse response.

};


} } // namespace ofx::HTTP
