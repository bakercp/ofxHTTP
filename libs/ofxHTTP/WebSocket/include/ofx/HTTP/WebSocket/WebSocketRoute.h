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


#include <set>
#include "ofx/HTTP/Server/BaseRoute.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/WebSocket/WebSocketRouteSettings.h"
#include "ofx/HTTP/WebSocket/WebSocketEvents.h"


namespace ofx {
namespace HTTP {


/// \brief A route for handling WebSockets.
class WebSocketRoute: public BaseRoute_<WebSocketRouteSettings>
{
public:
    /// \brief A typedef for a shared pointer.
    typedef std::shared_ptr<WebSocketRoute> SharedPtr;

    /// \brief A typedef for a weak pointer.
    typedef std::weak_ptr<WebSocketRoute> WeakPtr;

    /// \brief A typedef for the WebSocketRouteSettings.
    typedef WebSocketRouteSettings Settings;

    /// \brief Create a WebSocketRoute with the given Settings.
    /// \param settings The WebSocketRoute settings.
    WebSocketRoute(const Settings& settings);

    /// \brief Destroy the WebSocketRoute.
    virtual ~WebSocketRoute();

    /// \brief A custom canHandleRequest extends the BaseRoute to check
    ///        for WebSocket upgrade headers.
    /// \param request The HTTPServerRequest to test.
    /// \param isSecurePort Indicates whether the request was passed on
    ///        an SSL encrypted port.
    /// \sa BaseRoute_<WebSocketRouteSettings>::canHandleRequest()
    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const;

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    virtual void stop();

    bool sendFrame(const WebSocketConnection* connection,
                   const WebSocketFrame& frame);

    void broadcast(const WebSocketFrame& frame);

    void close(WebSocketConnection* connection);

    void close();

    template<class ListenerClass>
    void registerWebSocketEvents(ListenerClass* listener);

    template<class ListenerClass>
    void unregisterWebSocketEvents(ListenerClass* listener);

    std::size_t getNumWebSocketConnections() const;

    WebSocketEvents events;
        ///< \brief WebSocketEvents for WebSocket callbacks.

    static SharedPtr makeShared(const Settings& settings)
    {
        return SharedPtr(new WebSocketRoute(settings));
    }

protected:
    void registerWebSocketConnection(WebSocketConnection* connection);
    void unregisterWebSocketConnection(WebSocketConnection* connection);

    friend class WebSocketConnection;
    
private:
    typedef std::set<WebSocketConnection*>          WebSocketConnections;
    typedef WebSocketConnections::iterator          WebSocketConnectionsIter;

    WebSocketConnections _connections;

    mutable ofMutex _mutex; // locks the handlers set

};


template<class ListenerClass>
void WebSocketRoute::registerWebSocketEvents(ListenerClass* listener)
{
    ofAddListener(events.onOpenEvent, listener, &ListenerClass::onWebSocketOpenEvent);
    ofAddListener(events.onCloseEvent,listener, &ListenerClass::onWebSocketCloseEvent);
    ofAddListener(events.onFrameReceivedEvent, listener, &ListenerClass::onWebSocketFrameReceivedEvent);
    ofAddListener(events.onFrameSentEvent,listener,&ListenerClass::onWebSocketFrameSentEvent);
    ofAddListener(events.onErrorEvent,listener,&ListenerClass::onWebSocketErrorEvent);
}


template<class ListenerClass>
void WebSocketRoute::unregisterWebSocketEvents(ListenerClass* listener)
{
    ofRemoveListener(events.onOpenEvent,listener,&ListenerClass::onWebSocketOpenEvent);
    ofRemoveListener(events.onCloseEvent,listener,&ListenerClass::onWebSocketCloseEvent);
    ofRemoveListener(events.onFrameReceivedEvent,listener,&ListenerClass::onWebSocketFrameReceivedEvent);
    ofRemoveListener(events.onFrameSentEvent,listener,&ListenerClass::onWebSocketFrameSentEvent);
    ofRemoveListener(events.onErrorEvent,listener,&ListenerClass::onWebSocketErrorEvent);
}


} } // namespace ofx::HTTP
