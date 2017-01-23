//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SimpleWebSocketServer.h"


namespace ofx {
namespace HTTP {


SimpleWebSocketServer::SimpleWebSocketServer(const Settings& settings):
    BaseServer_<SimpleWebSocketServerSettings, SimpleSessionStore>(settings),
    _fileSystemRoute(settings.fileSystemRouteSettings),
    _webSocketRoute(settings.webSocketRouteSettings)
{
    addRoute(&_fileSystemRoute);
    addRoute(&_webSocketRoute);
}


SimpleWebSocketServer::~SimpleWebSocketServer()
{
    removeRoute(&_webSocketRoute);
    removeRoute(&_fileSystemRoute);
}


void SimpleWebSocketServer::setup(const Settings& settings)
{
    BaseServer_<SimpleWebSocketServerSettings, SimpleSessionStore>::setup(settings);
    _fileSystemRoute.setup(settings.fileSystemRouteSettings);
    _webSocketRoute.setup(settings.webSocketRouteSettings);
}


FileSystemRoute& SimpleWebSocketServer::fileSystemRoute()
{
    return _fileSystemRoute;
}


WebSocketRoute& SimpleWebSocketServer::webSocketRoute()
{
    return _webSocketRoute;
}


} } // namespace ofx::HTTP
