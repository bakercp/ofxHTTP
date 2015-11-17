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


#pragma once

#include "ofFileUtils.h"
#include "ofx/HTTP/BaseRequest.h"


namespace ofx {
namespace HTTP {


class PutRequest: public BaseRequest
{
public:
    /// \brief Construct a PutRequest with a given uri and http version.
    /// \param uri the Put endpoint uri.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    PutRequest(const std::string& uri,
               const std::string& httpVersion);

    virtual ~PutRequest();

    /// \brief Set the file to include in the body of the PUT request.
    /// \param filename The file to include in the body of the PUT request.
    void setPutFile(const std::string& filename);

    /// \brief Set the buffer to include in the body of the PUT request.
    /// \param buffer The buffer to include in the body of the PUT request.
    void setPutBuffer(const ofBuffer& buffer);

    /// \brief Set the start byte to include in the header of the PUT request.
    /// \param startByte The byte number representing the offset of the
    ///        data included in the body.  End byte is assumed to be the end
    ///        of the resource.
    void setContentRange(std::size_t startByte);

    /// \brief Set the start byte to include in the header of the PUT request.
    /// \param startByte The byte number representing the offset of the data
    ///        included in the body.
    /// \param endByte The byte number representing the end offset of the data
    ///        included in the body.
    /// \throws InvalidArgumentException if startByte is not less than or equal
    ///         to endByte.
    void setContentRange(std::size_t startByte,
                         std::size_t endByte);

    /// \brief Set the content type for the put request.
    ///
    /// The default content type is `application/octet-stream`.
    ///
    /// \contentType The content type of the PUT request.
    void setContentType(const std::string& contentType);

    /// \brief The default MIME type used for file parts.
    static const std::string DEFAULT_MEDIA_TYPE;

    virtual void prepareRequest() override;
    virtual void writeRequestBody(std::ostream& requestStream) override;

protected:
    std::size_t _startByte;
    
    std::size_t _endByte;

    std::string _contentType;
    
    ofBuffer _buffer;

};

    
} } // namespace ofx::HTTP
