//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
