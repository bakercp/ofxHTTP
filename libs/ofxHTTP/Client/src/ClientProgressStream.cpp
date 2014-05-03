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


#include "ofx/HTTP/Types/ClientProgressStream.h"


namespace ofx {
namespace HTTP {
namespace Client {


ClientProgressStreamBuf::ClientProgressStreamBuf():
    _request(0);
    _response(0);
    _context(0);
    _pIstr(0),
    _pOstr(0),
    _inputStreamBytes(0),
    _outputStreamBytes(0),
    _pInputStreamListener(0),
    _pOutputStreamListener(0)
{
}


//    ClientProgressStreamBuf(std::ostream& ostr,
//                            const BaseRequest& request,
//                            Context& context,
//                            AbstractRequestStreamProgressListener& requestStreamListener);
//
//    ClientProgressStreamBuf(std::istream& istr,
//                            const BaseRequest& request,
//                            const BaseResponse& response,
//                            Context& context,
//                            AbstractResponseStreamProgressListener& responseStreamListener);
//    



ClientProgressStreamBuf::ClientProgressStreamBuf(std::istream& istr,
                                                 AbstractInputStreamProgressListener& inputStreamListener):
    _pIstr(&istr),
    _pOstr(0),
    _inputStreamBytes(0),
    _outputStreamBytes(0),
    _pInputStreamListener(&inputStreamListener),
    _pOutputStreamListener(0)
{
}


ProgressStreamBuf::ProgressStreamBuf(std::ostream& ostr,
                                     AbstractOutputStreamProgressListener& outputStreamListener):
    _pIstr(0),
    _pOstr(&ostr),
    _pInputStreamListener(0),
    _pOutputStreamListener(&outputStreamListener)
{
}


ProgressStreamBuf::~ProgressStreamBuf()
{
}


int ProgressStreamBuf::readFromDevice()
{
    if (_pIstr)
    {
        int c = _pIstr->get();

        if (c != -1)
        {
            ++_inputStreamBytes;
        }

        return c;
    }
    return -1;
}


int ProgressStreamBuf::writeToDevice(char c)
{
    ++_outputStreamBytes;
    if (_pOstr) _pOstr->put(c);
    return charToInt(c);
    return c;
}


void ProgressStreamBuf::reset()
{
    _inputStreamBytes = 0;
    _outputStreamBytes = 0;
}


ProgressIOS::ProgressIOS()
{
    poco_ios_init(&_buf);
}


ProgressIOS::ProgressIOS(std::istream& istr,
                         AbstractInputStreamProgressListener& inputStreamListener):
    _buf(istr, inputStreamListener)
{
    poco_ios_init(&_buf);
}


ProgressIOS::ProgressIOS(std::ostream& ostr,
                         AbstractOutputStreamProgressListener& outputStreamListener):
    _buf(ostr, outputStreamListener)
{
    poco_ios_init(&_buf);
}


ProgressIOS::~ProgressIOS()
{
}


ProgressStreamBuf* ProgressIOS::rdbuf()
{
    return &_buf;
}


ProgressInputStream::ProgressInputStream(std::istream& istr,
                                         AbstractInputStreamProgressListener& inputStreamListener):
    ProgressIOS(istr, inputStreamListener),
    std::istream(&_buf)
{
}


ProgressInputStream::~ProgressInputStream()
{
}


ProgressOutputStream::ProgressOutputStream():
    std::ostream(&_buf)
{
}


ProgressOutputStream::ProgressOutputStream(std::ostream& ostr,
                                           AbstractOutputStreamProgressListener& outputStreamListener):
    ProgressIOS(ostr, outputStreamListener),
    std::ostream(&_buf)
{
}


ProgressOutputStream::~ProgressOutputStream()
{
}


} } } // namespace ofx::HTTP::Client
