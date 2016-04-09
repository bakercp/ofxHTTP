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


#include <string>
#include "json.hpp"
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
    /// Query and fragment parameters included in the URI _will not_ be
    /// automatically available for additional processing (e.g. for use in OAuth
    /// 1.0 signatures, which require validation of the query/form parameters).
    /// Any query parameters added later using the `FormRequest::addFormField()`
    /// methods will be treated as either form fields or URI query parameters
    /// depending on the request's HTTP method.  Form fields added to POST and
    /// PUT requests will be submitted as requests POST and PUT (e.g. GET) will
    /// be URL-encoded and appended to the request URI before submission.
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

    /// \brief Set the id of the request.
    /// \param requestId the id of the request.
    void setRequestId(const std::string& requestId);

    /// \brief Get the id of the request.
    /// \returns the id of the request.
    const std::string& getRequestId() const;

    /// \brief Generate a UUID string.
    /// \returns a UUID string.
    static std::string generateId();

    /// \brief Get a const reference to the raw form data.
    ///
    /// This allows read-only access to the raw form data, which prevents
    /// inappropriately adding form parts to non POST / PUT requests.
    ///
    /// \returns a const reference to the raw form data.
    const Poco::Net::HTMLForm& form() const;

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
