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


#include "ofx/HTTP/WebSocketRoute.h"
#include "ofx/HTTP/WebSocketExtensions.h"
#include "ofx/HTTP/WebSocketConnection.h"


namespace ofx {
namespace HTTP {


const std::string WebSocketRouteSettings::DEFAULT_WEBSOCKET_ROUTE_PATH_PATTERN = "/";
const Poco::Timespan WebSocketRouteSettings::DEFAULT_RECEIVE_TIMEOUT = Poco::Timespan(60 * Poco::Timespan::SECONDS);
const Poco::Timespan WebSocketRouteSettings::DEFAULT_SEND_TIMEOUT = Poco::Timespan(60 * Poco::Timespan::SECONDS);
const Poco::Timespan WebSocketRouteSettings::DEFAULT_POLL_TIMEOUT = Poco::Timespan(10 * Poco::Timespan::MILLISECONDS);


WebSocketRouteSettings::WebSocketRouteSettings(const std::string& routePathPattern,
                                               bool requireSecurePort):
    BaseRouteSettings(routePathPattern, requireSecurePort),
    _autoPingPongResponse(true),
    _keepAlive(true),
    _receiveTimeout(DEFAULT_RECEIVE_TIMEOUT),
    _sendTimeout(DEFAULT_SEND_TIMEOUT),
    _pollTimeout(DEFAULT_POLL_TIMEOUT),
    _bufferSize(DEFAULT_BUFFER_SIZE)
{
}


WebSocketRouteSettings::~WebSocketRouteSettings()
{
}


void WebSocketRouteSettings::setValidSubprotcols(const SubprotocolSet& validSubprotocols)
{
    _validSubprotocols = validSubprotocols;
}


const WebSocketRouteSettings::SubprotocolSet& WebSocketRouteSettings::getValidSubprotocols() const
{
    return _validSubprotocols;
}


void WebSocketRouteSettings::setValidOrigins(const OriginSet& validOrigins)
{
    _validOrigins = validOrigins;
}


const WebSocketRouteSettings::OriginSet& WebSocketRouteSettings::getValidOrigins() const
{
    return _validOrigins;
}


void WebSocketRouteSettings::setAutoPingPongResponse(bool autoPingPongResponse)
{
    _autoPingPongResponse = autoPingPongResponse;
}


bool WebSocketRouteSettings::getAutoPingPongResponse() const
{
    return _autoPingPongResponse;
}


void WebSocketRouteSettings::setKeepAlive(bool keepAlive)
{
    _keepAlive = keepAlive;
}


bool WebSocketRouteSettings::getKeepAlive() const
{
    return _keepAlive;
}


void WebSocketRouteSettings::setReceiveTimeout(const Poco::Timespan& receiveTimeout)
{
    _receiveTimeout = receiveTimeout;
}


Poco::Timespan WebSocketRouteSettings::getReceiveTimeout() const
{
    return _receiveTimeout;
}


void WebSocketRouteSettings::setSendTimeout(const Poco::Timespan& sendTimeout)
{
    _sendTimeout = sendTimeout;
}


Poco::Timespan WebSocketRouteSettings::getSendTimeout() const
{
    return _sendTimeout;
}


void WebSocketRouteSettings::setPollTimeout(const Poco::Timespan& pollTimeout)
{
    _pollTimeout = pollTimeout;
}


Poco::Timespan WebSocketRouteSettings::getPollTimeout() const
{
    return _pollTimeout;
}


void WebSocketRouteSettings::setBufferSize(std::size_t bufferSize)
{
    _bufferSize = bufferSize;
}


std::size_t WebSocketRouteSettings::getBufferSize() const
{
    return _bufferSize;
}


WebSocketRoute::WebSocketRoute(const Settings& settings):
    BaseRoute_<WebSocketRouteSettings>(settings)
{
    _filterFactories.push_back(std::make_unique<WebSocketPerMessageCompressionFactory>());
}


WebSocketRoute::~WebSocketRoute()
{
}


bool WebSocketRoute::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                      bool isSecurePort) const
{
    if (!BaseRoute_<WebSocketRouteSettings>::canHandleRequest(request, isSecurePort))
    {
        return false;
    }

    // check to see if this is an websocket upgrade request
    if (0 != Poco::icompare(request.get("Upgrade", ""), "websocket"))
    {
        return false;
    }

    // TODO: firefox hack
    // require websocket upgrade headers
    // this is all fixed in Poco 1.4.6 and 1.5.+
    std::string connectionHeader = Poco::toLower(request.get("Connection", ""));

    if (Poco::icompare(connectionHeader, "Upgrade") != 0 &&
        !ofIsStringInString(connectionHeader, "upgrade"))
    {
        // this request is coming from firefox, which is known to send things that look like:
        // Connection:keep-alive, Upgrade
        // thus failing the standard Poco upgrade test.
        // we can't do this here, but will do a similar hack in the handler
        return false;
    }
    
    return true;
}


Poco::Net::HTTPRequestHandler* WebSocketRoute::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
    return new WebSocketConnection(*this);
}


void WebSocketRoute::stop()
{
    // std::unique_lock<std::mutex> lock(_mutex);
    auto iter = _connections.begin();

    while (iter != _connections.end())
    {
        (*iter)->stop();
        ++iter;
    }
}


//bool WebSocketRoute::sendFrame(const WebSocketConnection* connection,
//                               const WebSocketFrame& frame)
//{
//    if (connection)
//    {
//        return connection->sendFrame(frame);
//    }
//    else
//    {
//        ofLogError("WebSocketRoute::sendFrame") << "0 == handler";
//        return false;
//    }
//}
//

//void WebSocketRoute::close(WebSocketConnection* connection)
//{
//    if (connection)
//    {
//        connection->stop();
//    }
//    else
//    {
//        ofLogError("WebSocketRoute::close") << "0 == handler";
//    }
//}


//void WebSocketRoute::close()
//{
//    // std::unique_lock<std::mutex> lock(_mutex);
//    auto iter = _connections.begin();
//
//    while (iter != _connections.end())
//    {
//        close(*iter);
//        ++iter;
//    }
//}


void WebSocketRoute::broadcast(const WebSocketFrame& frame)
{
    std::unique_lock<std::mutex> lock(_mutex);
    auto iter = _connections.begin();

    while (iter != _connections.end())
    {
        (*iter)->sendFrame(frame);
        ++iter;
    }
}


void WebSocketRoute::registerWebSocketConnection(WebSocketConnection* connection)
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (!_connections.insert(connection).second)
    {
        ofLogError("BaseWebSocketSessionManager::registerRouteHandler") << "Element was already in set!";
    }
}


void WebSocketRoute::unregisterWebSocketConnection(WebSocketConnection* connection)
{
    std::unique_lock<std::mutex> lock(_mutex);
    // TODO: this will never return more than 1
    std::size_t numErased = _connections.erase(connection);

    // TODO: this is strange.
    if (1 != numErased)
    {
        ofLogError("BaseWebSocketSessionManager::unregisterRouteHandler") << "1 != numErased" << numErased;
    }
}


std::size_t WebSocketRoute::getNumWebSocketConnections() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _connections.size();
}


const std::vector<std::unique_ptr<AbstractWebSocketFilterFactory>>& WebSocketRoute::getFilterFactories() const
{
    return _filterFactories;
}


} } // namespace ofx::HTTP
