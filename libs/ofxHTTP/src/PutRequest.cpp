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


#include "ofx/HTTP/PutRequest.h"


namespace ofx {
namespace HTTP {


const std::string PutRequest::DEFAULT_MEDIA_TYPE = "application/octet-stream";



PutRequest::PutRequest(const std::string& uri,
                       const Poco::Net::NameValueCollection formFields,
                       const std::string& httpVersion,
                       const Poco::UUID& requestId):
    BaseRequest(Poco::Net::HTTPRequest::HTTP_PUT,
                uri,
                formFields,
                httpVersion,
                requestId),
    _startByte(0),
    _endByte(0),
    _contentType(DEFAULT_MEDIA_TYPE)
{
}
    

PutRequest::~PutRequest()
{
}


void PutRequest::setPutFile(const std::string& filename)
{
    _buffer = ofBufferFromFile(filename);
}


void PutRequest::setPutBuffer(const ofBuffer& buffer)
{
    _buffer = buffer;
}


void PutRequest::setContentRange(std::size_t startByte)
{

}

void PutRequest::setContentRange(std::size_t startByte,
                                 std::size_t endByte)
{

}

void PutRequest::setContentType(const std::string& contentType)
{
    _contentType = contentType;
}


void PutRequest::prepareSubmit()
{

}

void PutRequest::writeRequestBody(std::ostream& requestStream)
{
    requestStream << _buffer;
}


} } // namespace ofx::HTTP
