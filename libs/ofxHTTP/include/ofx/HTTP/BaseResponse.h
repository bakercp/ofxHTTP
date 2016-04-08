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


#include <istream>
#include "Poco/Exception.h"
#include "Poco/NullStream.h"
#include "Poco/Runnable.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPIOStream.h"
#include "ofSSLManager.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/Context.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofTypes.h"
#include "ofPixels.h"


namespace ofx {
namespace HTTP {


/// \brief A BaseResponse is used to capture exceptions and response headers.
///
/// The user must ask for the input stream from the client or use a Buffered
/// Response to receive the data.
class BaseResponse: public Poco::Net::HTTPResponse
{
public:
    /// \brief Destroy the BaseResponse.
    virtual ~BaseResponse();

private:
    friend class BaseClient;

};


/// \brief The base class for a buffered response.
///
/// A buffered response buffers and packages all response data.
class BaseBufferedResponse: public BaseResponse
{
public:
    /// \brief Destroy the BaseBufferedResponse.
    virtual ~BaseBufferedResponse();

    /// \brief Get an error display message.
    ///
    /// This returns a string containing a human readable description of why
    /// a response may have failed.
    ///
    /// \returns erros as a string or an empty string if none.
    std::string error() const;

    /// \returns a const pointer to the exception or nullptr if none.
    const Poco::Exception* exception() const;

    /// \returns true if there is an exception.
    bool hasException() const;

    /// \returns true if the status is a 2xx Success code with no exceptions.
    bool isSuccess() const;

protected:
    /// \brief Set an exception.
    /// \param exception The exception to set.
    void setException(std::unique_ptr<Poco::Exception>&& exception);

    /// \brief Buffers the input stream.
    ///
    /// This section is allowed to throw exceptions that should be caught by the
    /// calling party. Those exceptions should be handled or set or added to the
    /// response by calling setException().
    ///
    /// \param istr The input stream to buffer.
    /// \throws Any number of exceptions.
    virtual void bufferStream(std::istream& responseStream) = 0;

private:
    /// \brief The captured exception.
    std::unique_ptr<Poco::Exception> _exception = nullptr;

    friend class BaseClient;

};


/// \brief A BufferedResponse reads the bytes into a raw byte buffer.
class BufferedResponse: public BaseBufferedResponse
{
public:
    /// \brief Destroy a BufferedResponse.
    virtual ~BufferedResponse();

    /// \returns the buffered input stream in a byte buffer.
    const ofBuffer& data() const;

    /// \brief Attempt to load the data as ofPixels.
    /// \returns filled ofPixels or an empty pixels if not possible.
    ofPixels pixels() const;

    /// \brief Attempt to load the data as parsed JSON data.
    /// \returns parsed JSON data or a null element if not possible.
    ofJson json() const;

protected:
    virtual void bufferStream(std::istream& responseStream) override;

private:
    /// \brief The buffered input stream data.
    ofBuffer _buffer;

    friend class BaseClient;

};


} } // namespace ofx::HTTP
