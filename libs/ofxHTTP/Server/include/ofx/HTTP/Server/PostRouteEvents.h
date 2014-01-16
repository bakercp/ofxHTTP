//// =============================================================================
////
//// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
////
//// Permission is hereby granted, free of charge, to any person obtaining a copy
//// of this software and associated documentation files (the "Software"), to deal
//// in the Software without restriction, including without limitation the rights
//// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//// copies of the Software, and to permit persons to whom the Software is
//// furnished to do so, subject to the following conditions:
////
//// The above copyright notice and this permission notice shall be included in
//// all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//// THE SOFTWARE.
////
//// =============================================================================
//
//
//#pragma once
//
//
//#include "Poco/Net/MediaType.h"
//#include "Poco/Net/NameValueCollection.h"
//#include "ofEvents.h"
//#include "ofFileUtils.h"
//#include "ofUtils.h"
//#include "ofx/HTTP/Server/ServerEvents.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
////class MultipartFormDataEventArgs: public BaseServerEvent
////{
////public:
////    FormDataEventArgs(const Headers& requestHeaders):
////        BaseServerEvent(requestHeaders)
////    {
////    }
////
////
////};
////
////
////class MultiPartDataEventArgs: public BaseServerEvent
////{
////public:
////    FormDataEventArgs(const Headers& requestHeaders):
////        BaseServerEvent(requestHeaders)
////    {
////    }
////
////    std::string getName() const
////    {
////        return _name;
////    }
////
////private:
////    Poco::Net::MediaType _mediaType;
////    std::string _name;
////
////};
////
////
////class FileFormDataEventArgs: public FormDataEventArgs
////{
////public:
////    FormDataEventArgs(Poco::Net::MediaType mediaType,
////                      std::string name):
////    _mediaType(mediaType),
////    _name(name)
////    {
////    }
////
////    Poco::Net::MediaType getMediaType() const
////    {
////        return _mediaType;
////    }
////
////    std::string getName() const
////    {
////        return _name;
////    }
////    
////private:
////    Poco::Net::MediaType _mediaType;
////    std::string _name;
////    
////};
////
////
////
//
//class PostEventArgs: public BaseServerEvent
//{
//public:
//    PostEventArgs(const std::string& fileName,
//                   std::size_t fileSize,
//                   std::size_t numBytesTransferred):
//        _fileName(fileName),
//        _fileSize(fileSize),
//        _numBytesTransferred(numBytesTransferred)
//    {
//    }
//
//    std::string getFileName() const
//    {
//        return _fileName;
//    }
//
//    std::size_t getFileSize()
//    {
//        return _fileSize;
//    }
//
//    std::size_t getNumBytesTransferred() const
//    {
//        return _numBytesTransferred;
//    }
//
//private:
//    std::string _fileName;
//    std::size_t _fileSize;
//    std::size_t _numBytesTransferred;
//};
//
//
//class PostRouteEvents
//{
//public:
//    ofEvent<PostEventArgs> onPost;
//
//};
//
//
//} } // namespace ofx::HTTP
