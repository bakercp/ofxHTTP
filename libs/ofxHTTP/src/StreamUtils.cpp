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


#include "ofx/HTTP/StreamUtils.h"


namespace ofx {
namespace HTTP {
        

//std::streamsize StreamUtils::consume(ResponseStream& responseStream,
//                                     std::size_t bufferSize)
//{
//    if(responseStream.hasResponseStream())
//    {
//        return consume(responseStream.getResponseStreamRef(),
//                       bufferSize);
//    }
//    else
//    {
//        return 0; // error
//    }
//}
//
//
//std::streamsize StreamUtils::consume(std::istream& istr, std::size_t bufferSize)
//{
//    Poco::NullOutputStream nos;
//    
//    return Poco::StreamCopier::copyStream(istr, nos, bufferSize);
//}
//
//
//std::streamsize StreamUtils::copyToString(ResponseStream& responseStream,
//                                          std::string& str,
//                                          std::size_t bufferSize)
//{
//    if(responseStream.hasResponseStream())
//    {
//        return copyToString(responseStream.getResponseStreamRef(),
//                            str,
//                            bufferSize);
//    }
//    else
//    {
//        return 0; // error
//    }
//}
//
//
//std::streamsize StreamUtils::copyToString(std::istream& istr,
//                                          std::string& str,
//                                          std::size_t bufferSize)
//{
//    return Poco::StreamCopier::copyToString(istr,
//                                            str,
//                                            bufferSize);
//}
//
//
//std::streamsize StreamUtils::copyTo(ResponseStream& responseStream,
//                                    std::ostream& ostr,
//                                    std::size_t bufferSize)
//{
//    if(responseStream.hasResponseStream())
//    {
//        return copyTo(responseStream.getResponseStreamRef(),
//                      ostr,
//                      bufferSize);
//    }
//    else
//    {
//        return 0; // error
//    }
//}
//    
//
//std::streamsize StreamUtils::copyTo(istream& istr,
//                                    ostream& ostr,
//                                    std::size_t bufferSize)
//{
//    return Poco::StreamCopier::copyStream(istr,
//                                          ostr,
//                                          bufferSize);
//}


} } // namespace ofx::HTTP
