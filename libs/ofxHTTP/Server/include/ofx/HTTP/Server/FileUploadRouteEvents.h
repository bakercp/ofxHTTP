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


#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofx/HTTP/Server/ServerEvents.h"


namespace ofx {
namespace HTTP {


class FileUploadEventArgs: public BaseServerEvent
{
public:
    FileUploadEventArgs(const std::string& fileName,
                   std::size_t fileSize,
                   std::size_t numBytesTransferred):
        _fileName(fileName),
        _fileSize(fileSize),
        _numBytesTransferred(numBytesTransferred)
    {
    }

    std::string getFileName() const
    {
        return _fileName;
    }

    std::size_t getFileSize()
    {
        return _fileSize;
    }

    std::size_t getNumBytesTransferred() const
    {
        return _numBytesTransferred;
    }

private:
    std::string _fileName;
    std::size_t _fileSize;
    std::size_t _numBytesTransferred;
};


class FileUploadRouteEvents
{
public:
    ofEvent<FileUploadEventArgs> onUploadStarted;
    ofEvent<FileUploadEventArgs> onUploadProgress;
    ofEvent<FileUploadEventArgs> onUploadFinished;

};


} } // namespace ofx::HTTP
