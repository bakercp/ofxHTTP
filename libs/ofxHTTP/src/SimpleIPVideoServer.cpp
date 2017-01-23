//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SimpleIPVideoServer.h"


namespace ofx {
namespace HTTP {


SimpleIPVideoServer::SimpleIPVideoServer(const Settings& settings):
    BaseServer_<SimpleIPVideoServerSettings, SimpleSessionStore>(settings),
    _fileSystemRoute(settings.fileSystemRouteSettings),
    _postRoute(settings.postRouteSettings),
    _ipVideoRoute(settings.ipVideoRouteSettings)
{
    addRoute(&_fileSystemRoute);
    addRoute(&_postRoute);
    addRoute(&_ipVideoRoute);
}


SimpleIPVideoServer::~SimpleIPVideoServer()
{
    removeRoute(&_ipVideoRoute);
    removeRoute(&_postRoute);
    removeRoute(&_fileSystemRoute);
}


void SimpleIPVideoServer::setup(const Settings& settings)
{
    BaseServer_<SimpleIPVideoServerSettings, SimpleSessionStore>::setup(settings);
    _fileSystemRoute.setup(settings.fileSystemRouteSettings);
    _postRoute.setup(settings.postRouteSettings);
    _ipVideoRoute.setup(settings.ipVideoRouteSettings);
}


void SimpleIPVideoServer::send(const ofPixels& pix)
{
    _ipVideoRoute.send(pix);
}


std::size_t SimpleIPVideoServer::numConnections() const
{
    return _ipVideoRoute.numConnections();
}


FileSystemRoute& SimpleIPVideoServer::fileSystemRoute()
{
    return _fileSystemRoute;
}


PostRoute& SimpleIPVideoServer::postRoute()
{
    return _postRoute;
}


IPVideoRoute& SimpleIPVideoServer::ipVideoRoute()
{
    return _ipVideoRoute;
}


} } // namespace ofx::HTTP
