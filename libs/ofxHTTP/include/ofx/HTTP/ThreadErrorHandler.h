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


#include "ofLog.h"
#include "Poco/ErrorHandler.h"


/// \brief A ThreadErrorHandler to implement in-thread exception handling.
class ThreadErrorHandler: public Poco::ErrorHandler
{
public:
    ThreadErrorHandler(const std::string& threadName = "NONE"):
        _threadName(threadName)
    {
    }
    
    virtual ~ThreadErrorHandler()
    {
    }
    
    void exception(const Poco::Exception& exc)
    {
        ofLogError("ofThreadErrorHandler::exception") << "Uncaught thread exception: " << exc.displayText();
    }
    
    void exception(const std::exception& exc)
    {
        ofLogError("ofThreadErrorHandler::exception") << "Uncaught thread exception: " << exc.what();
    }

    void exception()
    {
        ofLogError("ofThreadErrorHandler::exception") << "Uncaught thread exception: Unknown exception.";
    }
    
    void setThreadName(const std::string& threadName)
    {
        _threadName = threadName;
    }
    
    std::string getName() const
    {
        return _threadName;
    }
    
private:
    std::string _threadName;

};
