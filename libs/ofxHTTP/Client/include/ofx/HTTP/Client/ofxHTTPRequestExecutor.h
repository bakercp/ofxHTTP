//// =============================================================================
////
//// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
////
//// Permission is hereby granted, free of charge, to any person obtaining a copy
//// of this software and associated documentation files (the "Software"), to deal
//// in the Software without restriction, including without limitation the rights
//// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//// copies of the Software, and to permit persons to whom the Software is
//// furnished to do so, subject to the following conditions:
////
//// The above copyright notice and this permission notice shall be included in
//// all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//// THE SOFTWARE.
////
//// =============================================================================
//
//
//#pragma once
//
//
//#include "Poco/Thread.h"
//#include "Poco/Runnable.h"
//#include "ofTypes.h"
//#include "BaseRequest.h"
//#include "ClientContext.h"
//#include "ResponseStream.h"
//
//
//using Poco::Thread;
//using Poco::Runnable;
//using ofx::HTTP::Request::BaseRequest;
//using ofx::HTTP::ClientContext;
//using ofx::HTTP::ResponseStream;
//
//
//class ofxThreadSettings
//{
//public:
//    ofxThreadSettings(const std::string& _name = "default",
//                      const Thread::Priority& _priority = Thread::PRIO_NORMAL):
//        name(_name),
//        priority(_priority)
//    {
//    }
//    
//    std::string getName() const
//    {
//        return name;
//    }
//
//    void setName(const std::string& _name)
//    {
//        name = _name;
//    }
//    
//    Thread::Priority getPriority() const
//    {
//        return priority;
//    }
//
//    void setPriority(const Thread::Priority& _priority)
//    {
//        priority = _priority;
//    }
//
//protected:
//    string name;
//    Thread::Priority priority;
//};
//
//class ofxHTTPRequestExecutor : public Runnable {
//public:
//    enum State
//    {
//        NONE
//    };
//    
//    ofxHTTPRequestExecutor(const BaseRequest& _request,
//                           
//                           const ofxThreadSettings& _threadSettings = ofxThreadSettings());
//        
//    ~ofxHTTPRequestExecutor();
////    ActiveMethod
//    ofxThreadSettings getThreadSettings() const;
//    
//    
////    static ofx::HTTP::ResponseStream* execute(const BaseRequest& _request,
////                                          //ofxHTTPBaseResponseStreamConsumer* _streamConsumer,
////                                          ClientContext::SharedPtr context = ClientContext::defaultClientContext());
//
//    bool isDone() const;
//    
//    ofx::HTTP::ResponseStream* getResponseStream();
//    
//private:
//    bool bIsDone;
//    //Exception exception; // exceptions are passed with the ofxHTTPResponseStream
//    
//    BaseRequest* request;
//    
//    ResponseStream* responseStream;
//
////    ofMutex mutex;
//
//    ofxThreadSettings settings;
//
//};
