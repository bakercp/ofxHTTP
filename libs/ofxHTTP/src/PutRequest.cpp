//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/PutRequest.h"


namespace ofx {
namespace HTTP {


PutRequest::PutRequest(const std::string& uri,
                       const std::string& httpVersion):
    FormRequest(Poco::Net::HTTPRequest::HTTP_PUT,
                uri,
                httpVersion),
    _startByte(std::numeric_limits<std::size_t>::max()),
    _endByte(std::numeric_limits<std::size_t>::max()),
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
    _startByte = startByte;
}


void PutRequest::setContentRange(std::size_t startByte,
                                 std::size_t endByte)
{
    if (startByte < endByte)
    {
        _startByte = startByte;
        _startByte = endByte;
    }
    else
    {
        throw Poco::InvalidArgumentException("Start byte must be less than end byte.");
    }
}


void PutRequest::setContentType(const std::string& contentType)
{
    _contentType = contentType;
}


void PutRequest::prepareRequest()
{
    // The underlying _formÊdata is being ignored.
    set("Content-Length", ofToString(_buffer.size()));
}


void PutRequest::writeRequestBody(std::ostream& requestStream)
{
    // TODO don't read it all into memory.
    requestStream << _buffer;
}


} } // namespace ofx::HTTP
