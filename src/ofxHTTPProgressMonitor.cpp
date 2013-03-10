#include "ofxHTTPProgressMonitor.h"

//------------------------------------------------------------------------------
streamsize totalBytesTransferred;
streamsize totalBytes;
size_t transferBufferSize;


ofxHTTPProgressMonitor::ofxHTTPProgressMonitor() :
totalBytesTransferred(-1),
totalBytes(-1),
transferBufferSize(0),
lastUpdateTime(0),
bytesPerSecond(0)
{
    
}

//------------------------------------------------------------------------------
ofxHTTPProgressMonitor::~ofxHTTPProgressMonitor() { }

//------------------------------------------------------------------------------
void ofxHTTPProgressMonitor::update(streamsize _totalBytesTransferred,
                                    streamsize _totalBytes,
                                    size_t _transferBufferSize,
                                    unsigned long long _lastUpdate) {
    
//    ofScopedLock
    
}

//------------------------------------------------------------------------------
streamsize ofxHTTPProgressMonitor::getTotalBytesTranferred() {

}

//------------------------------------------------------------------------------
streamsize ofxHTTPProgressMonitor::getTotalBytes() {
    
}

//------------------------------------------------------------------------------
float ofxHTTPProgressMonitor::getPercentageTransferred() {
    
}

//------------------------------------------------------------------------------
size_t ofxHTTPProgressMonitor::getTransferBufferSize() {
    
}

//------------------------------------------------------------------------------
void ofxHTTPProgressMonitor::reset() {
    
}
