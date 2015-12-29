// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/BaseRoute.h"


namespace ofx {
namespace HTTP {


class FileSystemRouteSettings: public BaseRouteSettings
{
public:
    FileSystemRouteSettings(const std::string& routePathPattern = DEFAULT_ROUTE_PATH_PATTERN,
                            bool requireSecurePort = false,
                            bool requireAuthentication = false);

    virtual ~FileSystemRouteSettings();

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

    void setDefaultIndex(const std::string& defaultIndex);
    const std::string& getDefaultIndex() const;

    void setDocumentRoot(const std::string& documentRoot);
    const std::string& getDocumentRoot() const;

    void setAutoCreateDocumentRoot(bool autoCreateDocumentRoot);
    bool getAutoCreateDocumentRoot() const;

    void setRequireDocumentRootInDataFolder(bool requireDocumentRootInDataFolder);
    bool getRequireDocumentRootInDataFolder() const;

    static const std::string DEFAULT_DOCUMENT_ROOT;
    static const std::string DEFAULT_INDEX;

    /// \brief An unfortunate compromise until C++11.
    /// \note C++ is not able to initialize static collections until
    ///        after C++11.  This is a compromise until then.
    static const std::string DEFAULT_GET_HTTP_METHODS_ARRAY[];

    /// \brief The default HTTP methods for this route.
    static const HTTPMethodSet DEFAULT_GET_HTTP_METHODS;

private:
    std::string _defaultIndex;
    std::string _documentRoot;
    
    bool _autoCreateDocumentRoot;
    bool _requireDocumentRootInDataFolder;
    
};


/// \brief A route for serving files from the file system.
class FileSystemRoute: public BaseRoute_<FileSystemRouteSettings>
{
public:
    typedef FileSystemRouteSettings Settings;

    FileSystemRoute(const Settings& settings);

    virtual ~FileSystemRoute();

    virtual void handleRequest(ServerEventArgs& evt) override;

    virtual void handleErrorResponse(ServerEventArgs& evt);

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

};

    
} } // namespace ofx::HTTP
