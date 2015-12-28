// =============================================================================
//
// Copyright (c) 2012-2015 Christopher Baker <http://christopherbaker.net>
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


#pragma once


#include "ofx/HTTP/BaseServer.h"
#include "ofx/HTTP/FileSystemRoute.h"
#include "ofx/HTTP/IPVideoRoute.h"
#include "ofx/HTTP/PostRoute.h"


namespace ofx {
namespace HTTP {


class SimpleIPVideoServerSettings: public BaseServerSettings
{
public:
    FileSystemRouteSettings fileSystemRouteSettings;
    PostRouteSettings postRouteSettings;
    IPVideoRouteSettings ipVideoRouteSettings;
};


class SimpleIPVideoServer: public BaseServer_<SimpleIPVideoServerSettings, SimpleSessionStore>
{
public:
    typedef SimpleIPVideoServerSettings Settings;

    SimpleIPVideoServer(const Settings& settings = Settings());

    virtual ~SimpleIPVideoServer();

    virtual void setup(const Settings& settings);

    void send(ofPixels& pix);

    std::size_t getNumConnections() const;

    FileSystemRoute& getFileSystemRoute();
    PostRoute& getPostRoute();
    IPVideoRoute& getIPVideoRoute();

protected:
    FileSystemRoute _fileSystemRoute;
    PostRoute _postRoute;
    IPVideoRoute _ipVideoRoute;

};


} } // namespace ofx::HTTP
