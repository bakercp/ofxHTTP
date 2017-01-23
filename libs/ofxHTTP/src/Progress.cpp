//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
