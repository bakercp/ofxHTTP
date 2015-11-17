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
