// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/Server/BasicServer.h"
#include "ofx/HTTP/WebSocket/WebSocketRoute.h"
#include "ofx/HTTP/WebSocket/WebSocketRouteSettings.h"


namespace ofx {
namespace HTTP {


/// \brief Aggregate settings for a BasicWebSocketServer.
class BasicWebSocketServerSettings:
    public WebSocketRouteSettings,
    public BasicServerSettings
{
};


/// \brief A basic implementation of an HTTP Server supporting WebSockets.
class BasicWebSocketServer: public BasicServer
{
public:
    /// \brief A typedef for a shared pointer.
    typedef std::shared_ptr<BasicWebSocketServer> SharedPtr;

    /// \brief A typedef for a weak pointer.
    typedef std::weak_ptr<BasicWebSocketServer> WeakPtr;

    /// \brief A typedef for the BasicWebSocketServerSettings.
    typedef BasicWebSocketServerSettings Settings;

    /// \brief Create a BasicWebSocketServer with the provided Settings.
    /// \param settings The Settings used to configure the server.
    BasicWebSocketServer(const Settings& settings = Settings());

    /// \brief Destroy the BasicWebSocketServer.
    virtual ~BasicWebSocketServer();

    /// \returns the WebSocketRoute attached to this server.
    WebSocketRoute::SharedPtr getWebSocketRoute();

    /// \brief Construct a shared pointer to this server.
    /// \param settings The Settings used to configure the server.
    /// \note This will be replaced with C++11 (std::make_shared<...>).
    static SharedPtr makeShared(const Settings& settings = Settings())
    {
        return SharedPtr(new BasicWebSocketServer(settings));
    }

private:
    /// \brief The WebSocketRoute attached to this server.
    WebSocketRoute::SharedPtr _webSocketRoute;

};

    
} } // namespace ofx::HTTP
