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


#pragma once


#include <string>
#include <vector>
#include "ofConstants.h"


namespace ofx {
namespace HTTP {


/// \brief A SSE frame.
class SSEFrame
{
public:
    /// \brief Create a SSEFrame without an event name.
    /// \param data The UTF-8 encoded data to send.
    SSEFrame(const std::string& data);

    /// \brief Create a SSEFrame with an event name.
    /// \param event The UTF-8 encoded event name to send.
    /// \param data The UTF-8 encoded data to send.
    SSEFrame(const std::string& event, const std::string& data);

    /// \brief Destroy a SSEFrame.
    virtual ~SSEFrame();

    /// \returns the UTF-8 encoded event name or an empty string if unset.
    std::string event() const;

    /// \returns the UTF-8 encoded data to be sent.
    std::string data() const;

//    /// \brief Return the parsed lines of the text.
//    std::vector<std::string> lines() const;
//
//    /// \brief
//    std::string sseFormattedData() const;

protected:
    /// \brief The UTF-8 encoded event name or an empty string if unset
    std::string _event;

    /// \brief The UTF-8 encoded data to be sent.
    std::string _data;

};


/// \brief An indexed SSE frame for caching and playback purposes.
class IndexedSSEFrame
{
public:
    /// \brief Create an indexed SSE frame.
    /// \param frame The frame to cache.
    /// \param index The index of the frame.
    IndexedSSEFrame(const SSEFrame& frame, uint64_t index);

    /// \returns a const reference to the indexed frame.
    const SSEFrame& frame() const;

    /// \returns the index of the frame.
    uint64_t index() const;

    enum
    {
        //
        NOT_INDEXED = std::numeric_limits<uint64_t>::max()
    };

private:
    /// The indexed frame.
    SSEFrame _frame;

    /// \brief The index of the frame.
    uint64_t _index = NOT_INDEXED;

};

    
} } // namespace ofx::HTTP
