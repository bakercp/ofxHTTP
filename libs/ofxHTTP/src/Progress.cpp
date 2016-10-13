// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/Progress.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Timestamp.h"


namespace ofx {
namespace HTTP {

    
const int64_t Progress::UNKNOWN_CONTENT_LENGTH = Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH;


Progress::Progress(int64_t totalBytes): _totalBytes(totalBytes)
{
}


Progress::Progress()
{
}


Progress::~Progress()
{
}


int64_t Progress::getTotalBytesTranferred() const
{
    return _totalBytesTransferred;
}


void Progress::setTotalBytesTransferred(int64_t totalBytesTransferred)
{
    _lastUpdateTime.update();
    _totalBytesTransferred = totalBytesTransferred;
}



int64_t Progress::getTotalBytes() const
{
    return _totalBytes;
}


void Progress::setTotalBytes(int64_t totalBytes)
{
    _startTime.update();
    _totalBytes = totalBytes;
}


float Progress::progress() const
{
    if (_totalBytes == 0)
    {
        return 1.0;
    }
    else if (_totalBytes > 0)
    {
        double totalBytesTransferred = static_cast<double>(_totalBytesTransferred);
        double totalBytes = static_cast<double>(_totalBytes);
        return static_cast<float>(totalBytesTransferred / totalBytes);
    }
    else return UNKNOWN_CONTENT_LENGTH;
}


float Progress::bytesPerSecond() const
{
    return _totalBytesTransferred / (_startTime.elapsed() / 1000000.0);
}


Poco::Timestamp Progress::lastUpdateTime() const
{
    return _lastUpdateTime;
}


Poco::Timestamp Progress::startTime() const
{
    return _startTime;
}


} } // namespace ofx::HTTP
