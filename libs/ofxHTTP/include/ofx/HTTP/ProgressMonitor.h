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


#pragma once


#include <ios>
#include "Poco/Mutex.h"


namespace ofx {
namespace HTTP {


class ProgressMonitor
{
public:
    ProgressMonitor();
    virtual ~ProgressMonitor();
    
    void update(std::streamsize totalBytesTransferred,
                std::streamsize totalBytes,
                std::size_t transferBufferSize,
                uint64_t lastUpdate);

    std::streamsize getTotalBytesTranferred() const;
    std::streamsize getTotalBytes() const;
    
    float getPercentageTransferred() const;
    
    std::size_t getTransferBufferSize() const;

    void reset();
    
private:
    std::streamsize _totalBytesTransferred;
    std::streamsize _totalBytes;
    std::size_t _transferBufferSize;

    uint64_t _lastUpdateTime;
    float _bytesPerSecond;

    mutable Poco::FastMutex _mutex;

};


} } // namespace ofx::HTTP
