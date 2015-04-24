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


#pragma once


#include <algorithm>
#include "ofImage.h"
#include "ofx/HTTP/BaseRoute.h"
#include "ofx/HTTP/IPVideoRouteHandler.h"
#include "ofx/HTTP/IPVideoRouteSettings.h"
#include "ofx/HTTP/IPVideoFrameQueue.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


class IPVideoRoute: public BaseRoute_<IPVideoRouteSettings>
{
public:
    typedef IPVideoRouteSettings Settings;

    IPVideoRoute(const Settings& settings);
    
    virtual ~IPVideoRoute();

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    void send(ofPixels& pix) const;

    void addConnection(IPVideoRouteHandler* handler);

    void removeConnection(IPVideoRouteHandler* handler);

    std::size_t getNumConnections() const;

    virtual void stop();

protected:
    typedef std::vector<IPVideoRouteHandler*> Connections;

    Connections _connections;

    mutable Poco::FastMutex _mutex;

};


} } // namespace ofx::HTTP
