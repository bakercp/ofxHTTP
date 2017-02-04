//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <ostream>
#include <istream>
#include <cctype>
#include "Poco/UnbufferedStreamBuf.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Response.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


class ClientProgressStreamBuf: public Poco::UnbufferedStreamBuf
{
public:
    ClientProgressStreamBuf(Context& context,
                            Request& request,
                            std::ostream& ostr);

    ClientProgressStreamBuf(Context& context,
                            Request& request,
                            Response& response,
                            std::istream& istr);

    ~ClientProgressStreamBuf();

protected:
    int writeToDevice(char c) override;
    int readFromDevice() override;

private:
    Context* _pContext = nullptr;
    Request* _pRequest = nullptr;
    Response* _pResponse = nullptr;

    std::ostream* _pRequestStream = nullptr;
    std::istream* _pResponseStream = nullptr;

    std::streamsize _requestStreamBytesTransferred = 0;
    std::streamsize _responseStreamBytesTransferred = 0;

    Progress _requestStreamProgress;
    Progress _responseStreamProgress;

    int64_t _bytesPerUpdate = 0;
    int64_t _maxUpdateInterval = 0;

};


class ClientProgressIOS: public virtual std::ios
{
public:
    ClientProgressIOS(Context& context,
                      Request& request,
                      std::ostream& ostr);

    ClientProgressIOS(Context& context,
                      Request& request,
                      Response& response,
                      std::istream& istr);

    ~ClientProgressIOS();

    /// \brief Returns a pointer to the underlying streambuf.
    ClientProgressStreamBuf* rdbuf();

protected:
    /// \brief The client progress stream buffer.
    ClientProgressStreamBuf _buf;

};


class ClientProgressRequestStream:
    public ClientProgressIOS,
    public std::ostream
{
public:
    /// \brief Creates the CountingOutputStream and connects it to the given input stream.
    ClientProgressRequestStream(Context& context,
                                Request& request,
                                std::ostream& ostr);

    /// \brief Destroys the ProgressOutputStream.
    ~ClientProgressRequestStream();

};


class ClientProgressResponseStream:
    public ClientProgressIOS,
    public std::istream
{
public:
    /// \brief Creates the CountingInputStream and connects it to the given input stream.
    ClientProgressResponseStream(Context& context,
                                 Request& request,
                                 Response& response,
                                 std::istream& istr);

    /// \brief Destroys the stream.
    ~ClientProgressResponseStream();

};


class ClientProgressRequestStreamFilter: public IO::AbstractOutputStreamFilter
{
public:
    ClientProgressRequestStreamFilter(Context& context,
                                      Request& request);

    virtual ~ClientProgressRequestStreamFilter();

    std::ostream& filter(std::ostream& stream) override;

private:
    Context& _context;
    Request& _request;

    std::unique_ptr<ClientProgressRequestStream> _stream;
    
};


class ClientProgressResponseStreamFilter: public IO::AbstractInputStreamFilter
{
public:
    ClientProgressResponseStreamFilter(Context& context,
                                       Request& request,
                                       Response& response);

    virtual ~ClientProgressResponseStreamFilter();

    std::istream& filter(std::istream& stream) override;

private:
    Context& _context;
    Request& _request;
    Response& _response;

    std::unique_ptr<ClientProgressResponseStream> _stream;

};


} } // namespace ofx::HTTP
