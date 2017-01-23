//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <set>
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/BaseRoute.h"
#include "ofx/HTTP/SSEEvents.h"


namespace ofx {
namespace HTTP {


class SSEConnection;


/// \brief Settings for a SSERoute.
class SSERouteSettings: public BaseRouteSettings
{
public:
    /// \brief Create a SSERouteSettings.
    /// \param routePathPattern The regex pattern that this route will handle.
    /// \param requireSecurePort True if this route requires communication
    ///        on an SSL encrypted port.
    SSERouteSettings(const std::string& routePathPattern = DEFAULT_SSE_ROUTE_PATH_PATTERN,
                     bool requireSecurePort = false);

    /// \brief Destroy the SSERouteSettings.
    virtual ~SSERouteSettings();

    /// \brief Set the client retry interval in milliseconds.
    /// \param clientRetryInterval the client retry interval in milliseconds.
    void setClientRetryInterval(uint64_t clientRetryInterval);

    /// \brief Get the client retry interval in milliseconds.
    /// \returns the client retry interval in milliseconds.
    uint64_t getClientRetryInterval() const;

    /// \brief Set the maximum SSEFrame cache size per connection.
    /// \param cacheSize The maximum number of frames to cache per connection.
    void setCacheSize(std::size_t cacheSize) const;

    /// \brief Get the maximum SSEFrame cache size.
    std::size_t getCacheSize() const;

    /// \brief Set the maximum SSEFrame cache size.
    /// \param size The maximum SSEFrame cache size.
    void setCacheSize(std::size_t size);

    enum
    {
        /// \brief The client retry interval in milliseconds.
        DEFAULT_CLIENT_RETRY_INTERVAL = 15000,

        /// \brief The default client message cache size.
        DEFAULT_CACHE_SIZE = 25

    };

    /// \brief The default WebSocketRoute path pattern.
    static const std::string DEFAULT_SSE_ROUTE_PATH_PATTERN;

private:
    /// \brief The client retry interval sent with the retry: SSE message.
    uint64_t _clientRetryInterval;

    /// \brief The maximum SSEFrame cache size.
    std::size_t _cacheSize;

};


/// \brief A route for handling WebSockets.
class SSERoute: public BaseRoute_<SSERouteSettings>
{
public:
    /// \brief A typedef for the SSERouteSettings.
    typedef SSERouteSettings Settings;

    /// \brief Create a SSERoute with the given Settings.
    /// \param settings The WebSocketRoute settings.
    SSERoute(const Settings& settings);

    /// \brief Destroy the SSERoute.
    virtual ~SSERoute();

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const override;

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

    virtual void stop() override;

    /// \brief Send a SSEFrame to all connections.
    /// \param data The frame to send.
    /// \param cache True if the frame should be indexed and cached.
    void send(const std::string& data, bool cache = false);

    /// \brief Send a SSEFrame to all connections.
    /// \param event The event name to send.
    /// \param data The frame to send.
    /// \param cache True if the frame should be indexed and cached.
    void send(const std::string& event, const std::string& data, bool cache = false);

    /// \brief Send a SSEFrame to all connected connections.
    /// \param frame The frame to send.
    /// \param cache True if the frame should be indexed and cached.
    void send(const SSEFrame& frame, bool cache = false);

    /// \returns The number of active SSEConnections.
    std::size_t numConnections() const;

    /// \returns the size of the frame cache.
    std::size_t frameCacheSize() const;

    /// \brief Clears the frame cache.
    void clearFrameCache();

    /// \brief Register event listeners for this route.
    ///
    /// The listener class must implement the following callbacks:
    ///
    /// onSSEOpenEvent(...), onSSECloseEvent(...),
    /// onSSEFrameSentEvent(...).
    ///
    /// \tparam ListenerClass The lister class to register.
    /// \param listener A pointer to the listener class.
    /// \param priority The listener priority.
    template <class ListenerClass>
    void registerEventListeners(ListenerClass* listener,
                                int priority = OF_EVENT_ORDER_AFTER_APP);

    /// \brief Unregister event listeners for this route.
    /// \tparam ListenerClass The lister class to uregister.
    /// \param listener A pointer to the listener class.
    /// \param priority The listener priority.
    template <class ListenerClass>
    void unregisterEventListeners(ListenerClass* listener,
                                  int priority = OF_EVENT_ORDER_AFTER_APP);

    /// \brief SSEEvents for SSE callbacks.
    SSEEvents events;

private:
    void registerConnection(SSEConnection* connection);
    void unregisterConnection(SSEConnection* connection);

    /// \brief A collection of SSEConnection.
    std::set<SSEConnection*> _connections;

    /// \brief A queue of SSEFrames cached for reconnects.
    mutable std::queue<IndexedSSEFrame> _frameCache;

    /// \brief The current cache index.
    mutable std::uint64_t _cacheIndex = 0;
    
    /// \brief The mutex that locks the handler set.
    mutable std::mutex _mutex;

    friend class SSEConnection;

};


template <class ListenerClass>
void SSERoute::registerEventListeners(ListenerClass* listener, int priority)
{
    ofAddListener(events.onSSEOpenEvent, listener, &ListenerClass::onSSEOpenEvent, priority);
    ofAddListener(events.onSSECloseEvent, listener, &ListenerClass::onSSECloseEvent, priority);
    ofAddListener(events.onSSEFrameSentEvent, listener, &ListenerClass::onSSEFrameSentEvent, priority);
}


template <class ListenerClass>
void SSERoute::unregisterEventListeners(ListenerClass* listener, int priority)
{
    ofRemoveListener(events.onSSEOpenEvent, listener, &ListenerClass::onSSEOpenEvent, priority);
    ofRemoveListener(events.onSSECloseEvent, listener, &ListenerClass::onSSECloseEvent, priority);
    ofRemoveListener(events.onSSEFrameSentEvent, listener, &ListenerClass::onSSEFrameSentEvent, priority);
}


} } // namespace ofx::HTTP
