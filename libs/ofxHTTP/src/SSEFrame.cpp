//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
