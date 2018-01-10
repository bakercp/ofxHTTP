//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <limits>
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
