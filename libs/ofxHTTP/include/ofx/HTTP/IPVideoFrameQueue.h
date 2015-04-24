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
#include "ofx/HTTP/IPVideoFrame.h"
#include "ofx/HTTP/IPVideoFrameSettings.h"


namespace ofx {
namespace HTTP {


class IPVideoFrameQueue
{
public:
    IPVideoFrameQueue(std::size_t maxSize);

    virtual ~IPVideoFrameQueue();

    std::shared_ptr<IPVideoFrame> pop();
    
    void push(std::shared_ptr<IPVideoFrame> frame);

    std::size_t getMaxSize() const;

    void setMaxSize(std::size_t maxSize);

    std::size_t size() const;

    bool empty() const;

    void clear();

private:
    std::deque<std::shared_ptr<IPVideoFrame> > _frames;

    std::size_t _maxSize;

    mutable Poco::FastMutex _mutex;
    
};


} } // namespace ofx::HTTP
