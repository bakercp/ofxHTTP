//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "ofConstants.h"
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
class Request: public Poco::Net::HTTPRequest
{
public:
    /// \brief Construct an Request
    ///
    /// This simple constructor passes the raw URI as is with the Request.
    /// Query and fragment parameters included in the URI _will not_ be
    /// automatically available for additional processing (e.g. for use in OAuth
    /// 1.0 signatures, which require validation of the query/form parameters).
    /// Any query parameters added later using the `FormRequest::addFormField()`
    /// methods will be treated as either form fields or URI query parameters
    /// depending on the request's HTTP method.  Form fields added to POST and
    /// PUT requests will be submitted as requests POST and PUT (e.g. GET) will
    /// be URL-encoded and appended to the request URI before submission.
    ///
    /// If the endpoint URI does not include a host name, the URI will be
    /// resolved using the given HTTPSessionClient.
    ///
    /// \param method The HTTP method (e.g. GET, POST, PUT, etc).
    /// \param uri The endpoint URI.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    /// \throws Poco::SyntaxException if the uri is not valid.
    Request(const std::string& method,
            const std::string& uri,
            const std::string& httpVersion);

    /// \brief Destroy this Request.
    virtual ~Request();

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

    /// \brief Get the estimated content length for progress estimation purposes.
    ///
    /// Internally this will read from the Content-Length header. If that header
    /// is not available due to transfer-chunking being true, etc, then it will
    /// query the form to calculate the length. Typically, it is not advised to
    /// call this function until after any form preparation has been done.  If
    /// it is not possible to calculate the content length,
    /// UNKNOWN_CONTENT_LENGTH will be returned.
    ///
    /// \returns the estimated content length or UNKNOWN_CONTENT_LENGTH if unknown.
    int64_t estimatedContentLength() const;

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
    ///
    /// This function is not const because the underlying form content length
    /// calculator is not const.
    mutable Poco::Net::HTMLForm _form;

    friend class BaseClient;
    friend class Client;

};


} } // namespace ofx::HTTP
