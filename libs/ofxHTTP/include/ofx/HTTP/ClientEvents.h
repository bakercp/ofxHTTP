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


#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"
#include "ofx/IO/ByteBuffer.h"


namespace ofx {
namespace HTTP {


class MutableClientRequestArgs: public ofEventArgs
{
public:
    MutableClientRequestArgs(BaseRequest& request, Context& context);
    virtual ~MutableClientRequestArgs();
    BaseRequest& request() const;
    Context& context();

protected:
    BaseRequest& _request;
    Context& _context;
    
};


class MutableClientResponseArgs: public MutableClientRequestArgs
{
public:
    MutableClientResponseArgs(BaseRequest& request,
                              BaseResponse& response,
                              Context& context);
    
    virtual ~MutableClientResponseArgs();

    BaseResponse& response();

protected:
    BaseResponse& _response;
    
};



class BaseClientRequestArgs: public ofEventArgs
{
public:
    BaseClientRequestArgs(const BaseRequest& request, Context& context);

    virtual ~BaseClientRequestArgs();

    const BaseRequest& request() const;

    Context& context();

    std::string requestId() const;

protected:
    const BaseRequest& _request;

    Context& _context;
    
};


/// \brief A base class for progress event data.
class BaseProgressArgs
{
public:
    /// \brief Create BaseProgressArgs with the total bytes transfered.
    /// \param totalBytesTransferred The total number of bytes transferred.
    BaseProgressArgs(std::streamsize totalBytesTransferred);

    /// \brief Destroy the BaseProgressArgs.
    virtual ~BaseProgressArgs();

    /// \brief Get the total content length associated with this request.
    ///
    /// If the total content length is unknown (e.g. during chunked-transfer)
    /// UNKNOWN_CONTENT_LENGTH is returned.
    ///
    /// \returns the total length of the content in bytes iff the content length
    /// is known.  Otherwise returns UNKNOWN_CONTENT_LENGTH.
    virtual std::streamsize contentLength() const = 0;

    /// \brief Get the total number of bytes transferred during this request.
    /// \returns the total number of bytes transferred.
    std::streamsize totalBytesTransferred() const;

    /// \brief Get the progress of the request upload or download.
    ///
    /// If the total content length is unknown (e.g. during chunked-transfer)
    /// progress of UNKNOWN_CONTENT_LENGTH will be
    /// returned.
    ///
    /// \returns a value 0.0 - 1.0 iff the content length is known. Otherwise
    /// returns UNKNOWN_CONTENT_LENGTH.
    float progress() const;

    /// \brief A constant indicating an unknown content length.
    static const int UNKNOWN_CONTENT_LENGTH;

protected:
    /// \brief The total number of bytes transferred so far.
    std::streamsize _totalBytesTransferred;

};


class ClientRequestProgressArgs:
    public BaseClientRequestArgs,
    public BaseProgressArgs
{
public:
    ClientRequestProgressArgs(const BaseRequest& request,
                              Context& context,
                              std::streamsize totalBytesTransferred);

    virtual ~ClientRequestProgressArgs();

    std::streamsize contentLength() const override;

};


class BaseClientResponseArgs: public BaseClientRequestArgs
{
public:
    BaseClientResponseArgs(const BaseRequest& request,
                           const BaseResponse& response,
                           Context& context);

    virtual ~BaseClientResponseArgs();

    const BaseResponse& response() const;

protected:
    const BaseResponse& _response;

};


class ClientResponseProgressArgs:
    public BaseClientResponseArgs,
    public BaseProgressArgs
{
public:
    ClientResponseProgressArgs(const BaseRequest& request,
                               const BaseResponse& response,
                               Context& context,
                               std::streamsize bytesTransferred);

    virtual ~ClientResponseProgressArgs();

    std::streamsize contentLength() const override;

};



class ClientResponseEventArgs: public BaseClientResponseArgs
{
public:
    ClientResponseEventArgs(std::istream& responseStream,
                            const BaseRequest& request,
                            const BaseResponse& response,
                            Context& context);

    virtual ~ClientResponseEventArgs();

    std::istream& responseStream() const;

protected:
    std::istream& _responseStream;
    
};


class ClientResponseBufferEventArgs: public BaseClientResponseArgs
{
public:
    ClientResponseBufferEventArgs(const IO::ByteBuffer& buffer,
                                  const BaseRequest& request,
                                  const BaseResponse& response,
                                  Context& context);

    virtual ~ClientResponseBufferEventArgs();

    const IO::ByteBuffer& buffer() const;

protected:
    const IO::ByteBuffer _buffer;
    
};


class ClientErrorEventArgs: public BaseClientResponseArgs
{
public:
    ClientErrorEventArgs(const BaseRequest& request,
                         const BaseResponse& response,
                         Context& context,
                         const Poco::Exception& exception);

    virtual ~ClientErrorEventArgs();

    const Poco::Exception& exception() const;

protected:
    const Poco::Exception& _exception;

};


class ClientEvents
{
public:
    ofEvent<MutableClientRequestArgs> onHTTPClientRequestFilterEvent;
    ofEvent<MutableClientResponseArgs> onHTTPClientResponseFilterEvent;

    ofEvent<ClientErrorEventArgs> onHTTPClientErrorEvent;
    ofEvent<ClientResponseEventArgs> onHTTPClientResponseEvent;

    ofEvent<ClientRequestProgressArgs> onHTTPClientRequestProgress;
    ofEvent<ClientResponseProgressArgs> onHTTPClientResponseProgress;

};


} } // namespace ofx::HTTP
