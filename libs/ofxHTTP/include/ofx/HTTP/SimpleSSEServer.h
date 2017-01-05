// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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
#include "ofx/HTTP/SSERoute.h"


namespace ofx {
namespace HTTP {


/// \brief Aggregate settings for a SimpleSSEServer.
class SimpleSSEServerSettings: public BaseServerSettings
{
public:
    /// \brief File system route settings.
    FileSystemRouteSettings fileSystemRouteSettings;

    /// \brief Server sent event route settings.
    SSERouteSettings sseRouteSettings;
    
};


/// \brief A basic implementation of an HTTP Server supporting server sent events.
class SimpleSSEServer: public BaseServer_<SimpleSSEServerSettings, SimpleSessionStore>
{
public:
    /// \brief A typedef for the SimpleSSEServerSettings.
    typedef SimpleSSEServerSettings Settings;

    /// \brief Create a SimpleSSEServer with the provided Settings.
    /// \param settings The Settings used to configure the server.
    SimpleSSEServer(const Settings& settings = Settings());

    /// \brief Destroy the SimpleSSEServer.
    virtual ~SimpleSSEServer();

    virtual void setup(const Settings& settings) override;

    /// \returns the file system route.
    FileSystemRoute& fileSystemRoute();

    /// \returns the SSERoute attached to this server.
    SSERoute& sseRoute();

private:
    /// \brief The FileSystemRoute attached to this server.
    FileSystemRoute _fileSystemRoute;

    /// \brief The SSERoute attached to this server.
    SSERoute _sseRoute;

};

    
} } // namespace ofx::HTTP
