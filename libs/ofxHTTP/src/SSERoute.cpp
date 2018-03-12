//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/SSERoute.h"
#include "ofx/HTTP/SSEConnection.h"


namespace ofx {
namespace HTTP {


const std::string SSERouteSettings::DEFAULT_SSE_ROUTE_PATH_PATTERN = "/event-source";


SSERouteSettings::SSERouteSettings(const std::string& routePathPattern,
                                   bool requireSecurePort):
    BaseRouteSettings(routePathPattern, requireSecurePort),
    _clientRetryInterval(DEFAULT_CLIENT_RETRY_INTERVAL),
    _cacheSize(DEFAULT_CACHE_SIZE)
{
}


SSERouteSettings::~SSERouteSettings()
{
}


void SSERouteSettings::setClientRetryInterval(uint64_t clientRetryInterval)
{
    _clientRetryInterval = clientRetryInterval;
}


uint64_t SSERouteSettings::getClientRetryInterval() const
{
    return _clientRetryInterval;
}


std::size_t SSERouteSettings::getCacheSize() const
{
    return _cacheSize;
}


void SSERouteSettings::setCacheSize(std::size_t cacheSize)
{
    _cacheSize = cacheSize;
}


SSERoute::SSERoute(const Settings& settings):
    BaseRoute_<SSERouteSettings>(settings)
{
}


SSERoute::~SSERoute()
{
}


bool SSERoute::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                bool isSecurePort) const
{
    return BaseRoute_<SSERouteSettings>::canHandleRequest(request, isSecurePort);
}


Poco::Net::HTTPRequestHandler* SSERoute::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
    // std::cout << "CREATING SSE CONNECTION ... " << std::endl;
    // The caller takes ownership of the SSEConnection's memory.
    return new SSEConnection(*this);
}


void SSERoute::stop()
{
    // std::unique_lock<std::mutex> lock(_mutex);
    for (auto& connection : _connections)
    {
        connection->stop();
    }
}


void SSERoute::send(const std::string& data, bool cache)
{
    send(SSEFrame(data), cache);
}


void SSERoute::send(const std::string& event, const std::string& data, bool cache)
{
    send(SSEFrame(event, data), cache);
}


void SSERoute::send(const SSEFrame& frame, bool cache)
{
    std::unique_lock<std::mutex> lock(_mutex);

    uint64_t cacheIndex = cache ? (++_cacheIndex) : IndexedSSEFrame::NOT_INDEXED;

    IndexedSSEFrame indexedFrame(frame, cacheIndex);

    if (cache)
    {
        _frameCache.push(indexedFrame);

        while (_frameCache.size() > settings().getCacheSize())
        {
            _frameCache.pop();
        }
    }

    for (auto& connection : _connections)
    {
        connection->send(indexedFrame);
    }
}


void SSERoute::registerConnection(SSEConnection* connection)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (!_connections.insert(connection).second)
    {
        ofLogError("SSERoute::registerRouteHandler") << "Element was already in set!";
    }
}


void SSERoute::unregisterConnection(SSEConnection* connection)
{
    std::unique_lock<std::mutex> lock(_mutex);
    // TODO: this will never return more than 1
    std::size_t numErased = _connections.erase(connection);

    // TODO: this is strange.
    if (1 != numErased)
    {
        ofLogError("SSERoute::unregisterRouteHandler") << "1 != numErased" << numErased;
    }
}


std::size_t SSERoute::numConnections() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _connections.size();
}


std::size_t SSERoute::frameCacheSize() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _frameCache.size();
}


void SSERoute::clearFrameCache()
{
    std::unique_lock<std::mutex> lock(_mutex);
    std::queue<IndexedSSEFrame> empty; // a way to clear queues.
    std::swap(_frameCache, empty);
}


} } // namespace ofx::HTTP
