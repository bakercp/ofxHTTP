// =============================================================================
//
// Copyright (c) 2012-2016 Christopher Baker <http://christopherbaker.net>
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


#if defined(TARGET_WIN32)
// #define clash with std::min
// http://stackoverflow.com/questions/5004858/stdmin-gives-error
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif


#include <algorithm>
#include "ofImage.h"
#include "ofx/HTTP/BaseRoute.h"
#include "ofx/HTTP/HTTPUtils.h"
#include "ofx/HTTP/IPVideoFrame.h"


namespace ofx {
namespace HTTP {


class IPVideoFrameQueue
{
public:
    IPVideoFrameQueue(std::size_t maxSize);

    virtual ~IPVideoFrameQueue();

    std::shared_ptr<IPVideoFrame> pop();

    void push(std::shared_ptr<IPVideoFrame> frame);

    std::size_t getMaxSize() const;

    void setMaxSize(std::size_t maxSize);

    std::size_t size() const;

    bool empty() const;

    void clear();

private:
    std::deque<std::shared_ptr<IPVideoFrame>> _frames;
    
    std::size_t _maxSize;
    
    mutable std::mutex _mutex;
    
};


class IPVideoRouteSettings: public BaseRouteSettings
{
public:
    /// \brief Create the IPVideoRouteSettings with the given route path.
    /// \param routePathPattern The regex pattern that this route will handle.
    /// \param requireSecurePort True if this route requires
    ///        communication on an SSL encrypted port.
    IPVideoRouteSettings(const std::string& routePathPattern = DEFAULT_VIDEO_ROUTE,
                         bool requireSecurePort = false);

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

    enum
    {
        DEFAULT_MAX_CLIENT_CONNECTIONS = 5,
        DEFAULT_MAX_CLIENT_BITRATE     = 1024,
        DEFAULT_MAX_CLIENT_FRAMERATE   = 30,
        DEFAULT_MAX_CLIENT_QUEUE_SIZE  = 10,
    };

    enum
    {
        DEFAULT_MAX_STREAM_WIDTH  = 1920,
        DEFAULT_MAX_STREAM_HEIGHT = 1080,
    };

    static const std::string DEFAULT_VIDEO_ROUTE;
    static const std::string DEFAULT_BOUNDARY_MARKER;
    static const Poco::Net::MediaType DEFAULT_MEDIA_TYPE;

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


class IPVideoRouteHandler;


class IPVideoRoute: public BaseRoute_<IPVideoRouteSettings>
{
public:
    typedef IPVideoRouteSettings Settings;

    IPVideoRoute(const Settings& settings);
    
    virtual ~IPVideoRoute();

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

    void send(ofPixels& pix) const;

    void addConnection(IPVideoRouteHandler* handler);

    void removeConnection(IPVideoRouteHandler* handler);

    std::size_t getNumConnections() const;

    virtual void stop() override;

protected:
    typedef std::vector<IPVideoRouteHandler*> Connections;

    Connections _connections;

    mutable std::mutex _mutex;

};


class IPVideoRouteHandler:
    public BaseRouteHandler_<IPVideoRoute>,
    public IPVideoFrameQueue
{
public:
    IPVideoRouteHandler(IPVideoRoute& route);
    virtual ~IPVideoRouteHandler();

    void handleRequest(ServerEventArgs& evt) override;

    void stop() override;

    float getCurrentBitRate() const;   // bits / second
    float getCurrentFrameRate() const; // frames / second

    IPVideoFrameSettings getFrameSettings() const;

protected:
    IPVideoFrameSettings _frameSettings;

    bool _isRunning;

    uint64_t _startTime;

    uint64_t _bytesSent;
    uint64_t _framesSent;

    uint64_t _lastFrameSent;
    uint64_t _lastFrameDuration;
    uint64_t _targetFrameDuration;
    
    uint64_t _nextScheduledFrame;
    
    mutable std::mutex _mutex;

};


} } // namespace ofx::HTTP
