//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/IPVideoRoute.h"
#include "Poco/CountingStream.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "ofImage.h"


#undef min // for windows
#undef max // for windows


namespace ofx {
namespace HTTP {


IPVideoFrameQueue::IPVideoFrameQueue(std::size_t maxSize):
    _maxSize(maxSize)
{
}


IPVideoFrameQueue::~IPVideoFrameQueue()
{
}


std::shared_ptr<IPVideoFrame> IPVideoFrameQueue::pop()
{
    std::shared_ptr<IPVideoFrame> frame;

    std::unique_lock<std::mutex> lock(_mutex);

    if (!_frames.empty())
    {
        frame = _frames.front();
        _frames.pop_front();
        return frame;
    }
    else
    {
        return frame;
    }
}


void IPVideoFrameQueue::push(std::shared_ptr<IPVideoFrame> frame)
{
    std::unique_lock<std::mutex> lock(_mutex);

    _frames.push_back(frame);

    while (_frames.size() > _maxSize)
    {
        _frames.pop_front();
    }
}


std::size_t IPVideoFrameQueue::getMaxSize() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _maxSize;
}


void IPVideoFrameQueue::setMaxSize(std::size_t maxSize)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _maxSize = maxSize;

    while (_frames.size() > _maxSize)
    {
        _frames.pop_front();
    }
}


std::size_t IPVideoFrameQueue::size() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _frames.size();
}


bool IPVideoFrameQueue::empty() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _frames.empty();
}


void IPVideoFrameQueue::clear()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _frames.clear();
}


const std::string IPVideoRouteSettings::DEFAULT_VIDEO_ROUTE = "/ipvideo";
const std::string IPVideoRouteSettings::DEFAULT_BOUNDARY_MARKER = "--boundary";
const Poco::Net::MediaType IPVideoRouteSettings::DEFAULT_MEDIA_TYPE = Poco::Net::MediaType("multipart/x-mixed-replace");


IPVideoRouteSettings::IPVideoRouteSettings(const std::string& routePathPattern,
                                           bool requireSecurePort):
    BaseRouteSettings(routePathPattern, requireSecurePort),
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


void IPVideoRouteSettings::setFrameSettings(const IPVideoFrameSettings& frameSettings)
{
    _frameSettings = frameSettings;
}


IPVideoFrameSettings IPVideoRouteSettings::getFrameSettings() const
{
    return _frameSettings;
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


IPVideoRoute::IPVideoRoute(const Settings& settings):
    BaseRoute_<IPVideoRouteSettings>(settings)
{
}


IPVideoRoute::~IPVideoRoute()
{
}


Poco::Net::HTTPRequestHandler* IPVideoRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new IPVideoConnection(*this);
}


void IPVideoRoute::send(const ofPixels& pix) const
{
    if (pix.isAllocated())
    {
        uint64_t timestamp = ofGetElapsedTimeMillis();

        ofBuffer compressedPixels;

        IPVideoFrameSettings frameSettings = _settings.getFrameSettings();

        if (frameSettings.getWidth() != IPVideoFrameSettings::NO_RESIZE
            ||  frameSettings.getHeight() != IPVideoFrameSettings::NO_RESIZE
            ||  frameSettings.getFlipHorizontal()
            ||  frameSettings.getFlipVertical())
        {
            int newWidth = frameSettings.getWidth() != IPVideoFrameSettings::NO_RESIZE ? frameSettings.getWidth() : pix.getWidth();
            int newHeight = frameSettings.getHeight() != IPVideoFrameSettings::NO_RESIZE ? frameSettings.getHeight() : pix.getHeight();

            ofPixels pixels = pix;

            if (newWidth != pix.getWidth() || newHeight != pix.getHeight())
            {
                pixels.resize(newWidth, newHeight);
            }

            if (frameSettings.getFlipVertical() || frameSettings.getFlipHorizontal())
            {
                pixels.mirror(frameSettings.getFlipVertical(),
                              frameSettings.getFlipHorizontal());
            }

            ofSaveImage(pixels, compressedPixels, OF_IMAGE_FORMAT_JPEG, frameSettings.getQuality());
        }
        else
        {
            ofPixels pixels = pix;
            ofSaveImage(pixels, compressedPixels, OF_IMAGE_FORMAT_JPEG, frameSettings.getQuality());
        }

        std::unique_lock<std::mutex> lock(_mutex);

        Connections::const_iterator iter = _connections.begin();

        std::shared_ptr<IPVideoFrame> frame = std::make_shared<IPVideoFrame>(frameSettings, timestamp, compressedPixels);

        while (iter != _connections.end())
        {
            if (*iter != nullptr)
            {
                (*iter)->push(frame);
            }
            else
            {
                ofLogError("IPVideoRoute::send") << "Found a NULL IPVideoRouteHandler*.  This should not happen.";
            }

            ++iter;
        }
    }
    else
    {
        ofLogError("IPVideoRoute::pushFrame") << "Pushing unallocated pixels.";
    }
}


