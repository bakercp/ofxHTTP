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


#include "IPVideoRoute.h"


namespace ofx {
namespace HTTP {


IPVideoRoute::IPVideoRoute(const Settings& settings):
    _settings(settings)
{
}

IPVideoRoute::~IPVideoRoute()
{
}

std::string IPVideoRoute::getRoutePathPattern() const
{
    return _settings.getRoutePathPattern();
}

bool IPVideoRoute::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                    bool isSecurePort) const
{
    return request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
           BaseRoute::canHandleRequest(request, isSecurePort);
}

Poco::Net::HTTPRequestHandler* IPVideoRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new IPVideoRouteHandler(*this);
}

void IPVideoRoute::send(ofPixels& pix)
{
    if(pix.isAllocated())
    {
        unsigned long long timestamp = ofGetElapsedTimeMillis();

        ofPixels pixels(pix); // copy the pixels (const!)
        ofBuffer compressedPixels;

        ofSaveImage(pixels, compressedPixels, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);

        ofScopedLock lock(_mutex);

        ConnectionsIter iter = _connections.begin();

        IPVideoFrameSettings settings;
//
//        settings.quality = settings.quality;

        IPVideoFrame::SharedPtr frame = IPVideoFrame::makeShared(settings,timestamp,compressedPixels);

        while(iter != _connections.end())
        {
            if(0 != (*iter))
            {
                (*iter)->push(frame);
            }
            else
            {
                ofLogError("IPVideoRoute::send") << "Found a NULL IPVideoRouteHandler*.  This should not happen.";
            }
            ++iter;
        }
            
    } else {
        ofLogError("ofxIpVideoServerRoute::pushFrame") << "Pushing unallocated pixels.";
    }
}

IPVideoRouteSettings IPVideoRoute::getSettings() const
{
    return _settings;
}

void IPVideoRoute::addConnection(IPVideoRouteHandler* handler)
{
    ofScopedLock lock(_mutex);
    _connections.push_back(handler);
}

void IPVideoRoute::removeConnection(IPVideoRouteHandler* handler)
{
    ofScopedLock lock(_mutex);
    _connections.erase(std::remove(_connections.begin(), _connections.end(), handler), _connections.end());
}

std::size_t IPVideoRoute::getNumConnections() const
{
    ofScopedLock lock(_mutex);
    return _connections.size();
}

void IPVideoRoute::stop()
{
    if(!_connections.empty())
    {
        for(std::size_t i = _connections.size() - 1; i > 0; --i)
        {
            if(0 != _connections[i])
            {
                _connections[i]->stop();
            }
        }
    }


}


} } // namespace ofx::HTTP
