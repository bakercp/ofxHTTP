/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 =============================================================================*/

#pragma once

#include "ofUtils.h"
#include "ofTypes.h"

#define UNKNOWN_TOTAL_BYTES -1

class ofxHTTPProgressMonitor {
public:
    
    ofxHTTPProgressMonitor();
    virtual ~ofxHTTPProgressMonitor();
    
    void update(streamsize _totalBytesTransferred,
                streamsize _totalBytes,
                size_t _transferBufferSize,
                unsigned long long _lastUpdate = ofGetSystemTime());

    streamsize getTotalBytesTranferred();
    streamsize getTotalBytes();
    
    float getPercentageTransferred();
    
    size_t getTransferBufferSize();

    void reset();
    
private:
    streamsize totalBytesTransferred;
    streamsize totalBytes;
    size_t transferBufferSize;

    unsigned long long lastUpdateTime;
    float bytesPerSecond;
    
    ofMutex mutex;
    
};