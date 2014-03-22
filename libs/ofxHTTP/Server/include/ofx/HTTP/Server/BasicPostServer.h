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


#include "ofx/HTTP/Server/BasicServer.h"
#include "ofx/HTTP/Server/PostRoute.h"
#include "ofx/HTTP/Server/PostRouteSettings.h"
#include "ofx/HTTP/Server/PostRouteEvents.h"


namespace ofx {
namespace HTTP {


class BasicPostServerSettings:
    public PostRouteSettings,
    public BasicServerSettings
{
};


class BasicPostServer: public BasicServer
{
public:
    typedef std::shared_ptr<BasicPostServer> SharedPtr;
    typedef std::weak_ptr<BasicPostServer> WeakPtr;
    typedef BasicPostServerSettings Settings;

    BasicPostServer(const Settings& settings = Settings());
    virtual ~BasicPostServer();

    PostRoute::SharedPtr getPostRoute();

    /// \brief this method is a hack replacement for
    /// std::make_shared<BasicServer>(...);
    static SharedPtr makeShared(const Settings& settings = Settings())
    {
        return SharedPtr(new BasicPostServer(settings));
    }

protected:
    PostRoute::SharedPtr _postRoute;
    
};


} } // namespace ofx::HTTP
