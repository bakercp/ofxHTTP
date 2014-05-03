// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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
#include "Poco/Foundation.h"
#include "Poco/Timestamp.h"
#include "Poco/UnbufferedStreamBuf.h"
#include "ofx/HTTP/Client/BaseRequest.h"
#include "ofx/HTTP/Client/BaseResponse.h"
#include "ofx/HTTP/Client/Context.h"


namespace ofx {
namespace HTTP {
namespace Client {


class AbstractRequestStreamProgressListener
{
public:
    virtual ~AbstractRequestStreamProgressListener()
    {
    }

    virtual void progress(const BaseRequest& request,
                          Context& context,
                          std::streamsize total) = 0;
};


class AbstractResponseStreamProgressListener
{
public:
    virtual ~AbstractResponseStreamProgressListener()
    {
    }

    virtual void progress(const BaseRequest& request,
                          const BaseResponse& response,
                          Context& context,
                          std::streamsize total) = 0;
    
};


class ClientProgressStreamBuf: public Poco::UnbufferedStreamBuf
{
public:

    virtual std::ostream& filter(std::ostream& requestStream,
                                 const BaseRequest& request,
                                 Context& context) = 0;

    ClientProgressStreamBuf();

    ClientProgressStreamBuf(std::ostream& ostr,
                            const BaseRequest& request,
                            Context& context,
                            AbstractRequestStreamProgressListener& requestStreamListener);

    ClientProgressStreamBuf(std::istream& istr,
                            const BaseRequest& request,
                            const BaseResponse& response,
                            Context& context,
                            AbstractResponseStreamProgressListener& responseStreamListener);

    ~ClientProgressStreamBuf();

    void reset();

protected:
    int readFromDevice();
    int writeToDevice(char c);

private:
    const* BaseRequest _request;
    const* BaseResponse _response;
    Context* _context;

    std::istream* _pIstr;
    std::ostream* _pOstr;

    std::streamsize _inputStreamBytes;
    std::streamsize _outputStreamBytes;

    AbstractInputStreamProgressListener*  _pInputStreamListener;
    AbstractOutputStreamProgressListener* _pOutputStreamListener;

};


class ProgressIOS: public virtual std::ios
{
public:
    ProgressIOS();
    ProgressIOS(std::istream& istr,
                AbstractInputStreamProgressListener& inputStreamListener);
    ProgressIOS(std::ostream& ostr,
                AbstractOutputStreamProgressListener& outputStreamListener);
    ~ProgressIOS();

    /// \brief Returns a pointer to the underlying streambuf.
    ProgressStreamBuf* rdbuf();

protected:
    ProgressStreamBuf _buf;

};


class ProgressInputStream: public ProgressIOS, public std::istream
{
public:
    /// Creates the CountingInputStream and connects it
    /// to the given input stream.
    ProgressInputStream(std::istream& istr,
                        AbstractInputStreamProgressListener& inputStreamListener);

    /// Destroys the stream.
    ~ProgressInputStream();

};


class ProgressOutputStream: public ProgressIOS, public std::ostream
{
public:
    /// Creates an unconnected CountingOutputStream.
    ProgressOutputStream();

    /// Creates the CountingOutputStream and connects it
    /// to the given input stream.
    ProgressOutputStream(std::ostream& ostr,
                         AbstractOutputStreamProgressListener& outputStreamListener);

    /// Destroys the ProgressOutputStream.
    ~ProgressOutputStream();

};


} } } // namespace ofx::HTTP::Client
