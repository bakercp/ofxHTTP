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


#include "AbstractTypes.h"
#include "WebSocketRouteHandler.h"
#include "WebSocketRouteInterface.h"
#include "WebSocketEvents.h"


namespace ofx {
namespace HTTP {


class WebSocketRoute: public BaseWebSocketSessionManager, public WebSocketRouteInterface
{
public:
    typedef std::shared_ptr<WebSocketRoute> SharedPtr;
    typedef std::weak_ptr<WebSocketRoute>   WeakPtr;
    typedef WebSocketRouteSettings Settings;

    WebSocketRoute(const Settings& settings);
    virtual ~WebSocketRoute();

    virtual std::string getRoutePathPattern() const;

    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool isSecurePort) const;

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    virtual void stop();

    WebSocketRouteSettings getSettings() const;
    BaseWebSocketSessionManager& getSessionManagerRef();

    static SharedPtr makeShared(const Settings& settings)
    {
        return SharedPtr(new WebSocketRoute(settings));
    }

private:
    Settings _settings;
    
};


} } // namespace ofx::HTTP
