//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SimpleSSEServer.h"


namespace ofx {
namespace HTTP {


SimpleSSEServer::SimpleSSEServer(const Settings& settings):
    BaseServer_<SimpleSSEServerSettings, SimpleSessionStore>(settings),
    _fileSystemRoute(settings.fileSystemRouteSettings),
    _sseRoute(settings.sseRouteSettings)
{
    addRoute(&_fileSystemRoute);
    addRoute(&_sseRoute);
}


SimpleSSEServer::~SimpleSSEServer()
{
    removeRoute(&_sseRoute);
    removeRoute(&_fileSystemRoute);
}


void SimpleSSEServer::setup(const Settings& settings)
{
    BaseServer_<SimpleSSEServerSettings, SimpleSessionStore>::setup(settings);
    _fileSystemRoute.setup(settings.fileSystemRouteSettings);
    _sseRoute.setup(settings.sseRouteSettings);
}


FileSystemRoute& SimpleSSEServer::fileSystemRoute()
{
    return _fileSystemRoute;
}


SSERoute& SimpleSSEServer::sseRoute()
{
    return _sseRoute;
}


} } // namespace ofx::HTTP
