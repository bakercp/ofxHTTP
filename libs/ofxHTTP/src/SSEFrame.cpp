// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/SSEFrame.h"


namespace ofx {
namespace HTTP {


SSEFrame::SSEFrame(const std::string& data):
    SSEFrame("", data)
{
}


SSEFrame::SSEFrame(const std::string& event, const std::string& data):
    _event(event),
    _data(data)
{
}


SSEFrame::~SSEFrame()
{
}


std::string SSEFrame::event() const
{
    return _event;
}


std::string SSEFrame::data() const
{
    return _data;
}


IndexedSSEFrame::IndexedSSEFrame(const SSEFrame& frame, uint64_t index):
    _frame(frame),
    _index(index)
{
}


const SSEFrame& IndexedSSEFrame::frame() const
{
    return _frame;
}


uint64_t IndexedSSEFrame::index() const
{
    return _index;
}


} } // namespace ofx::HTTP