std::size_t IPVideoRoute::numConnections() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _connections.size();
}


void IPVideoRoute::stop()
{
    Connections::reverse_iterator iter = _connections.rbegin();

    while (iter != _connections.rend())
    {
        if (*iter != nullptr)
        {
            (*iter)->stop();
        }

        ++iter;
    }
}


void IPVideoRoute::addConnection(IPVideoConnection* handler)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _connections.push_back(handler);
}


void IPVideoRoute::removeConnection(IPVideoConnection* handler)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _connections.erase(std::remove(_connections.begin(), _connections.end(), handler), _connections.end());
}


IPVideoConnection::IPVideoConnection(IPVideoRoute& route):
    BaseRouteHandler_<IPVideoRoute>(route),
    IPVideoFrameQueue(route.settings().getMaxClientQueueSize())
{
}


IPVideoConnection::~IPVideoConnection()
{
}


void IPVideoConnection::handleRequest(ServerEventArgs& evt)
{
    if(route().settings().getMaxClientConnections() != 0 && // 0 == no limit
       route().numConnections() >= route().settings().getMaxClientConnections())
    {
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_SERVICE_UNAVAILABLE,
                                          "Maximum client connections exceeded. Please try again later.");

        route().handleRequest(evt);
        return;
    }

    Poco::URI uri;

    try
    {
        uri = Poco::URI(evt.request().getURI());
    }
    catch (const Poco::SyntaxException& exc)
    {
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR,
                                          "Request URI Invalid.");

        route().handleRequest(evt);
        return;
    }

    std::string query = uri.getQuery();


    Poco::Net::NameValueCollection queryMap = HTTPUtils::getQueryMap(uri);

    if (queryMap.has("vflip"))
    {
        std::string vflip = queryMap.get("vflip");
        _frameSettings.setFlipVertical(Poco::icompare(vflip, "1")    == 0 ||
                                       Poco::icompare(vflip, "true") == 0 ||
                                       Poco::icompare(vflip, "t")    == 0 ||
                                       Poco::icompare(vflip, "y")    == 0 ||
                                       Poco::icompare(vflip, "yes")  == 0);
    }

    if (queryMap.has("hflip"))
    {
        std::string hflip = queryMap.get("hflip");
        _frameSettings.setFlipHorizontal(Poco::icompare(hflip, "1")    == 0 ||
                                         Poco::icompare(hflip, "true") == 0 ||
                                         Poco::icompare(hflip, "t")    == 0 ||
                                         Poco::icompare(hflip, "y")    == 0 ||
                                         Poco::icompare(hflip, "yes")  == 0);
    }

    if (queryMap.has("size"))
    {
        std::string size = queryMap.get("size");
        Poco::toLowerInPlace(size);
        std::vector<std::string>  tokens = ofSplitString(size, "x");

        if (tokens.size() == 2)
        {
            std::size_t width = 0;
            std::istringstream token0(tokens[0]);
            token0 >> width;

            std::size_t height = 0;
            std::istringstream token1(tokens[1]);
            token1 >> height;

            if (width > 0 && height > 0)
            {
                width = std::min(width, route().settings().getMaxStreamWidth());
                height = std::min(height, route().settings().getMaxStreamHeight());
                _frameSettings.setWidth(width);
                _frameSettings.setHeight(height);
            }
        }
    }

    if (queryMap.has("quality"))
    {
        std::string quality = queryMap.get("quality");

        if (Poco::icompare(quality,"best"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_BEST);
        }
        else if (Poco::icompare(quality,"high"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_HIGH);
        }
        else if (Poco::icompare(quality,"medium"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_MEDIUM);
        }
        else if (Poco::icompare(quality,"low"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_LOW);
        }
        else if (Poco::icompare(quality,"worst"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_WORST);
        }
        else
        {
            // no change
        }
    }
    
    route().addConnection(this);
    
    try
    {
        Poco::Net::MediaType mediaType = route().settings().getMediaType();
        mediaType.setParameter("boundary", route().settings().getBoundaryMarker());
        evt.response().set("Cache-control", "no-cache, no-store, must-revalidate");
        evt.response().set("Pragma", "no-cache");
        evt.response().set("Server", "ofx::HTTP::IPVideoServer");
        evt.response().setContentType(mediaType);
        evt.response().set("Expires", Poco::DateTimeFormatter::format(Poco::Timestamp(0),
                                                                      Poco::DateTimeFormat::HTTP_FORMAT));
        
        std::ostream& outputStream = evt.response().send();
        
        Poco::CountingOutputStream ostr(outputStream);
        
        while (_isRunning)
        {
            if (outputStream.good() && !outputStream.fail() && !outputStream.bad())
            {
                if (!empty())
                {
                    std::shared_ptr<IPVideoFrame> frame = pop();
                    
                    if (frame != nullptr)
                    {
                        const ofBuffer& buffer = frame->buffer();

                        ostr << route().settings().getBoundaryMarker();
                        ostr << "\r\n";
                        ostr << "Content-Type: image/jpeg";
                        ostr << "\r\n";
                        ostr << "Content-Length: " << buffer.size();
                        ostr << "\r\n";
                        ostr << "\r\n";
                        ostr << buffer;
                        
                        uint64_t now = ofGetElapsedTimeMillis();
                        _lastFrameDuration = now - _lastFrameSent;
                        _lastFrameSent = now;
                        _bytesSent += static_cast<uint64_t>(ostr.chars()); // add the counts
                        ostr.reset();               // reset the counts
                        ostr.flush();
                    }
                    else
                    {
                        ofLogVerbose("IPVideoRouteHandler::handleRequest") << "Null buffer.";
                    }
                }
                else
                {
                    // ofLogVerbose("IPVideoRouteHandler::handleRequest") << "Queue empty.";
                }
            }
            else
            {
                throw Poco::Exception("Response stream failed or went bad -- it was probably interrupted.");
            }
            
            Poco::Thread::sleep(30);  // TODO: smarter ways of doing for rate / fps limiting
        }
    }
    catch (const Poco::Exception& e)
    {
        ofLogError("IPVideoRouteHandler::handleRequest") << "Exception: " << e.displayText();
    }
    
    route().removeConnection(this);
}


void IPVideoConnection::stop()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _isRunning = false;
}


IPVideoFrameSettings IPVideoConnection::frameSettings() const
{
    return _frameSettings;
}


float IPVideoConnection::currentBitRate() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return static_cast<float>(_bytesSent) * 8.0f / (ofGetElapsedTimeMillis() - _startTime);
}


float IPVideoConnection::currentFrameRate() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return static_cast<float>(_framesSent) / (ofGetElapsedTimeMillis() - _startTime);
}


} } // namespace ofx::HTTP
