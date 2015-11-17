// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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
#include "ofx/HTTP/WebSocketRoute.h"


namespace ofx {
namespace HTTP {


/// \brief Aggregate settings for a SimpleWebSocketServer.
class SimpleWebSocketServerSettings: public BaseServerSettings
{
public:
    FileSystemRouteSettings fileSystemRouteSettings;
    WebSocketRouteSettings webSocketServerSettings;
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

    virtual void setup(const Settings& settings);

    FileSystemRoute& getFileSystemRoute();

    /// \returns the WebSocketRoute attached to this server.
    WebSocketRoute& getWebSocketRoute();

private:
    /// \brief The FileSystemRoute attached to this server.
    FileSystemRoute _fileSystemRoute;

    /// \brief The WebSocketRoute attached to this server.
    WebSocketRoute _webSocketRoute;

};

    
} } // namespace ofx::HTTP
