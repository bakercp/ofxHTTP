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


#include "IPVideoFrameQueue.h"


namespace ofx {
namespace HTTP {


IPVideoFrameQueue::IPVideoFrameQueue(std::size_t maxSize):
    _maxSize(maxSize)
{
}

IPVideoFrameQueue::~IPVideoFrameQueue()
{
}

IPVideoFrame::SharedPtr IPVideoFrameQueue::pop()
{
    IPVideoFrame::SharedPtr frame;

    ofScopedLock lock(_mutex);

    if(!_frames.empty())
    {
        frame = _frames.front();
        _frames.pop_front();
        return frame;
    }
    else
    {
        return frame;
    }
}

void IPVideoFrameQueue::push(IPVideoFrame::SharedPtr frame)
{
    ofScopedLock lock(_mutex);

    _frames.push_back(frame);

    while(_frames.size() > _maxSize)
    {
        _frames.pop_front();
    }
}

std::size_t IPVideoFrameQueue::getMaxSize() const
{
    ofScopedLock lock(_mutex);
    return _maxSize;
}

void IPVideoFrameQueue::setMaxSize(std::size_t maxSize)
{
    ofScopedLock lock(_mutex);
    _maxSize = maxSize;
    while(_frames.size() > _maxSize)
    {
        _frames.pop_front();
    }
}

std::size_t IPVideoFrameQueue::size() const
{
    ofScopedLock lock(_mutex);
    return _frames.size();
}

bool IPVideoFrameQueue::empty() const
{
    ofScopedLock lock(_mutex);
    return _frames.empty();
}

    
} } // namespace ofx::HTTP
