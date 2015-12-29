// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include <ostream>
#include <istream>
#include <cctype>
#include "Poco/UnbufferedStreamBuf.h"
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/BaseResponse.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


class AbstractRequestStreamListener
{
public:
    virtual ~AbstractRequestStreamListener()
    {
    }

    virtual void progress(const BaseRequest& request,
                          Context& context,
                          std::streamsize totalBytesTransferred) = 0;

};


class AbstractResponseStreamListener
{
public:
    virtual ~AbstractResponseStreamListener()
    {
    }

    virtual void progress(const BaseRequest& request,
                          const BaseResponse& response,
                          Context& context,
                          std::streamsize totalBytesTransferred) = 0;
    
};


class ClientProgressStreamBuf: public Poco::UnbufferedStreamBuf
{
public:
//    ClientProgressStreamBuf();

    ClientProgressStreamBuf(std::ostream& ostr,
                            const BaseRequest& request,
                            Context& context,
                            AbstractRequestStreamListener& listener,
                            std::streamsize bytesPerProgressUpdate);

    ClientProgressStreamBuf(std::istream& istr,
                            const BaseRequest& request,
                            const BaseResponse& response,
                            Context& context,
                            AbstractResponseStreamListener& listener,
                            std::streamsize bytesPerProgressUpdate);

    ~ClientProgressStreamBuf();

    void reset();

protected:
    int readFromDevice();
    int writeToDevice(char c);

private:
    const BaseRequest* _pRequest;
    const BaseResponse* _pResponse;
    Context* _pContext;

    std::ostream* _pRequestStream;
    std::istream* _pResponseStream;

    AbstractRequestStreamListener*  _pRequestStreamListener;
    AbstractResponseStreamListener* _pResponseStreamListener;

    std::streamsize _requestStreamBytes;
    std::streamsize _responseStreamBytes;

    std::streamsize _bytesPerProgressUpdate;

};


class ClientProgressIOS: public virtual std::ios
{
public:
    ClientProgressIOS(std::ostream& ostr,
                      const BaseRequest& request,
                      Context& context,
                      AbstractRequestStreamListener& listener,
                      std::streamsize bytesPerProgressUpdate);

    ClientProgressIOS(std::istream& istr,
                      const BaseRequest& request,
                      const BaseResponse& response,
                      Context& context,
                      AbstractResponseStreamListener& listener,
                      std::streamsize bytesPerProgressUpdate);

    ~ClientProgressIOS();

    /// \brief Returns a pointer to the underlying streambuf.
    ClientProgressStreamBuf* rdbuf();

protected:
    ClientProgressStreamBuf _buf;

};


class ClientProgressRequestStream: public ClientProgressIOS, public std::ostream
{
public:
    /// \brief Creates the CountingOutputStream and connects it to the given input stream.
    ClientProgressRequestStream(std::ostream& ostr,
                                const BaseRequest& request,
                                Context& context,
                                AbstractRequestStreamListener& listener,
                                std::streamsize bytesPerProgressUpdate);

    /// \brief Destroys the ProgressOutputStream.
    ~ClientProgressRequestStream();
    
};


class ClientProgressResponseStream: public ClientProgressIOS, public std::istream
{
public:
    /// \brief Creates the CountingInputStream and connects it to the given input stream.
    ClientProgressResponseStream(std::istream& istr,
                                 const BaseRequest& request,
                                 const BaseResponse& response,
                                 Context& context,
                                 AbstractResponseStreamListener& listener,
                                 std::streamsize bytesPerProgressUpdate);

    /// \brief Destroys the stream.
    ~ClientProgressResponseStream();

};


} } // namespace ofx::HTTP
