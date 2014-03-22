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


#include <map>
#include <string>
#include "Poco/URI.h"
#include "Poco/Net/HTMLForm.h"
//#include "Poco/Net/NameValueCollection.h"
#include "ofTypes.h"
#include "ofx/HTTP/Client/BaseRequest.h"
#include "ofFileUtils.h"

namespace ofx {
namespace HTTP {
namespace Client {


class PostRequest: public BaseRequest
{
public:
    /// \brief An enumeration describing this POST's form encoding.
    enum FormEncoding
    {
        FORM_ENCODING_URL,
            ///< \brief Equivalent to application/x-www-form-urlencoded.
        FORM_ENCODING_MULTIPART
            ///< \brief Equivalent to multipart/form-data.
    };

    /// \brief Construct a PostRequest with the given uri.
    /// \param uri the Post endpoint uri.
    PostRequest(const std::string& uri);

    /// \brief Construct a PostRequest with a given uri and http version.
    /// \param uri the Post endpoint uri.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    PostRequest(const std::string& uri,
                const std::string& httpVersion);

    /// \brief Destroy the PostRequest.
    virtual ~PostRequest();

    /// \brief Set the form's encoding.
    /// \warning If encoding is set to FORM_ENCODING_URL, files will not be
    ///     uploaded.
    /// \param formEncoding the encoding to pass with the post.
    void setFormEncoding(FormEncoding formEncoding);

    /// \returns the current form encoding.
    FormEncoding getFormEncoding() const;

    /// \brief Add a name value pair to upload with this POST.
    /// \param name The field name.
    /// \param value The field value.
    void addField(const std::string& name,
                  const std::string& value);

    /// \brief Add a file for upload with this POST.
    /// \warning This method will sets the encoding to FORM_ENCODING_MULTIPART.
    /// \param name The form name of the attachment.
    /// \param path The path to the file.
    /// \param mediaType The mime type to send with the file.
    void addFile(const std::string& name,
                 const std::string& path,
                 const std::string& mediaType = DEFAULT_MEDIA_TYPE);

    /// \brief Add a buffer of bytes for upload with this POST.
    /// \warning This method will sets the encoding to FORM_ENCODING_MULTIPART.
    /// \param name The form name of the attachment.
    /// \param buffer The bytes to send.
    /// \param mediaType The mime type to send with the file.
    void addBuffer(const std::string& name,
                   const ofBuffer& buffer,
                   const std::string& mediaType = DEFAULT_MEDIA_TYPE);

    /// \brief Clear all form data.
    void clear();

    /// \brief The default MIME type used for file parts.
    static const std::string DEFAULT_MEDIA_TYPE;

protected:
    virtual void finalizeRequest();
    virtual void writeRequestBody(std::ostream& requestStream);

    std::shared_ptr<Poco::Net::HTMLForm> _form;

};


} } } // ofx::HTTP::Request
