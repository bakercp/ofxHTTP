//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/SimpleFileServer.h"
#include "ofx/HTTP/PostRoute.h"
#include "ofx/HTTP/PostRouteEvents.h"


namespace ofx {
namespace HTTP {


/// \brief Settings for a SimplePostServer.
class SimplePostServerSettings: public BaseServerSettings
{
public:
    /// \brief File system route settings.
    FileSystemRouteSettings fileSystemRouteSettings;

    /// \brief Post route settings.
    PostRouteSettings postRouteSettings;

};


class SimplePostServer:
    public BaseServer_<SimplePostServerSettings, SimpleSessionStore>
{
public:
    typedef SimplePostServerSettings Settings;

    SimplePostServer(const Settings& settings = Settings());

    virtual ~SimplePostServer();

    virtual void setup(const Settings& settings) override;

    /// \return A reference to the file system route.
    FileSystemRoute& fileSystemRoute();

    PostRoute& postRoute();

protected:
    /// \brief The FileSystemRoute attached to this server.
    FileSystemRoute _fileSystemRoute;

    PostRoute _postRoute;
    
};


} } // namespace ofx::HTTP
