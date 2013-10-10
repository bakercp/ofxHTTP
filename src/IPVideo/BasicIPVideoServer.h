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


#include "BasicServer.h"
#include "IPVideoRoute.h"
#include "IPVideoRouteSettings.h"


namespace ofx {
namespace HTTP {


class BasicIPVideoServerSettings:
    public IPVideoRouteSettings,
    public BasicServerSettings
{
};


class BasicIPVideoServer: public BasicServer
{
public:
    typedef std::shared_ptr<BasicIPVideoServer> SharedPtr;
    typedef std::weak_ptr<BasicIPVideoServer>   WeakPtr;
    typedef BasicIPVideoServerSettings Settings;

    BasicIPVideoServer(const Settings& settings = Settings());
    virtual ~BasicIPVideoServer();


    void send(ofPixels& pix);


    std::size_t getNumConnections() const
    {
        return _ipVideoRoute->getNumConnections();
    }

    static SharedPtr makeShared(const Settings& settings = Settings())
    {
        return SharedPtr(new BasicIPVideoServer(settings));
    }

protected:
    IPVideoRoute::SharedPtr _ipVideoRoute;

};


} } // namespace ofx::HTTP
