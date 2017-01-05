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


#include <ios>
#include <map>
#include "Poco/Timestamp.h"


namespace ofx {
namespace HTTP {


class Progress
{
public:
    Progress();

    Progress(int64_t totalBytes);

    virtual ~Progress();

    int64_t getTotalBytesTranferred() const;

    void setTotalBytesTransferred(int64_t totalBytesTransferred);

    int64_t getTotalBytes() const;

    void setTotalBytes(int64_t totalBytes);

    /// \brief Get the progress of the request upload or download.
    ///
    /// If the total content length is unknown (e.g. during chunked-transfer)
    /// that can't be estimated, progress of UNKNOWN_CONTENT_LENGTH will be
    /// returned.
    ///
    /// In cases where the totalBytes cannot be accurately determined, progress
    /// values not accurately reflect the actual progress. The user is
    /// is encouraged to handle progress values outside of of expected 0-1
    /// ranges.
    ///
    /// In cases where the total bytes are 0, the progress will always return
    /// 1.0 indicating completion.
    ///
    /// \returns a progress value if total bytes are > 0, otherwise UNKNOWN_CONTENT_LENGTH.
    float progress() const;

    /// \returns the approximate bytes transferred per second.
    float bytesPerSecond() const;

    /// \returns the last updated time.
    Poco::Timestamp lastUpdateTime() const;

    Poco::Timestamp startTime() const;

    /// \brief A constant indicating an unknown content length.
    static const int64_t UNKNOWN_CONTENT_LENGTH;

private:
    int64_t _totalBytesTransferred = 0;
    int64_t _totalBytes = UNKNOWN_CONTENT_LENGTH;
    Poco::Timestamp _lastUpdateTime;
    Poco::Timestamp _startTime;
    float _bytesPerSecond = 0.0;

};


} } // namespace ofx::HTTP
