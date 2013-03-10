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

#include "Poco/Thread.h"
#include "Poco/Runnable.h"

#include "ofTypes.h"

#include "ofxHTTPBaseRequest.h"
#include "ofxHTTPConstants.h"
#include "ofxHTTPClientContext.h"
#include "ofxHTTPResponseStream.h"

using Poco::Thread;
using Poco::Runnable;

class ofxThreadSettings {
public:
    ofxThreadSettings(const string& _name = "default",
                      const Thread::Priority& _priority = Thread::PRIO_NORMAL) :
    name(_name),
    priority(_priority)
    { }
    
    string getName() const { return name; }
    void   setName(const string& _name) { name = _name; }
    
    Thread::Priority getPriority() const { return priority; }
    void setPriority(const Thread::Priority& _priority) { priority = _priority; }

protected:
    Thread::Priority priority;
    string name;
};

enum ofxHTTPRequestExecutorState {
  NONE
};

class ofxHTTPRequestExecutor : public Runnable {
public:
    
    ofxHTTPRequestExecutor(const ofxHTTPBaseRequest& _request,
                           
                           const ofxThreadSettings& _threadSettings = ofxThreadSettings());
        
    ~ofxHTTPRequestExecutor();
//    ActiveMethod
    ofxThreadSettings getThreadSettings() const;
    
    
    static ofxHTTPResponseStream* execute(const ofxHTTPBaseRequest& _request,
                                          //ofxHTTPBaseResponseStreamConsumer* _streamConsumer,
                                          ofxHTTPClientContextPtr context = ofxHTTPClientContext::defaultClientContext());
    
    bool isDone() const;
    
    ofxHTTPResponseStream* getResponseStream();
    
private:
    bool bIsDone;
    //Exception exception; // exceptions are passed with the ofxHTTPResponseStream
    
    ofxHTTPBaseRequest* request;
    
    ofxHTTPResponseStream* responseStream;

//    ofMutex mutex;

    ofxThreadSettings settings;

};