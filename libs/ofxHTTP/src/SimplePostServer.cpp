//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SimplePostServer.h"


namespace ofx {
namespace HTTP {


SimplePostServer::SimplePostServer(const Settings& settings):
    BaseServer_<SimplePostServerSettings, SimpleSessionStore>(settings),
    _fileSystemRoute(settings.fileSystemRouteSettings),
    _postRoute(settings.postRouteSettings)
{
    addRoute(&_fileSystemRoute);
    addRoute(&_postRoute);
}


SimplePostServer::~SimplePostServer()
{
    removeRoute(&_postRoute);
    removeRoute(&_fileSystemRoute);
}


void SimplePostServer::setup(const Settings& settings)
{
    BaseServer_<SimplePostServerSettings, SimpleSessionStore>::setup(settings);
    _fileSystemRoute.setup(settings.fileSystemRouteSettings);
    _postRoute.setup(settings.postRouteSettings);
}


PostRoute& SimplePostServer::postRoute()
{
    return _postRoute;
}


} } // namespace ofx::HTTP
