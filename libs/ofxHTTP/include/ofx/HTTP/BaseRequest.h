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


#include <string>
#include "ofTypes.h"
#include "ofLog.h"
#include "ofUtils.h"
#include "Poco/String.h"
#include "Poco/UTF8String.h"
#include "Poco/URI.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTMLForm.h"


namespace ofx {
namespace HTTP {


/// \brief A Custom Base HTTP Request.
class BaseRequest: public Poco::Net::HTTPRequest
{
public:
    /// \brief Construct an HTTPRequest
    ///
    /// This simple constructor passes the raw URI as is with the HTTPRequest.
    /// query and fragment parameters included in the URI will not be available
    /// for additional processing (e.g. for use in OAuth signatures).  Any
    /// query parameters / form fields added later using the addField() method
    /// will be processed separately according to the provided HTTP method (e.g.
    /// form fields added to a GET requests will be URL-encoded and added to the
    /// request URI before submission).
    ///
    /// \param method The HTTP method (e.g. GET, POST, PUT, etc).
    /// \param uri The endpoint URI.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    /// \throws Poco::SyntaxException if the uri is not valid.
    BaseRequest(const std::string& method,
                const std::string& uri,
                const std::string& httpVersion);

    /// \brief Destroy this BaseReuqest.
    virtual ~BaseRequest();

    /// \brief Write the HTTPRequest to an output stream.
    ///
    /// This method hides the method with the same signature from the base
    /// class. While this is not ideal, it allows us to rewrite the request
    /// in circumstances where the target servers do not support absolute
    /// URI paths.
    virtual void write(std::ostream& ostr) const;

    /// \brief Add a name value pair to upload with this request.
    /// \param name The field name.
    /// \param value The field value.
    void addFormField(const std::string& name,
                      const std::string& value);

    /// \brief Set a name value pair to upload with this request.
    /// \param name The field name.
    /// \param value The field value.
    void setFormField(const std::string& name,
                      const std::string& value);

    /// \brief Add additional form fields.
    /// \param formFields The form fields to add.
    void addFormFields(const Poco::Net::NameValueCollection& formFields);

    /// \brief Set the id of the request.
    /// \param requestId the id of the request.
    void setRequestId(const std::string& requestId);

    /// \brief Get the id of the request.
    /// \returns the id of the request.
    const std::string& getRequestId() const;

    /// \brief Get a const reference to the raw form data.
    /// \returns a const reference to the raw form data.
    const Poco::Net::HTMLForm& getForm() const;

    /// \brief Get a reference to the raw form data.
    /// \returns a reference to the raw form data.
    Poco::Net::HTMLForm& getForm();

    /// \brief Generate a UUID string.
    /// \returns a UUID string.
    static std::string generateId();

    /// \brief The default MIME type.
    static const std::string DEFAULT_MEDIA_TYPE;

protected:
    /// \brief Prepare the current request.
    virtual void prepareRequest();

    /// \brief Write the body of the request.
    /// \param requestStream The stream to write to.
    virtual void writeRequestBody(std::ostream& requestStream);

    /// \brief The original URI passed in during construction.
    ///
    /// During an HTTP session, the underlying HTTPRequest may modify or
    /// otherwise edit the internal URI to meet specifications.  This raw URI
    /// is stored unmodified for later reference.
    /// Poco::URI _rawURI;

    /// \brief A unique request id generated for this request.
    std::string _requestId;

    /// \brief A form with all query terms / form parameters.
    Poco::Net::HTMLForm _form;

    friend class BaseClient;

};


} } // namespace ofx::HTTP
