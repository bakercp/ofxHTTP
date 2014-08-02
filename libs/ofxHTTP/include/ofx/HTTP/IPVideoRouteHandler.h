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

#if defined(TARGET_WIN32)
    // #define clash with std::min
    // http://stackoverflow.com/questions/5004858/stdmin-gives-error
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
#endif


#include <algorithm>
#include "Poco/CountingStream.h"
#include "Poco/Exception.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Thread.h"
#include "ofx/HTTP/IPVideoRouteSettings.h"
#include "ofx/HTTP/IPVideoFrame.h"
#include "ofx/HTTP/IPVideoFrameSettings.h"
#include "ofx/HTTP/IPVideoFrameQueue.h"
#include "ofx/HTTP/Utils.h"


namespace ofx {
namespace HTTP {


class IPVideoRoute;


class IPVideoRouteHandler: public AbstractRouteHandler, public IPVideoFrameQueue
{
public:
    IPVideoRouteHandler(IPVideoRoute& parent);
    virtual ~IPVideoRouteHandler();
    
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response);

    virtual void stop();

    float getCurrentBitRate() const;   // bits / second
    float getCurrentFrameRate() const; // frames / second

    IPVideoFrameSettings getFrameSettings() const;

protected:
    IPVideoFrameSettings _frameSettings;
    IPVideoRoute& _parent;
    bool _isRunning;

    unsigned long long _startTime;

    unsigned long long _bytesSent;
    unsigned long long _framesSent;

    unsigned long long _lastFrameSent;
    unsigned long long _lastFrameDuration;
    unsigned long long _targetFrameDuration;

    unsigned long long _nextScheduledFrame;

    mutable ofMutex _mutex;

};


} } // namespace ofx::HTTP
