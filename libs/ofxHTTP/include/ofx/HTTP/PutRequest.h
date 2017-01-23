//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once

#include "ofFileUtils.h"
#include "ofx/HTTP/FormRequest.h"


namespace ofx {
namespace HTTP {


class PutRequest: public FormRequest
{
public:
    /// \brief Construct a PutRequest with a given uri and http version.
    /// \param uri the Put endpoint uri.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    PutRequest(const std::string& uri,
               const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1);

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

    virtual void prepareRequest() override;
    virtual void writeRequestBody(std::ostream& requestStream) override;

protected:
    std::size_t _startByte;
    
    std::size_t _endByte;

    std::string _contentType;
    
    ofBuffer _buffer;

};

    
} } // namespace ofx::HTTP
