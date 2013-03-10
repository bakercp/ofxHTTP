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
 
 ==============================================================================*/

#pragma once

#include "Poco/Exception.h"
#include "Poco/NullStream.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/StreamCopier.h"

#include "Poco/Net/HTTPAuthenticationParams.h"
#include "Poco/Net/NetException.h"

#include "ofEvents.h"

#include "ofxHTTPClientContext.h"
#include "ofxHTTPBaseTypes.h"

#include "ofxHTTPSessionSettings.h"
#include "ofxHTTPStreamUtils.h"

using Poco::IOException;
using Poco::NoThreadAvailableException;
using Poco::NullOutputStream;
using Poco::StreamCopier;
using Poco::Thread;
using Poco::ThreadPool;

using Poco::Net::HTTPAuthenticationParams;
using Poco::Net::HostNotFoundException;

typedef ThreadPool ofThreadPool;

class ofxHTTPClient {
public:
    
    ofxHTTPClient(ofxHTTPClientContextPtr _context = ofxHTTPClientContext::defaultClientContext(),
                  ofThreadPool& threadPool = ofThreadPool::defaultPool());

    virtual ~ofxHTTPClient();
    
    ofxHTTPResponseStream* exectute(const ofxHTTPBaseRequest& _request);
    void exectute(const ofxHTTPBaseRequest& _request, ofxBaseHTTPResponseStreamConsumer* consumer);
    
    ofxHTTPClientContextPtr getClientContext();
    
    ofThreadPool& getThreadPoolRef(); // modifications to the thread pool can be made via the ref
    
    
    
    // convenience method
    
    void setCredentials(const ofxHTTPAuthScope& scope, const ofxHTTPCredentials& credentials);
    void setCredentials(const URI& uri, const string& username, const string& password);
    
    
protected:
    void update(ofEventArgs& args);

    static ofxHTTPResponseStream* openResponseStream(const ofxHTTPBaseRequest& request,
                                                     ofxHTTPClientContextPtr context = ofxHTTPClientContext::defaultClientContext());
        
    Thread syncThread; // thread for executing syncronous calls
                       // must always be immediately joined
    
    ofThreadPool& threadPool; // thread pool for executing asynchronous calls
    
    ofxHTTPClientContextPtr context;

//    ofThreadErrorHandler errorHandler;
//    ErrorHandler* previousErrorHandler;
    
};