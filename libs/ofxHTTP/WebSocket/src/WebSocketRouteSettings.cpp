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


#include "ofx/HTTP/WebSocket/WebSocketRouteSettings.h"


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


} } // namespace ofx::HTTP
