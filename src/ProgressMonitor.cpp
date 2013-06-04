#include "ProgressMonitor.h"


namespace ofx {
namespace HTTP {

//------------------------------------------------------------------------------
//streamsize totalBytesTransferred;
//streamsize totalBytes;
//size_t transferBufferSize;


ProgressMonitor::ProgressMonitor()
: totalBytesTransferred(-1)
, totalBytes(-1)
, transferBufferSize(0)
, lastUpdateTime(0)
, bytesPerSecond(0)
{
    
}

//------------------------------------------------------------------------------
ProgressMonitor::~ProgressMonitor() { }

//------------------------------------------------------------------------------
void ProgressMonitor::update(std::streamsize _totalBytesTransferred,
                             std::streamsize _totalBytes,
                             size_t _transferBufferSize,
                             unsigned long long _lastUpdate)
{
    
//    ofScopedLock
    
}

//------------------------------------------------------------------------------
std::streamsize ProgressMonitor::getTotalBytesTranferred() {
    return 0;
}

//------------------------------------------------------------------------------
std::streamsize ProgressMonitor::getTotalBytes() {
    return 0;
}

//------------------------------------------------------------------------------
float ProgressMonitor::getPercentageTransferred() {
    return 0;
}

//------------------------------------------------------------------------------
size_t ProgressMonitor::getTransferBufferSize() {
    return 0;
}

//------------------------------------------------------------------------------
void ProgressMonitor::reset() {
    
}


} }
