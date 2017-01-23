//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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


/// \brief A wrapper for a FIFO IPVideoFrame queue.
///
/// The maximum number of frames that can be queued is noted by the \p maxSize.
class IPVideoFrameQueue
{
public:
    /// \brief Create an IPVideoFrameQueue with a given \p maxSize.
    /// \param maxSize The maximum size of the frame queue.
    IPVideoFrameQueue(std::size_t maxSize);

    /// \brief Destroy the IPVideoFrameQueue.
    virtual ~IPVideoFrameQueue();

    /// \returns the oldest frame in the queue and removes it.
    std::shared_ptr<IPVideoFrame> pop();

    /// \brief Push a frame onto the queue.
    /// \param frame The frame to push.
    void push(std::shared_ptr<IPVideoFrame> frame);

    /// \returns the maximum size of the frame queue.
    std::size_t getMaxSize() const;

    /// \brief Set the maximum size of the queue.
    /// \param maxSize The maximum size of the frame queue.
    void setMaxSize(std::size_t maxSize);

    /// \returns the number of frames in the frame queue.
    std::size_t size() const;

    /// \returns true iff the frame queue is empty.
    bool empty() const;

    /// \brief Clear all frames from the frame queue.
    void clear();

private:
    /// \brief The queue of IPVideoFrames to send.
    std::deque<std::shared_ptr<IPVideoFrame>> _frames;

    /// \brief The maximum size of the frame queue.
    std::size_t _maxSize;

    /// \brief THe mutex to protect multi-threaded data access.
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

    void setFrameSettings(const IPVideoFrameSettings& frameSettings);
    IPVideoFrameSettings getFrameSettings() const;

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
    IPVideoFrameSettings _frameSettings;

    std::size_t _maxClientConnections;
    std::size_t _maxClientBitRate;
    std::size_t _maxClientFrameRate;
    std::size_t _maxClientQueueSize;
    std::size_t _maxStreamWidth;
    std::size_t _maxStreamHeight;
    
    std::string _boundaryMarker;
    Poco::Net::MediaType _mediaType;
    
};


class IPVideoConnection;


class IPVideoRoute: public BaseRoute_<IPVideoRouteSettings>
{
public:
    typedef IPVideoRouteSettings Settings;

    IPVideoRoute(const Settings& settings);
    
    virtual ~IPVideoRoute();

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

    void send(const ofPixels& pix) const;

    std::size_t numConnections() const;

    virtual void stop() override;

protected:
    void addConnection(IPVideoConnection* handler);

    void removeConnection(IPVideoConnection* handler);
    
    typedef std::vector<IPVideoConnection*> Connections;

    Connections _connections;

    mutable std::mutex _mutex;

    friend class IPVideoConnection;

};


/// \brief The IPVideo route handler.
class IPVideoConnection:
    public BaseRouteHandler_<IPVideoRoute>,
    public IPVideoFrameQueue
{
public:
    /// \brief Create an IPVideoConnection.
    /// \param route The parent route.
    IPVideoConnection(IPVideoRoute& route);

    /// \brief Destroy the IPVideoConnection.
    virtual ~IPVideoConnection();

    void handleRequest(ServerEventArgs& evt) override;

    void stop() override;

    /// \brief Get the current bit rate in bits / second.
    /// \return the current bit rate in bits / second.
    float currentBitRate() const;

    /// \brief Get the current frame rate in frames / second.
    /// \returns the current frame rate in frames / second.
    float currentFrameRate() const;

    /// \brief Get the current frame settings.
    /// \returns the frame settings.
    IPVideoFrameSettings frameSettings() const;

protected:
    /// \brief The frame settings for this handler.
    IPVideoFrameSettings _frameSettings;

    /// \brief True if the handler is running.
    bool _isRunning = true;

    /// \brief The start time fo the connection.
    uint64_t _startTime = 0;

    /// \brief The number of bytes sent to the client from this connection.
    uint64_t _bytesSent = 0;

    /// \brief The number of frames sent to the client from this connection.
    uint64_t _framesSent = 0;

    /// \brief The time of the last frame sent.
    uint64_t _lastFrameSent = 0;

    /// \brief The amount of time required for the last frame.
    uint64_t _lastFrameDuration = 0;

    /// \brief The target frame duration.
    uint64_t _targetFrameDuration = 0;

    /// \brief The time the next frame should be sent.
    uint64_t _nextScheduledFrame = 0;

    /// \brief The mutex for protecting data.
    mutable std::mutex _mutex;

};


} } // namespace ofx::HTTP
