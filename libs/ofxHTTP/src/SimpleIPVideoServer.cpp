// =============================================================================
//
// Copyright (c) 2012-2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


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


void SimpleIPVideoServer::send(ofPixels& pix)
{
    _ipVideoRoute.send(pix);
}


std::size_t SimpleIPVideoServer::getNumConnections() const
{
    return _ipVideoRoute.getNumConnections();
}


FileSystemRoute& SimpleIPVideoServer::getFileSystemRoute()
{
    return _fileSystemRoute;
}


PostRoute& SimpleIPVideoServer::getPostRoute()
{
    return _postRoute;
}


IPVideoRoute& SimpleIPVideoServer::getIPVideoRoute()
{
    return _ipVideoRoute;
}


} } // namespace ofx::HTTP
