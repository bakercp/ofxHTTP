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


#include "WebSocketRouteSettings.h"


namespace ofx {
namespace HTTP {




WebSocketRouteSettings::WebSocketRouteSettings(const std::string& routePathPattern):
    BaseRouteSettings(routePathPattern),
    _subprotocol(""),
    _allowEmptySubprotocol(false),
    _allowCrossOriginConnections(false),
    _isBinary(false),
    _autoPingPongResponse(true),
    _keepAlive(true),
    _receiveTimeout(Poco::Timespan(60 * Poco::Timespan::SECONDS)),
    _sendTimeout(Poco::Timespan(60 * Poco::Timespan::SECONDS)),
    _pollTimeout(Poco::Timespan(10 * Poco::Timespan::MILLISECONDS)),
    _bufferSize(1024)
{
}

WebSocketRouteSettings::~WebSocketRouteSettings()
{
}

void WebSocketRouteSettings::setSubprotocol(const std::string& subprotocol)
{
    _subprotocol = subprotocol;
}

std::string WebSocketRouteSettings::getSubprotocol() const
{
    return _subprotocol;
}

void WebSocketRouteSettings::setAllowEmptySubprotocol(bool allowEmptySubprotocol)
{
    _allowEmptySubprotocol = allowEmptySubprotocol;
}

bool WebSocketRouteSettings::getAllowEmptySubprotocol() const
{
    return _allowEmptySubprotocol;
}

void WebSocketRouteSettings::setAllowCrossOriginConnections(bool allowCrossOriginConnections)
{
    _allowCrossOriginConnections = allowCrossOriginConnections;
}

bool WebSocketRouteSettings::getAllowCrossOriginConnections() const
{
    return _allowCrossOriginConnections;
}

void WebSocketRouteSettings::setIsBinary(bool isBinary)
{
    _isBinary = isBinary;
}

bool WebSocketRouteSettings::getIsBinary() const
{
    return _isBinary;
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
