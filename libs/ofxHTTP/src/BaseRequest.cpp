//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


BaseRequest::BaseRequest(const std::string& method,
                         const std::string& uri,
                         const std::string& httpVersion):
    Poco::Net::HTTPRequest(method, uri, httpVersion),
    _requestId(generateId())
{
}


BaseRequest::~BaseRequest()
{
}


void BaseRequest::write(std::ostream& ostr) const
{
    try
    {
        Poco::URI uri(getURI());

        std::string pathAndQuery = uri.getPathAndQuery();

        if (pathAndQuery.empty())
        {
            pathAndQuery = "/";
        }

        ostr << getMethod();
        // Here we use just path/query.
        ostr << " " << pathAndQuery;
        ostr << " " << getVersion();
        ostr << "\r\n";

        HTTPMessage::write(ostr);
        ostr << "\r\n";
    }
    catch (const Poco::SyntaxException& exc)
    {
        ofLogWarning("ofx::HTTP::BaseRequest") << "Unable to parse URI, using: " << getURI() << " : " << exc.displayText();
        Poco::Net::HTTPRequest::write(ostr);
    }
}


void BaseRequest::setRequestId(const std::string& requestId)
{
    _requestId = requestId;
}


const std::string& BaseRequest::getRequestId() const
{
    return _requestId;
}


std::string BaseRequest::generateId()
{
    return Poco::UUIDGenerator::defaultGenerator().createOne().toString();
}


const Poco::Net::HTMLForm& BaseRequest::form() const
{
    return _form;
}


void BaseRequest::prepareRequest()
{
    _form.prepareSubmit(*this);

    // Remove a trailing question mark appended by the prepareSubmit method that
    // can occur when dealing with query parameters passed with the URI rather
    // than the name value collection.
    std::string uri = getURI();

    if (!uri.empty() && '?' == uri[uri.size() - 1])
    {
        setURI(uri.substr(0, uri.size() - 1));
    }
}


void BaseRequest::writeRequestBody(std::ostream& requestStream)
{
    _form.write(requestStream);
}


int64_t BaseRequest::estimatedContentLength() const
{
    int64_t contentLength = getContentLength64();

    // If needed, try to calculate it based on the form data.
    if (contentLength == UNKNOWN_CONTENT_LENGTH)
    {
        // TODO: This long thing is a work-around for bug https://github.com/pocoproject/poco/issues/1337
        contentLength = (_form.getEncoding() == Poco::Net::HTMLForm::ENCODING_URL) ? 0 : _form.calculateContentLength();
    }

    return contentLength;
}


} } // namespace ofx::HTTP
