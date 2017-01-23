//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ThreadSettings.h"


namespace ofx {
namespace HTTP {


ThreadSettings::ThreadSettings(const std::string& name,
                               const Poco::Thread::Priority& priority):
    _name(name),
    _priority(priority)
{
}


ThreadSettings::~ThreadSettings()
{
}


std::string ThreadSettings::getName() const
{
    return _name;
}


void ThreadSettings::setName(const std::string& name)
{
    _name = name;
}


Poco::Thread::Priority ThreadSettings::getPriority() const
{
    return _priority;
}


void ThreadSettings::setPriority(const Poco::Thread::Priority& priority)
{
    _priority = priority;
}


} } // namespace ofx::HTTP
