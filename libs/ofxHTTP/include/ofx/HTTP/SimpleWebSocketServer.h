//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/BaseServer.h"
#include "ofx/HTTP/FileSystemRoute.h"
#include "ofx/HTTP/WebSocketRoute.h"


namespace ofx {
namespace HTTP {


/// \brief Aggregate settings for a SimpleWebSocketServer.
class SimpleWebSocketServerSettings: public BaseServerSettings
{
public:
    /// \brief File system route settings.
    FileSystemRouteSettings fileSystemRouteSettings;

    /// \brief Web Socket route settings.
    WebSocketRouteSettings webSocketRouteSettings;

};


/// \brief A basic implementation of an HTTP Server supporting WebSockets.
class SimpleWebSocketServer: public BaseServer_<SimpleWebSocketServerSettings, SimpleSessionStore>
{
public:
    /// \brief A typedef for the SimpleWebSocketServerSettings.
    typedef SimpleWebSocketServerSettings Settings;

    /// \brief Create a SimpleWebSocketServer with the provided Settings.
    /// \param settings The Settings used to configure the server.
    SimpleWebSocketServer(const Settings& settings = Settings());

    /// \brief Destroy the SimpleWebSocketServer.
    virtual ~SimpleWebSocketServer();

    virtual void setup(const Settings& settings) override;

    /// \return A reference to the file system route.
    FileSystemRoute& fileSystemRoute();

    /// \returns the WebSocketRoute attached to this server.
    WebSocketRoute& webSocketRoute();

private:
    /// \brief The FileSystemRoute attached to this server.
    FileSystemRoute _fileSystemRoute;

    /// \brief The WebSocketRoute attached to this server.
    WebSocketRoute _webSocketRoute;

};

    
} } // namespace ofx::HTTP
