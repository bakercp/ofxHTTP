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


#include "IPVideoRouteSettings.h"


namespace ofx {
namespace HTTP {


const std::string IPVideoRouteSettings::DEFAULT_VIDEO_ROUTE = "/ipvideo";
const std::size_t IPVideoRouteSettings::DEFAULT_MAX_CLIENT_CONNECTIONS = 5;
const std::size_t IPVideoRouteSettings::DEFAULT_MAX_CLIENT_BITRATE = 1024;
const std::size_t IPVideoRouteSettings::DEFAULT_MAX_CLIENT_FRAMERATE = 30;
const std::size_t IPVideoRouteSettings::DEFAULT_MAX_CLIENT_QUEUE_SIZE = 10;
const std::string IPVideoRouteSettings::DEFAULT_BOUNDARY_MARKER = "--boundary";
const Poco::Net::MediaType IPVideoRouteSettings::DEFAULT_MEDIA_TYPE = Poco::Net::MediaType("multipart/x-mixed-replace");
const std::size_t IPVideoRouteSettings::DEFAULT_MAX_STREAM_WIDTH = 1920;
const std::size_t IPVideoRouteSettings::DEFAULT_MAX_STREAM_HEIGHT = 1080;

IPVideoRouteSettings::IPVideoRouteSettings(const std::string& routePathPattern):
    BaseRouteSettings(routePathPattern),
    _maxClientConnections(DEFAULT_MAX_CLIENT_CONNECTIONS),
    _maxClientBitRate(DEFAULT_MAX_CLIENT_BITRATE),
    _maxClientFrameRate(DEFAULT_MAX_CLIENT_FRAMERATE),
    _maxClientQueueSize(DEFAULT_MAX_CLIENT_QUEUE_SIZE),
    _maxStreamWidth(DEFAULT_MAX_STREAM_WIDTH),
    _maxStreamHeight(DEFAULT_MAX_STREAM_HEIGHT),
    _boundaryMarker(DEFAULT_BOUNDARY_MARKER),
    _mediaType(DEFAULT_MEDIA_TYPE)
{
}

IPVideoRouteSettings::~IPVideoRouteSettings()
{
}

void IPVideoRouteSettings::setMaxClientConnections(std::size_t maxClientConnections)
{
    _maxClientConnections = maxClientConnections;
}

std::size_t IPVideoRouteSettings::getMaxClientConnections() const
{
    return _maxClientConnections;
}

void IPVideoRouteSettings::setMaxClientBitRate(std::size_t maxClientBitRate)
{
    _maxClientBitRate = maxClientBitRate;
}

std::size_t IPVideoRouteSettings::getMaxClientBitRate() const
{
    return _maxClientBitRate;
}

void IPVideoRouteSettings::setMaxClientFrameRate(std::size_t maxClientFrameRate)
{
    _maxClientFrameRate = maxClientFrameRate;
}

void IPVideoRouteSettings::setMaxClientQueueSize(std::size_t maxClientQueueSize)
{
    _maxClientQueueSize = maxClientQueueSize;
}

std::size_t IPVideoRouteSettings::getMaxClientQueueSize() const
{
    return _maxClientQueueSize;
}

std::size_t IPVideoRouteSettings::getMaxClientFrameRate() const
{
    return _maxClientFrameRate;
}

void IPVideoRouteSettings::setBoundaryMarker(const std::string& boundaryMarker)
{
    _boundaryMarker = boundaryMarker;
}

std::string IPVideoRouteSettings::getBoundaryMarker() const
{
    return _boundaryMarker;
}

void IPVideoRouteSettings::setMediaType(const Poco::Net::MediaType& mediaType)
{
    _mediaType = mediaType;
}

Poco::Net::MediaType IPVideoRouteSettings::getMediaType() const
{
    return _mediaType;
}

void IPVideoRouteSettings::setMaxStreamWidth(std::size_t maxStreamWidth)
{
    _maxStreamWidth = maxStreamWidth;
}

std::size_t IPVideoRouteSettings::getMaxStreamWidth() const
{
    return _maxStreamWidth;
}

void IPVideoRouteSettings::setMaxStreamHeight(std::size_t maxStreamHeight)
{
    _maxStreamHeight = maxStreamHeight;
}

std::size_t IPVideoRouteSettings::getMaxStreamHeight() const
{
    return _maxStreamHeight;
}


} } // namespace ofx:HTTP
