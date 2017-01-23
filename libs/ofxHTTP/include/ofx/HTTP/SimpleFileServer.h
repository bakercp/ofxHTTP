//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/BaseServer.h"
#include "ofx/HTTP/FileSystemRoute.h"


namespace ofx {
namespace HTTP {


/// \brief Settings for a SimpleFileServer.
class SimpleFileServerSettings: public BaseServerSettings
{
public:
    /// \brief File system route settings.
    FileSystemRouteSettings fileSystemRouteSettings;
};


/// \brief A simple file server implementation.
class SimpleFileServer: public BaseServer_<SimpleFileServerSettings, SimpleSessionStore>
{
public:
    /// \brief A typedef for the SimpleFileServerSettings.
    typedef SimpleFileServerSettings Settings;

    /// \brief Create a SimpleFileServer with the provided Settings.
    /// \param settings The Settings used to configure the server.
    SimpleFileServer(const Settings& settings = Settings());

    /// \brief Destroy the BasicServer.
    virtual ~SimpleFileServer();

    virtual void setup(const Settings& settings) override;

    /// \return A reference to the file system route.
    FileSystemRoute& fileSystemRoute();

protected:
    /// \brief The FileSystemRoute attached to this server.
    FileSystemRoute _fileSystemRoute;

};


} } // namespace ofx::HTTP
