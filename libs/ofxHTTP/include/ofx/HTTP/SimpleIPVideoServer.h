//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/BaseServer.h"
#include "ofx/HTTP/FileSystemRoute.h"
#include "ofx/HTTP/IPVideoRoute.h"
#include "ofx/HTTP/PostRoute.h"


namespace ofx {
namespace HTTP {


/// \brief Settings for a SimpleIPVideoServer.
class SimpleIPVideoServerSettings: public BaseServerSettings
{
public:
    /// \brief File system route settings.
    FileSystemRouteSettings fileSystemRouteSettings;

    /// \brief Post route settings.
    PostRouteSettings postRouteSettings;

    /// \brief IPVideo route settings.
    IPVideoRouteSettings ipVideoRouteSettings;
};


/// \brief A simple MJPEG-based IPVideo server implementation.
class SimpleIPVideoServer: public BaseServer_<SimpleIPVideoServerSettings, SimpleSessionStore>
{
public:
    /// \brief A typedef for the SimpleIPVideoServerSettings.
    typedef SimpleIPVideoServerSettings Settings;

    /// \brief Create a SimpleIPVideoServer with the given settings.
    /// \param settings the server settings.
    SimpleIPVideoServer(const Settings& settings = Settings());

    /// \brief Destroy the simeple IP Video Server.
    virtual ~SimpleIPVideoServer();

    virtual void setup(const Settings& settings) override;

    /// \brief Submit the pixels to send.
    /// \param pixels The pixels to send.
    void send(const ofPixels& pixels);

    /// \returns the number of clicents that are currently connected.
    std::size_t numConnections() const;

    /// \returns A reference to the file system route.
    FileSystemRoute& fileSystemRoute();

    /// \returns A reference to the post route.
    PostRoute& postRoute();

    /// \returns A reference to the IPVideo route.
    IPVideoRoute& ipVideoRoute();

protected:
    /// \brief The FileSystemRoute attached to this server.
    FileSystemRoute _fileSystemRoute;

    /// \brief The PostRoute attached to this server.
    PostRoute _postRoute;

    /// \brief THe IPVideo route attached to this server.
    IPVideoRoute _ipVideoRoute;

};


} } // namespace ofx::HTTP
