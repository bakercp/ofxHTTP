//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
    
    std::string getThreadName() const
    {
        return _threadName;
    }
    
private:
    std::string _threadName;

};
