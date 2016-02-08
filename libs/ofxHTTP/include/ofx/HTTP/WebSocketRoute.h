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


#include <set>
#include "ofx/HTTP/BaseRoute.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/WebSocketEvents.h"


namespace ofx {
namespace HTTP {


/// \brief Settings for a WebSocketRoute.
class WebSocketRouteSettings: public BaseRouteSettings
{
public:
    /// \brief A typedef for subprotocols.
    typedef std::set<std::string> SubprotocolSet;

    /// \brief A typedef for origins.
    typedef std::set<std::string> OriginSet;

    /// \brief Create a WebSocketRouteSettings.
    /// \param routePathPattern The regex pattern that this route will handle.
    /// \param requireSecurePort True if this route requires communication
    ///        on an SSL encrypted port.
    WebSocketRouteSettings(const std::string& routePathPattern = DEFAULT_WEBSOCKET_ROUTE_PATH_PATTERN,
                           bool requireSecurePort = false);

    /// \brief Destroy the WebSocketRouteSettings.
    virtual ~WebSocketRouteSettings();

    /// \brief Set the list of valid subprotocols.
    ///
    /// An empty set means that any requested subprotocol will be accepted.  A
    /// non-empty set means that the requested subprotocol MUST be in the set.
    ///
    /// \param subprotocols A set of valid subprotocols.
    void setValidSubprotcols(const SubprotocolSet& subprotocols);

    /// \brief Get a set of valid subprotocols.
    ///
    /// If empty, all requested subprotocols will be accepted.
    ///
    /// \returns The set of valid subprotocols.
    const SubprotocolSet& getValidSubprotocols() const;

    /// \brief Set the list of valid origins.
    ///
    /// An empty set means that any Origin header in the request will be
    /// accepted. A non-empty set means that the request origin MUST be in the
    /// set.
    ///
    /// \param origins A set of valid origins.
    void setValidOrigins(const OriginSet& origins);

    /// \brief Get a set of valid origins.
    ///
    /// If empty, all Origin headers will be accepted.
    ///
    /// \returns The set of valid origins.
    const OriginSet& getValidOrigins() const;

    /// \param autoPingPongResponse If set to true, the WebSocket connection
    ///        will return all PINGs with a PONG.
    void setAutoPingPongResponse(bool autoPingPongResponse);

    /// \returns true iff auto ping pong response is enabled.
    bool getAutoPingPongResponse() const;

    /// \param keepAlive True iff keep alive should be enabled.
    void setKeepAlive(bool keepAlive);

    /// \returns true iff keep alive is enabled.
    bool getKeepAlive() const;

    /// \brief Set the receive timeout.
    /// \param receiveTimeout The amount of time to wait when receiving data.
    void setReceiveTimeout(const Poco::Timespan& receiveTimeout);

    /// \returns the receive timeout.
    Poco::Timespan getReceiveTimeout() const;

    void setSendTimeout(const Poco::Timespan& sendTimeout);

    /// \returns the send timeout.
    Poco::Timespan getSendTimeout() const;

    /// \brief Set the polling timeout.
    /// \param pollTimeout the polling timeout.
    void setPollTimeout(const Poco::Timespan& pollTimeout);

    /// \returns the poll timeout.
    Poco::Timespan getPollTimeout() const;

    /// \brief Set the WebSocket buffers size.
    /// \param bufferSize The buffer size in bytes.
    void setBufferSize(std::size_t bufferSize);

    /// \brief Get the WebSocket buffer size.
    /// \returns the WebSocket buffers size in bytes.
    std::size_t getBufferSize() const;

    /// \brief The default WebSocketRoute path pattern.
    static const std::string DEFAULT_WEBSOCKET_ROUTE_PATH_PATTERN;
    static const Poco::Timespan DEFAULT_RECEIVE_TIMEOUT;
    static const Poco::Timespan DEFAULT_SEND_TIMEOUT;
    static const Poco::Timespan DEFAULT_POLL_TIMEOUT;

    enum
    {
        /// \brief Default buffer size in bytes.
        DEFAULT_BUFFER_SIZE = 8192
    };
    
    
private:
    SubprotocolSet _validSubprotocols;
    OriginSet _validOrigins;
    
    /// \brief Automatically return pong frames.
    bool _autoPingPongResponse;
    bool _keepAlive;
    
    Poco::Timespan _receiveTimeout;
    Poco::Timespan _sendTimeout;
    Poco::Timespan _pollTimeout;
    
