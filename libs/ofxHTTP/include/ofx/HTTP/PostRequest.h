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


#include "ofx/HTTP/BaseRequest.h"


namespace ofx {
namespace HTTP {


class PostRequest: public BaseRequest
{
public:
    struct FormPart
	{
		std::string name;
        Poco::Net::PartSource* pSource;
	};

    typedef std::vector<FormPart> FormParts;

    /// \brief An enumeration describing this POST's form encoding.
    enum FormEncoding
    {
        /// \brief Equivalent to application/x-www-form-urlencoded.
        FORM_ENCODING_URL,
        /// \brief Equivalent to multipart/form-data.
        FORM_ENCODING_MULTIPART
    };

    /// \brief Construct a PostRequest with a given uri and http version.
    /// \param uri the Post endpoint uri.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    PostRequest(const std::string& uri, const std::string& httpVersion);

    /// \brief Destroy the PostRequest.
    virtual ~PostRequest();

    void addFormPart(const FormPart& part);

    void addFormParts(const FormParts& parts);

    /// \brief Add a file for upload with this POST.
    /// \warning This method will sets the encoding to FORM_ENCODING_MULTIPART.
    /// \param name The form name of the attachment.
    /// \param path The path to the file.
    /// \param mediaType The mime type to send with the file.
    void addFormFile(const std::string& name,
                     const std::string& path,
                     const std::string& mediaType = DEFAULT_MEDIA_TYPE);

    /// \brief Add a buffer of bytes for upload with this POST.
    /// \warning This method will sets the encoding to FORM_ENCODING_MULTIPART.
    /// \param name The form name of the attachment.
    /// \param buffer The bytes to send.
    /// \param mediaType The mime type to send with the file.
    void addFormBuffer(const std::string& name,
                       const ofBuffer& buffer,
                       const std::string& mediaType = DEFAULT_MEDIA_TYPE);

    /// \brief Set the form's encoding.
    /// \warning If encoding is set to FORM_ENCODING_URL, files will not be
    ///     uploaded.
    /// \param formEncoding the encoding to pass with the post.
    void setFormEncoding(FormEncoding formEncoding);

    /// \returns the current form encoding.
    FormEncoding getFormEncoding() const;

protected:
    /// \brief Custom prepareRequest() to set the content length;
    void prepareRequest();

    /// \brief Writes from the form buffer.
    /// \param requestStream The stream to write the request body.
    void writeRequestBody(std::ostream& requestStream);

private:
    /// \brief A utility output buffer.
    std::stringstream _outBuffer;

};


} } // namespace ofx::HTTP
