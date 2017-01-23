//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SimpleFileServer.h"


namespace ofx {
namespace HTTP {


SimpleFileServer::SimpleFileServer(const Settings& settings):
    BaseServer_<SimpleFileServerSettings, SimpleSessionStore>(settings),
    _fileSystemRoute(settings.fileSystemRouteSettings)
{
    addRoute(&_fileSystemRoute);
}


SimpleFileServer::~SimpleFileServer()
{
    removeRoute(&_fileSystemRoute);
}


void SimpleFileServer::setup(const Settings& settings)
{
    BaseServer_<SimpleFileServerSettings, SimpleSessionStore>::setup(settings);
    _fileSystemRoute.setup(settings.fileSystemRouteSettings);
}


FileSystemRoute& SimpleFileServer::fileSystemRoute()
{
    return _fileSystemRoute;
}


} } // namespace ofx::HTTP
