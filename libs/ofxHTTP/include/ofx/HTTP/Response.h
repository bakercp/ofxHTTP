//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Context.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofx/IO/ByteBufferStream.h"
#include "ofImage.h"
#include "ofTypes.h"
#include "ofJson.h"
#include "ofXml.h"


namespace ofx {
namespace HTTP {


/// \brief A Response is used to capture exceptions and response headers.
///
/// The user must ask for the input stream from the client or use a Buffered
/// Response to receive the data.
class Response: public Poco::Net::HTTPResponse
{
public:
    /// \brief Destroy the Response.
    virtual ~Response();

    /// \brief Calculate the estimated content length.
    ///
    /// This is based upon the given Content-Length header, or other information
    /// if the Content-Length header is not available.
    ///
    /// \returns estimated content length or UNKNOWN_CONTENT_LENGTH if unknown.
    int64_t estimatedContentLength() const;

    /// \returns true of this response has a valid response stream.
    virtual bool hasResponseStream() const;

    /// \brief Get the data stream.
    ///
    /// stream() returns the response input stream. If the stream can be
    /// buffered (i.e. its content length is finite and known), the content
    /// will be be buffered. Subsequent calls to stream() will present the
    /// buffered data as a stream. Buffered data can also be accessed via the
    /// buffer() method.
    ///
    /// If the stream cannot be buffered (i.e. it is a streaming connection and
    /// its content is not finite or known), the content will not be buffered.
    /// In this case attempts to call stream() after it has been consumed may
    /// result in an exception.
    ///
    /// \returns the response stream.
    virtual std::istream& stream();

    /// \brief Determine if the response can be buffered.
    ///
    /// This is equivalent to checking if
    ///
    ///     return estimatedContentLength() != UNKNOWN_CONTENT_LENGTH.
    ///
    /// \returns true if this stream can be buffered.
    bool isBufferable() const;

    /// \returns true if the stream has already been buffered.
    bool isBuffered() const;

    /// \brief Get the buffer, if available.
    /// \returns response or an empty buffer if the response can't be buffered.
    ofBuffer buffer();

    /// \brief Get the byte buffer, if available.
    /// \returns response or an empty buffer if the response can't be buffered.
    IO::ByteBuffer byteBuffer();

    /// \returns true if the CONTENT-TYPE matches `image/*`.
    bool isPixels() const;

    /// \brief Get response as ofPixels.
    /// \returns empty pixels if the response can't be buffered or pixels can't be loaded.
    ofPixels pixels();

    /// \returns true if the CONTENT-TYPE header is `text/json` or `application/json`.
    bool isJson() const;

    /// \brief Get response as ofJson.
    /// \returns empty json if the response can't be buffered or json can't be parsed.
    ofJson json();

    /// \returns true if the CONTENT-TYPE header is `text/xml` or `application/xml`.
    bool isXml() const;

    /// \brief Get response as ofXml.
    /// \returns empty xml if the response can't be buffered or xml can't be parsed.
    ofXml xml();

    /// \brief Save the buffer contents to a file.
    ///
    /// Internally calls buffer() and saves the contents to the path.
    ///
    /// \param file The path to save the buffer.
    /// \returns true if the file save operation was successful.
    bool toFile(const std::filesystem::path& path);

    /// \brief Determine if this response is informational.
    /// \sa https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
    /// \returns true if the HTTP response code is >= 100 and < 200.
    bool isInformational() const;

    /// \brief Determine if this response is a success.
    /// \sa https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
    /// \returns true if the HTTP response code is >= 200 and < 300.
    bool isSuccess() const;

    /// \brief Determine if this response is a redirection.
    /// \sa https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
    /// \returns true if the HTTP response code is >= 300 and < 400.
    bool isRedirection() const;

    /// \brief Determine if this response is a client error.
    /// \sa https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
    /// \returns true if the HTTP response code is >= 400 and < 500.
    bool isClientError() const;

    /// \brief Determine if this response is a server error.
    /// \sa https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
    /// \returns true if the HTTP response code is >= 500.
    bool isServerError() const;

    /// \returns a string representing the response status and reason.
    std::string statusAndReason() const;

    /// \brief A constant value representing `Content-Range`.
    static const std::string CONTENT_RANGE;

    /// \brief A constant value representing a `byte`.
    static const std::string BYTES_UNIT;

private:
    /// \brief Internal function for buffering the response.
    void _bufferResponse();

    /// \brief The response stream created by the client.
    std::unique_ptr<IO::FilteredInputStream> _responseStream = nullptr;

    /// \brief The input stream to wrap the buffer.
    ///
    /// If the stream is buffered, this is non-null.
    std::unique_ptr<IO::ByteBufferInputStream> _bbis = nullptr;

    /// \brief The buffered stream data.
    IO::ByteBuffer _buffer;

    friend class BaseClient;
    friend class Client;

};


///// \brief A BufferedResponse is used to capture exceptions and response headers.
/////
///// When accessing the stream, the stream will be buffered. Alternatively, the
///// user can simply ask for the buffer.
//class BufferedResponse: public Response
//{
//public:
////    /// \brief Destroy the BufferedResponse.
////    virtual ~BufferedResponse();
////
////    /// \brief Get the response as a stream.
////    ///
////    /// If the stream has not been accessed, it will buffer the response.
////    /// If the stream has already been accessed, this will return an input
////    /// stream to the buffer.
////    ///
////    /// \returns the response input stream.
////    virtual std::istream& stream();
////
////    /// \brief Get the buffer.
////    ofBuffer buffer();
////
////    /// \returns true if the stream has already been buffered.
////    bool isBuffered() const;
//
//private:
//
//};



class ClientExchange
{
public:
    ClientExchange(Context& context,
                   Request& request,
                   Response& response):
        _context(context),
        _request(request),
        _response(response)
    {
    }

    Context& context()
    {
        return _context;
    }

    Request& request()
    {
        return _request;
    }

    Response& response()
    {
        return _response;
    }

private:
    Context& _context;
    Request& _request;
    Response& _response;

};










///// \brief A PackagedResponse owns both the Session and Request.
/////
///// It is used at a high level to return all data used during a session.
/////
///// \tparam RequestType The subclass of Request captured in the response.
//template<typename RequestType>
//class PackagedResponse: public Response
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
