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
#include "AbstractTypes.h"


namespace ofx {
namespace HTTP {


class BaseServerRouteSettings : public AbstractHasRoute {
public:
    BaseServerRouteSettings(const std::string& route = DEFAULT_ROUTE);

    virtual ~BaseServerRouteSettings();

    // TODO:
    // e.g. http://httpd.apache.org/docs/2.2/mod/mod_deflate.html
    // or the reverse? compression type as key?
    //    ofxHTTPCompressorEntry html(MediaType("text/html"));
    //    html.addCompressionType(GZIP);
    //    html.addCompressionType(DEFLATE);
    //
    //    ofxHTTPCompressorEntry plain(MediaType("text/plain"));
    //    html.addCompressionType(GZIP);
    //    html.addCompressionType(DEFLATE);

    virtual std::string getRoute() const;

    std::string defaultIndex;
    std::string documentRoot;

    bool bAutoCreateDocumentRoot;
    bool bRequireDocumentRootInDataFolder;

//    std::map<Poco::Net::MediaType,CompressorEntry> contentEncoding;

protected:
    static const std::string DEFAULT_ROUTE;
    static const std::string DEFAULT_DOCUMENT_ROOT;
    static const std::string DEFAULT_INDEX;

    std::string _route;

};


} } // namespace ofx::HTTP
