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
#include "IPVideoRouteHandler.h"
#include "IPVideoRouteInterface.h"
#include "IPVideoRouteSettings.h"
#include "IPVideoFrameQueue.h"
#include "Utils.h"


namespace ofx {
namespace HTTP {


class IPVideoRoute: public IPVideoRouteInterface
{
public:
    typedef std::shared_ptr<IPVideoRoute> SharedPtr;
    typedef std::weak_ptr<IPVideoRoute>   WeakPtr;
    typedef IPVideoRouteSettings Settings;

    IPVideoRoute(const Settings& settings);
    virtual ~IPVideoRoute();

    virtual std::string getRoutePathPattern() const;
    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const;

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    void send(ofPixels& pix);

    IPVideoRouteSettings getSettings() const;

    void addConnection(IPVideoRouteHandler* handler);
    void removeConnection(IPVideoRouteHandler* handler);

    std::size_t getNumConnections() const;

    virtual void stop();

    static SharedPtr makeShared(const Settings& settings)
    {
        return SharedPtr(new IPVideoRoute(settings));
    }

protected:
    typedef std::vector<IPVideoRouteHandler*>           Connections;
    typedef std::vector<IPVideoRouteHandler*>::iterator ConnectionsIter;

    Connections _connections;

    Settings _settings;

    mutable ofMutex _mutex;

};


} } // namespace ofx::HTTP
