//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/BaseServer.h"
#include "ofx/HTTP/FileSystemRoute.h"
#include "ofx/HTTP/SSERoute.h"


namespace ofx {
namespace HTTP {


/// \brief Aggregate settings for a SimpleSSEServer.
class SimpleSSEServerSettings: public BaseServerSettings
{
public:
    /// \brief File system route settings.
    FileSystemRouteSettings fileSystemRouteSettings;

    /// \brief Server sent event route settings.
    SSERouteSettings sseRouteSettings;
    
};


/// \brief A basic implementation of an HTTP Server supporting server sent events.
class SimpleSSEServer: public BaseServer_<SimpleSSEServerSettings, SimpleSessionStore>
{
public:
    /// \brief A typedef for the SimpleSSEServerSettings.
    typedef SimpleSSEServerSettings Settings;

    /// \brief Create a SimpleSSEServer with the provided Settings.
    /// \param settings The Settings used to configure the server.
    SimpleSSEServer(const Settings& settings = Settings());

    /// \brief Destroy the SimpleSSEServer.
    virtual ~SimpleSSEServer();

    virtual void setup(const Settings& settings) override;

    /// \returns the file system route.
    FileSystemRoute& fileSystemRoute();

    /// \returns the SSERoute attached to this server.
    SSERoute& sseRoute();

private:
    /// \brief The FileSystemRoute attached to this server.
    FileSystemRoute _fileSystemRoute;

    /// \brief The SSERoute attached to this server.
    SSERoute _sseRoute;

};

    
} } // namespace ofx::HTTP
