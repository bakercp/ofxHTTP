//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "Poco/Thread.h"


namespace ofx {
namespace HTTP {


class ThreadSettings
{
public:
    ThreadSettings(const std::string& name = "default",
                   const Poco::Thread::Priority& priority = Poco::Thread::PRIO_NORMAL);

    virtual ~ThreadSettings();

    std::string getName() const;
    void setName(const std::string& name);
    Poco::Thread::Priority getPriority() const;
    void setPriority(const Poco::Thread::Priority& priority);

protected:
    std::string _name;
    Poco::Thread::Priority _priority;
    
};


} } // namespace ofx::HTTP
