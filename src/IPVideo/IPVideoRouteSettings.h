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


#include "Poco/Net/MediaType.h"
#include "BaseRouteSettings.h"


namespace ofx {
namespace HTTP {


class IPVideoRouteSettings: public BaseRouteSettings
{
public:
    IPVideoRouteSettings(const std::string& routePathPattern = DEFAULT_VIDEO_ROUTE);
    virtual ~IPVideoRouteSettings();

    void setMaxClientConnections(std::size_t maxClientConnections);
    std::size_t getMaxClientConnections() const;

    void setMaxClientBitRate(std::size_t maxClientBitRate);
    std::size_t getMaxClientBitRate() const;

    void setMaxClientFrameRate(std::size_t maxClientFrameRate);
    std::size_t getMaxClientFrameRate() const;

    void setMaxClientQueueSize(std::size_t maxClientQueueSize);
    std::size_t getMaxClientQueueSize() const;

    void setBoundaryMarker(const std::string& boundaryMarker);
    std::string getBoundaryMarker() const;

    void setMediaType(const Poco::Net::MediaType& mediaType);
    Poco::Net::MediaType getMediaType() const;

    void setMaxStreamWidth(std::size_t maxStreamWidth);
    std::size_t getMaxStreamWidth() const;

    void setMaxStreamHeight(std::size_t maxStreamHeight);
    std::size_t getMaxStreamHeight() const;

    static const std::string DEFAULT_VIDEO_ROUTE;
    static const std::size_t DEFAULT_MAX_CLIENT_CONNECTIONS;
    static const std::size_t DEFAULT_MAX_CLIENT_BITRATE;
    static const std::size_t DEFAULT_MAX_CLIENT_FRAMERATE;
    static const std::size_t DEFAULT_MAX_CLIENT_QUEUE_SIZE;
    static const std::string DEFAULT_BOUNDARY_MARKER;
    static const Poco::Net::MediaType DEFAULT_MEDIA_TYPE;
    static const std::size_t DEFAULT_MAX_STREAM_WIDTH;
    static const std::size_t DEFAULT_MAX_STREAM_HEIGHT;

private:
    std::size_t _maxClientConnections;
    std::size_t _maxClientBitRate;
    std::size_t _maxClientFrameRate;
    std::size_t _maxClientQueueSize;
    std::size_t _maxStreamWidth;
    std::size_t _maxStreamHeight;

    std::string _boundaryMarker;
    Poco::Net::MediaType _mediaType;

};


} } // namespace ofx::HTTP