    /// \brief WebSocket buffer size in bytes.
    std::size_t _bufferSize;
    
};


/// \brief A route for handling WebSockets.
class WebSocketRoute: public BaseRoute_<WebSocketRouteSettings>
{
public:
    /// \brief A typedef for the WebSocketRouteSettings.
    typedef WebSocketRouteSettings Settings;

    /// \brief Create a WebSocketRoute with the given Settings.
    /// \param settings The WebSocketRoute settings.
    WebSocketRoute(const Settings& settings);

    /// \brief Destroy the WebSocketRoute.
    virtual ~WebSocketRoute();

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const override;

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

    virtual void stop() override;

    /// \brief Send a WebSocketFrame to all connected websockets.
    /// \param frame The frame to send.
    void broadcast(const WebSocketFrame& frame);

    /// \brief Register event listeners for this route.
    ///
    /// The listener class must implement the following callbacks:
    ///
    /// onWebSocketOpenEvent(...), onWebSocketCloseEvent(...),
    /// onWebSocketFrameReceivedEvent(...), onWebSocketFrameSentEvent(...),
    /// onWebSocketErrorEvent(...).
    ///
    /// \tparam ListenerClass The lister class to register.
    /// \param listener A pointer to the listener class.
    /// \param priority The listener priority.
    template <class ListenerClass>
    void registerWebSocketEvents(ListenerClass* listener,
                                 int priority = OF_EVENT_ORDER_AFTER_APP);

    /// \brief Unregister event listeners for this route.
    /// \tparam ListenerClass The lister class to uregister.
    /// \param listener A pointer to the listener class.
    /// \param priority The listener priority.
    template <class ListenerClass>
    void unregisterWebSocketEvents(ListenerClass* listener,
                                   int priority = OF_EVENT_ORDER_AFTER_APP);

    /// \returns The number of active WebSocketConnections.
    std::size_t getNumWebSocketConnections() const;

    /// \returns a reference to the filter factories.
    const std::vector<std::unique_ptr<AbstractWebSocketFilterFactory>>& getFilterFactories() const;

    /// \brief WebSocketEvents for WebSocket callbacks.
    WebSocketEvents events;

protected:
    void registerWebSocketConnection(WebSocketConnection* connection);
    void unregisterWebSocketConnection(WebSocketConnection* connection);

    friend class WebSocketConnection;
    
private:
    /// \brief A collection of filter factories.
    std::vector<std::unique_ptr<AbstractWebSocketFilterFactory>> _filterFactories;

    /// \brief A collection of WebSocketConnections.
    std::set<WebSocketConnection*> _connections;

    /// \brief The mutex that locks the handler set.
    mutable std::mutex _mutex;

};


template <class ListenerClass>
void WebSocketRoute::registerWebSocketEvents(ListenerClass* listener, int priority)
{
    ofAddListener(events.onWebSocketOpenEvent, listener, &ListenerClass::onWebSocketOpenEvent, priority);
    ofAddListener(events.onWebSocketCloseEvent, listener, &ListenerClass::onWebSocketCloseEvent, priority);
    ofAddListener(events.onWebSocketFrameReceivedEvent, listener, &ListenerClass::onWebSocketFrameReceivedEvent, priority);
    ofAddListener(events.onWebSocketFrameSentEvent,listener, &ListenerClass::onWebSocketFrameSentEvent, priority);
    ofAddListener(events.onWebSocketErrorEvent, listener, &ListenerClass::onWebSocketErrorEvent, priority);
}


template <class ListenerClass>
void WebSocketRoute::unregisterWebSocketEvents(ListenerClass* listener, int priority)
{
    ofRemoveListener(events.onWebSocketOpenEvent, listener, &ListenerClass::onWebSocketOpenEvent, priority);
    ofRemoveListener(events.onWebSocketCloseEvent, listener, &ListenerClass::onWebSocketCloseEvent, priority);
    ofRemoveListener(events.onWebSocketFrameReceivedEvent, listener, &ListenerClass::onWebSocketFrameReceivedEvent, priority);
    ofRemoveListener(events.onWebSocketFrameSentEvent, listener, &ListenerClass::onWebSocketFrameSentEvent, priority);
    ofRemoveListener(events.onWebSocketErrorEvent, listener, &ListenerClass::onWebSocketErrorEvent, priority);
}


} } // namespace ofx::HTTP
