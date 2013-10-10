// =============================================================================
//
// Copyright (c) 2012-2013 Christopher Baker <http://christopherbaker.net>
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


#include "IPVideoRouteHandler.h"


namespace ofx {
namespace HTTP {


IPVideoRouteHandler::IPVideoRouteHandler(IPVideoRouteInterface& parent):
    BaseRouteHandler(parent),
    IPVideoFrameQueue(parent.getSettings().getMaxClientQueueSize()),
    _parent(parent),
    _isRunning(true),
    _startTime(0),
    _bytesSent(0),
    _framesSent(0),
    _lastFrameSent(0),
    _lastFrameDuration(0),
    _targetFrameDuration(0),
    _nextScheduledFrame(0)
{
}

IPVideoRouteHandler::~IPVideoRouteHandler()
{
}

void IPVideoRouteHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                        Poco::Net::HTTPServerResponse& response)
{
    if(_parent.getSettings().getMaxClientConnections() != 0 && // 0 == no limit
       _parent.getNumConnections() >= _parent.getSettings().getMaxClientConnections())
    {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_SERVICE_UNAVAILABLE,
                                    "Maximum client connections exceeded.  Please try again later.");
        _parent.handleRequest(request,response);
        return;
    }

    Poco::URI uri;

    try
    {
        uri = Poco::URI(request.getURI());
    }
    catch (const Poco::SyntaxException& exc)
    {
        ofLogError("IPVideoRoute::createRequestHandler") << exc.what();
    }

    std::string query = uri.getQuery();

    Poco::Net::NameValueCollection queryMap = Utils::getQueryMap(uri);

    if(queryMap.has("vflip"))
    {
        std::string vflip = queryMap.get("vflip");
        _frameSettings.setFlipVertical(Poco::icompare(vflip,"1")    == 0 ||
                                       Poco::icompare(vflip,"true") == 0 ||
                                       Poco::icompare(vflip,"t")    == 0 ||
                                       Poco::icompare(vflip,"y")    == 0 ||
                                       Poco::icompare(vflip,"yes")  == 0);
    }

    if(queryMap.has("hflip"))
    {
        std::string hflip = queryMap.get("hflip");
        _frameSettings.setFlipHorizontal(Poco::icompare(hflip,"1")    == 0 ||
                                         Poco::icompare(hflip,"true") == 0 ||
                                         Poco::icompare(hflip,"t")    == 0 ||
                                         Poco::icompare(hflip,"y")    == 0 ||
                                         Poco::icompare(hflip,"yes")  == 0);
    }

    if(queryMap.has("size"))
    {
        std::string size = queryMap.get("size");
        Poco::toLowerInPlace(size);
        std::vector<std::string> tokens = ofSplitString(size,"x");

        if(tokens.size() == 2)
        {
            std::size_t width = 0;
            std::istringstream token0(tokens[0]);
            token0 >> width;

            std::size_t height = 0;
            std::istringstream token1(tokens[1]);
            token1 >> height;

            if(width > 0 && height > 0)
            {
                width = std::min(width,_parent.getSettings().getMaxStreamWidth());
                height = std::min(height,_parent.getSettings().getMaxStreamHeight());
                _frameSettings.setWidth(width);
                _frameSettings.setHeight(height);
            }
        }
    }

    if(queryMap.has("quality"))
    {
        std::string quality = queryMap.get("quality");
        if(Poco::icompare(quality,"best"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_BEST);
        }
        else if(Poco::icompare(quality,"high"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_HIGH);
        }
        else if(Poco::icompare(quality,"medium"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_MEDIUM);
        }
        else if(Poco::icompare(quality,"low"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_LOW);
        }
        else if(Poco::icompare(quality,"worst"))
        {
            _frameSettings.setQuality(OF_IMAGE_QUALITY_WORST);
        }
        else
        {
            // no change
        }
    }

    _parent.addConnection(this);

    try
    {
        Poco::Net::MediaType mediaType = _parent.getSettings().getMediaType();
        mediaType.setParameter("boundary", _parent.getSettings().getBoundaryMarker());
        response.set("Cache-control","no-cache, no-store, must-revalidate");
        response.set("Pragma","no-cache");
        response.set("Server","ofx::HTTP::IPVideoServer");
        response.setContentType(mediaType);
        response.set("Expires",Poco::DateTimeFormatter::format(Poco::Timestamp(0),
                                                               Poco::DateTimeFormat::HTTP_FORMAT));

        std::ostream& outputStream = response.send();

        Poco::CountingOutputStream ostr(outputStream);

        while(_isRunning)
        {
            if(outputStream.good() && !outputStream.fail() && !outputStream.bad())
            {
                if(!empty())
                {
                    IPVideoFrame::SharedPtr frame = pop();

                    if(0 != frame)
                    {
                        ofBuffer& buffer = frame->getBufferRef();

                        ostr << _parent.getSettings().getBoundaryMarker();
                        ostr << "\r\n";
                        ostr << "Content-Type: image/jpeg";
                        ostr << "\r\n";
                        ostr << "Content-Length: " << buffer.size();
                        ostr << "\r\n";
                        ostr << "\r\n";
                        ostr << buffer;

                        unsigned long long now = ofGetElapsedTimeMillis();
                        _lastFrameDuration = now - _lastFrameSent;
                        _lastFrameSent = now;
                        _bytesSent += ostr.chars(); // add the counts
                        ostr.reset();               // reset the counts
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
    catch(Poco::Exception& e)
    {
//        ofLogVerbose("IPVideoRouteHandler::handleRequest") << "Exception: " << e.displayText();
    }

    _parent.removeConnection(this);

}

void IPVideoRouteHandler::stop()
{
    _isRunning = false;
}

IPVideoFrameSettings IPVideoRouteHandler::getFrameSettings() const
{
    return _frameSettings;
}

float IPVideoRouteHandler::getCurrentBitRate() const
{
    ofScopedLock lock(_mutex);
    return (float)_bytesSent * 8.0f / (ofGetElapsedTimeMillis() - _startTime);
}

float IPVideoRouteHandler::getCurrentFrameRate() const
{
    ofScopedLock lock(_mutex);
    return (float)_framesSent / (ofGetElapsedTimeMillis() - _startTime);
}


} } // namespace ofx::HTTP
