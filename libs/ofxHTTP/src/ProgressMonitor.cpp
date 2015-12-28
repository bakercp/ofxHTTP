// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/ProgressMonitor.h"


namespace ofx {
namespace HTTP {


ProgressMonitor::ProgressMonitor() :
    _totalBytesTransferred(-1),
    _totalBytes(-1),
    _transferBufferSize(0),
    _lastUpdateTime(0),
    _bytesPerSecond(0)
{
}


ProgressMonitor::~ProgressMonitor()
{
}


void ProgressMonitor::update(std::streamsize,
                             std::streamsize,
                             std::size_t,
                             uint64_t)
{
//    _totalBytesTransferred  = totalBytesTransferred;
//    _totalBytes             = totalBytes;
//    _transferBufferSize     = transferBufferSize;
//    _lastUpdateTime         = lastUpdateTime;
//    _bytesPerSecond         = bytesPerSecond;
}


std::streamsize ProgressMonitor::getTotalBytesTranferred() const
{
    return 0;
}


std::streamsize ProgressMonitor::getTotalBytes() const
{
    return 0;
}


float ProgressMonitor::getPercentageTransferred() const
{
    return 0;
}


std::size_t ProgressMonitor::getTransferBufferSize() const
{
    return 0;
}


void ProgressMonitor::reset()
{
    _totalBytesTransferred = -1;
    _totalBytes            = -1;
    _transferBufferSize    = 0;
    _lastUpdateTime        = 0;
    _bytesPerSecond        = 0;
}


} } // namespace ofx::HTTP
