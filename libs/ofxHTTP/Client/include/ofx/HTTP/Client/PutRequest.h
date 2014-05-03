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

#include "ofFileUtils.h"
#include "ofx/HTTP/Client/BaseRequest.h"


namespace ofx {
namespace HTTP {
namespace Client {


class PutRequest: public BaseRequest
{
public:
    /// \brief Construct a PutRequest with a given uri and http version.
    /// \param uri the Put endpoint uri.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    /// \param requestId A unique UUID for this request.
    PutRequest(const std::string& uri,
               const Poco::Net::NameValueCollection formFields = Poco::Net::NameValueCollection(),
               const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1,
               const Poco::UUID& requestId = generateUUID());

    virtual ~PutRequest();
    
    void setPutFile(const std::string& filename);

    void setPutBuffer(const ofBuffer& buffer);

    void setContentRange(std::size_t startByte);

    void setContentRange(std::size_t startByte,
                         std::size_t endByte);

    void setContentType(const std::string& contentType);

    /// \brief The default MIME type used for file parts.
    static const std::string DEFAULT_MEDIA_TYPE;

    virtual void prepareSubmit();
    
    virtual void writeRequestBody(std::ostream& requestStream);

protected:
    std::size_t _startByte;
    std::size_t _endByte;

    std::string _contentType;
    
    ofBuffer _buffer;

};

    
} } } // ofx::HTTP::Client
