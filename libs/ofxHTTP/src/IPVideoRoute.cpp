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


#include "ofx/HTTP/IPVideoRoute.h"


namespace ofx {
namespace HTTP {


IPVideoRoute::IPVideoRoute(const Settings& settings):
    BaseRoute_<IPVideoRouteSettings>(settings)
{
}


IPVideoRoute::~IPVideoRoute()
{
}


Poco::Net::HTTPRequestHandler* IPVideoRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new IPVideoRouteHandler(*this);
}


void IPVideoRoute::send(ofPixels& pix) const
{
    if (pix.isAllocated())
    {
        unsigned long long timestamp = ofGetElapsedTimeMillis();

        ofBuffer compressedPixels;

        // TODO: turbo jpeg an option here?
        ofSaveImage(pix, compressedPixels, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);

        Poco::FastMutex::ScopedLock lock(_mutex);

        Connections::const_iterator iter = _connections.begin();

        IPVideoFrameSettings settings;
//
//        settings.quality = settings.quality;

        std::shared_ptr<IPVideoFrame> frame = std::shared_ptr<IPVideoFrame>(new IPVideoFrame(settings, timestamp, compressedPixels));

        while (iter != _connections.end())
        {
            if(*iter)
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


void IPVideoRoute::addConnection(IPVideoRouteHandler* handler)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _connections.push_back(handler);
}


void IPVideoRoute::removeConnection(IPVideoRouteHandler* handler)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _connections.erase(std::remove(_connections.begin(), _connections.end(), handler), _connections.end());
}


std::size_t IPVideoRoute::getNumConnections() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _connections.size();
}


void IPVideoRoute::stop()
{
    if (!_connections.empty())
    {
        Connections::reverse_iterator iter = _connections.rbegin();

        while (iter != _connections.rend())
        {
            if (*iter)
            {
                (*iter)->stop();
            }

            ++iter;
        }
    }
}


} } // namespace ofx::HTTP
