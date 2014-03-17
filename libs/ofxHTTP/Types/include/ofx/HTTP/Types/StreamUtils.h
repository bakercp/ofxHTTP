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


#include "Poco/StreamCopier.h"
#include "ofConstants.h"
#include "ofx/HTTP/Client/ResponseStream.h"


namespace ofx {
namespace HTTP {
    

//class StreamUtils
//{
//public:
//    static std::streamsize consume(ResponseStream& responseStream,
//                                   std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
//
//    static std::streamsize consume(std::istream& istr,
//                                   std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
//    
//    static std::streamsize copyToString(ResponseStream& responseStream,
//                                        std::string& str,
//                                        std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
//
//    static std::streamsize copyToString(std::istream& istr,
//                                        std::string& str,
//                                        std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
//    
//    static std::streamsize copyTo(ResponseStream& responseStream,
//                                  std::ostream& ostr,
//                                  std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
//
//    static std::streamsize copyTo(std::istream& istr,
//                                  std::ostream& ostr,
//                                  std::size_t bufferSize = DEFAULT_BUFFER_SIZE);
//
//protected:
//    enum
//    {
//        DEFAULT_BUFFER_SIZE = 8192
//    };
//
//};

    
} } // namespace ofx::HTTP
