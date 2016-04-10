// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/FormRequest.h"


namespace ofx {
namespace HTTP {


/// \brief A type for representing string and file HTML form parts.
class FormPart
{
public:
    /// \brief The type of the FormPart.
    enum class Type
    {
        /// \brief A string part.
        ///
        /// String parts can also represent encoded binary data (e.g. Base64-
        /// encoded images).
        STRING,

        /// \brief A file part.
        ///
        /// File FormParts include a name and a path toread and file to transmit.
        FILE
    };

    /// \brief Create a FormPart.
    /// \param type The FormPart type to create.
    /// \param name The part's form name.
    /// \param value The part's value. This is either a string value or a file path.
    /// \param mediaType The media type of the part (default "application/octet-stream").
    FormPart(Type type,
             const std::string& name,
             const std::string& value,
             const std::string& mediaType = "application/octet-stream"):
        _type(type),
        _name(name),
        _value(value),
        _mediaType(mediaType)
    {
    }

    /// \returns the FormPart type.
    Type type() const
    {
        return _type;
    }

    /// \returns the FormPart's form name.
    std::string name() const
    {
        return _name;
    }

    /// \returns the FormPart's value. This is either a string value or a file path.
    std::string value() const
    {
        return _value;
    }

    /// \returns the FormPart's media type.
    std::string mediaType() const
    {
        return _mediaType;
    }

private:
    /// \brief The form part type.
    Type _type;

    /// \brief The form name of the part.
    std::string _name;

    /// \brief The value, either a string to send or a file name.
    std::string _value;

    /// \brief The media type of the FormPart.
    std::string _mediaType;

};


/// \brief An HTTP POST Request.
class PostRequest: public FormRequest
{
public:
    /// \brief An enumeration describing this POST's form encoding.
    enum FormEncoding
    {
        /// \brief Equivalent to application/x-www-form-urlencoded.
        FORM_ENCODING_URL = 0,
        /// \brief Equivalent to multipart/form-data.
        FORM_ENCODING_MULTIPART = 1
    };

    /// \brief Construct a PostRequest with a given uri and http version.
    /// \param uri the Post endpoint uri.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    PostRequest(const std::string& uri,
                const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1);

    /// \brief Destroy the PostRequest.
    virtual ~PostRequest();

    /// \brief Add a form part.
    ///
    /// Setting a form part will automatically set the form encoding to
    /// FORM_ENCODING_MULTIPART.
    ///
    /// \param part The form part to add.
    void addFormPart(const FormPart& part);

    /// \brief Add form parts.
    ///
    /// Setting a form part will automatically set the form encoding to
    /// FORM_ENCODING_MULTIPART.
    ///
    /// \param parts The form parts to add.
    void addFormParts(const std::vector<FormPart>& parts);

    /// \brief Add a file for upload with this POST.
    ///
    /// Setting a form part will automatically set the form encoding to
    /// FORM_ENCODING_MULTIPART.
    ///
    /// \param name The form name of the attachment.
    /// \param path The path to the file.
    /// \param mediaType The mime type to send with the file.
    void addFormFile(const std::string& name,
                     const std::string& path,
                     const std::string& mediaType = DEFAULT_MEDIA_TYPE);

    /// \brief Add a string for upload with this POST.
    ///
    /// This string might be a Base64 or similar encoded buffer.
    ///
    /// Setting a form part will automatically set the form encoding to
    /// FORM_ENCODING_MULTIPART.
    ///
    /// \warning This method will sets the encoding to FORM_ENCODING_MULTIPART.
    /// \param name The form name of the attachment.
    /// \param buffer The bytes to send.
    /// \param mediaType The mime type to send with the file.
    void addFormString(const std::string& name,
                       const std::string& buffer,
                       const std::string& mediaType = DEFAULT_MEDIA_TYPE);

    /// \brief Set the form's encoding.
    ///
    /// \warning If encoding is set to FORM_ENCODING_URL after adding form parts,
    /// files will not be uploaded.
    ///
    /// \param formEncoding the encoding to pass with the post.
    void setFormEncoding(FormEncoding formEncoding);

    /// \returns the current form encoding.
    FormEncoding getFormEncoding() const;

    /// \brief Get a reference to the raw form data.
    /// \returns a reference to the raw form data.
    Poco::Net::HTMLForm& form();

protected:
    /// \brief Custom prepareRequest() to set the content length;
    void prepareRequest();

    /// \brief Writes from the form buffer.
    /// \param requestStream The stream to write the request body.
    void writeRequestBody(std::ostream& requestStream);

private:
    /// \brief A utility output buffer used for HTTP 1.0 connections.
    std::stringstream _outBuffer;

};


} } // namespace ofx::HTTP
