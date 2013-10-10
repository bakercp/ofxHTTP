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

#include <queue>

//#include "Poco/Condition.h"

//#include "ofTypes.h"

#include "IPVideoFrame.h"
#include "IPVideoFrameSettings.h"


namespace ofx {
namespace HTTP {


class IPVideoFrameQueue
{
public:
    IPVideoFrameQueue(std::size_t maxSize);
    virtual ~IPVideoFrameQueue();

    IPVideoFrame::SharedPtr pop();
    
    void push(IPVideoFrame::SharedPtr frame);

    std::size_t getMaxSize() const;
    void setMaxSize(std::size_t maxSize);
    std::size_t size() const;
    bool empty() const;

private:
    typedef std::deque<IPVideoFrame::SharedPtr> ClientFrameQueue;

    ClientFrameQueue _frames;
    std::size_t _maxSize;

    mutable ofMutex _mutex;
    
};


} } // namespace ofx::HTTP
