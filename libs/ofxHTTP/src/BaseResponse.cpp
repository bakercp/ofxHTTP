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


#include "ofx/HTTP/BaseResponse.h"
//#include "Poco/CountingStream.h"
//#include "Poco/TeeStream.h"


namespace ofx {
namespace HTTP {


BaseResponse::BaseResponse():
    Poco::Net::HTTPResponse()//,
//    _pResponseStream(0),
//    _pException(0)
{
}


BaseResponse::~BaseResponse()
{
//    // deleting a null pointer is a noop
//    delete _pResponseStream; // cleans up the stream and the backing session
//    _pResponseStream = 0;
//
//    delete _pException;
//    _pException = 0;
}


//bool BaseResponse::hasResponseStream() const
//{
//    return 0 != _pResponseStream;
//}
//
//std::istream& BaseResponse::getResponseStream()
//{
//    poco_assert(_pResponseStream);
//    
//    return *_pResponseStream;
//}
//
//
//void BaseResponse::setResponseStream(std::istream* pResponseStream)
//{
//    delete _pResponseStream;
//    _pResponseStream = pResponseStream;
//}
//
//
//bool BaseResponse::hasException() const
//{
//    return 0 != _pException;
//}
//
//
//const Poco::Exception* BaseResponse::getException() const
//{
//    return _pException;
//}
//
//
//void BaseResponse::setException(Poco::Exception* pException)
//{
//    delete _pException;
//    _pException = pException;
//}


} } // namespace ofx::HTTP
