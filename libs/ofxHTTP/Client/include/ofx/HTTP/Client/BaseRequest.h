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


#include <string>
#include "ofTypes.h"
#include "ofLog.h"
#include "ofFileUtils.h"
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
namespace Client {


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
    /// \param method the HTTP method (e.g. GET, POST, PUT, etc).
    /// \param uri the endpoint uri
    /// \param formFields a collection of form fields to be processed.
    /// \param verion Either HTTP/1.0 or HTTP/1.1.
    /// \param requestId A unique UUID for this request.
	BaseRequest(const std::string& method,
                const std::string& uri,
                const Poco::Net::NameValueCollection& formFields = Poco::Net::NameValueCollection(),
                const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1,
                const Poco::UUID& requestId = generateUUID());

    /// \brief Destroy this BaseReuqest.
    virtual ~BaseRequest();

    /// \brief Add a name value pair to upload with this POST.
    /// \param name The field name.
    /// \param value The field value.
    void addFormField(const std::string& name,
                      const std::string& value);

    void addFormFields(const Poco::Net::NameValueCollection& formFields);

    const Poco::UUID& getRequestId() const;

    const Poco::Net::HTMLForm& getForm() const;

    Poco::Net::HTMLForm& getFormRef();

    static Poco::UUID generateUUID();

    static const std::string DEFAULT_MEDIA_TYPE;
        ///< \brief The default MIME type.

protected:
    virtual void prepareRequest();

    virtual void writeRequestBody(std::ostream& requestStream);

    /// \brief A unique request id generated for this request.
    Poco::UUID _requestId;

    /// \brief A form with all query terms / form parameters.
    Poco::Net::HTMLForm _form;

    friend class BaseClient;

};


} } } // namespace ofx::HTTP::Client
