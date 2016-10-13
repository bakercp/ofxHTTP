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
#include "ofConstants.h"
#include "json.hpp"
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
#include "ofx/IO/ByteBufferStream.h"
#include "ofImage.h"
#include "ofTypes.h"


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

    virtual bool hasResponseStream() const;

    /// \brief Get the data stream.
    ///
    /// Sub-classes may buffer this stream and allow the stream to be returned
    /// multiple times.
    virtual std::istream& stream();

    /// \brief Calculate the estimated content length.
    ///
    /// This is based upon the given Content-Length header, or other information
    /// if the Content-Length header is not available.
    ///
    /// \returns estimated content length or UNKNOWN_CONTENT_LENGTH if unknown.
    int64_t estimatedContentLength() const;

    static const std::string CONTENT_RANGE;
    static const std::string BYTES_UNIT;

private:
    std::unique_ptr<IO::FilteredInputStream> _responseStream = nullptr;

    friend class BaseClient;
    friend class Client;

};


/// \brief A BaseResponse is used to capture exceptions and response headers.
///
/// The user must ask for the input stream from the client or use a Buffered
/// Response to receive the data.
///
///
class BufferedResponse: public BaseResponse
{
public:
    /// \brief Destroy the BufferedResponse.
    virtual ~BufferedResponse();

    /// \brief Get the response as a stream.
    ///
    /// If the stream has not been accessed, it will buffer the response.
    /// If the stream has already been accessed, this will return an input
    /// stream to the buffer.
    ///
    /// \returns the response input stream.
    virtual std::istream& stream();

    /// \brief Get the buffer.
    ofBuffer buffer();

    /// \returns true if the stream has already been buffered.
    bool isBuffered() const;

private:
    void _bufferResponse();

    /// \brief The input stream to wrap the buffer.
    ///
    /// If the stream is buffered, this is non-null.
    std::unique_ptr<IO::ByteBufferInputStream> _bbis = nullptr;

    /// \brief The buffered stream data.
    IO::ByteBuffer _buffer;

};



class ClientTransaction
{
public:
    ClientTransaction(BaseRequest& request,
                      BaseResponse& response,
                      Context& context):
        _request(request),
        _response(response),
        _context(context)
    {
    }

    BaseRequest& request()
    {
        return _request;
    }

    BaseResponse& response()
    {
        return _response;
    }

    Context& context()
    {
        return _context;
    }

private:
    BaseRequest& _request;
    BaseResponse& _response;
    Context& _context;

};










///// \brief A PackagedResponse owns both the Session and Request.
/////
///// It is used at a high level to return all data used during a session.
/////
///// \tparam RequestType The subclass of BaseRequest captured in the response.
//template<typename RequestType>
//class PackagedResponse: public BaseResponse
//{
//public:
//    /// \brief Create a packaged response from a Request and a Context.
//    ///
//    /// It is assumed that the PackagedResponse will take ownership of the
//    /// request and response. After creating the response, the request and
//    /// context used to create the packaged response will be empty.
//    ///
//    /// \param request The RequestType to package.
//    /// \param context The Context to package.
//    PackagedResponse(std::unique_ptr<RequestType> request,
//                     std::unique_ptr<Context> context):
//        _request(std::move(request)),
//        _context(std::move(context))
//    {
//    }
//
//    /// \brief Destroy the PackagedResponse response.
//    virtual ~PackagedResponse()
//    {
//    }
//
//    /// \returns a reference to the request.
//    RequestType& request()
//    {
//        return *_request;
//    }
//
//    /// \returns a const reference to the request.
//    const RequestType& request() const
//    {
//        return *_request;
//    }
//
//    /// \returns a reference to the Context.
//    Context& context()
//    {
//        return *_context;
//    }
//
//    /// \returns a const reference to the Context.
//    const Context& context() const
//    {
//        return *_context;
//    }
//
//    /// \brief Get an error display message.
//    ///
//    /// This returns a string containing a human readable description of why
//    /// a response may have failed.
//    ///
//    /// \returns errors as a string or an empty string if none.
//    virtual std::string error() const
//    {
//        std::stringstream ss;
//
//        if (getStatus() < 200 || getStatus() >= 300)
//        {
//            ss << "HTTPStatus: " << getStatus() << " Reason: " << getReason();
//        }
//
//        if (hasException())
//        {
//            if (!ss.str().empty())
//            {
//                ss << " ";
//            }
//
//            ss << _exception->displayText();
//        }
//        
//        return ss.str();
//    }
//
//    /// \returns a const pointer to the exception or nullptr if none.
//    const Poco::Exception* exception() const
//    {
//        return _exception.get();
//    }
//
//    /// \returns true if there is an exception.
//    bool hasException() const
//    {
//        return _exception != nullptr;
//    }
//
//    /// \returns true if the status is a 2xx Success code with no exceptions.
//    virtual bool isSuccess() const
//    {
//        return !hasException() && getStatus() >= 200 && getStatus() < 300;
//    }
//
//protected:
//    /// \brief Take ownership of an exception.
//    /// \param exception The exception to set.
//    void setException(std::unique_ptr<Poco::Exception> exception)
//    {
//        _exception = std::move(exception);
//    }
//
//private:
//    /// \brief The request owned by this response.
//    std::unique_ptr<RequestType> _request = nullptr;
//
//    /// \brief The context owned by this response.
//    std::unique_ptr<Context> _context = nullptr;
//
//    /// \brief The captured exception, if any.
//    std::unique_ptr<Poco::Exception> _exception = nullptr;
//
//    friend class BaseClient;
//
//};

//
//
///// \brief A BufferedResponse reads the bytes into a raw byte buffer.
//template<typename RequestType>
//class BufferedResponse: public PackagedResponse<RequestType>
//{
//public:
//    using PackagedResponse<RequestType>::PackagedResponse;
//
//    /// \brief Destroy a BufferedResponse.
//    virtual ~BufferedResponse()
//    {
//    }
//
//    /// \brief Set the buffered data.
//    ///
//    /// Upon setting the buffered data, parseBuffer() will be called so that
//    /// sub-classes can extract structured data.
//    ///
//    /// \param buffer The buffered data.
//    void setBuffer(const ofBuffer& buffer)
//    {
//        _buffer = buffer;
//        parseBuffer();
//    }
//
//    /// \returns the raw buffered data.
//    const ofBuffer& getBuffer() const
//    {
//        return _buffer;
//    }
//
//protected:
//    /// \brief Sub-classes can implement parse buffer to extract structured data.
//    virtual void parseBuffer()
//    {
//    }
//
//private:
//    /// \brief The buffered input stream data.
//    ofBuffer _buffer;
//
//};


} } // namespace ofx::HTTP
