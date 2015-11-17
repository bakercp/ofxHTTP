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


#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


const std::string BaseRequest::DEFAULT_MEDIA_TYPE = "application/octet-stream";


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
        ofLogWarning("ofx::HTTP::BaseRequest") << "Unable to parse URI, using: " << getURI();
        Poco::Net::HTTPRequest::write(ostr);
    }
}


void BaseRequest::addFormFields(const Poco::Net::NameValueCollection& formFields)
{
    Poco::Net::NameValueCollection::ConstIterator iter = formFields.begin();

    while (iter != formFields.end())
    {
        _form.add(iter->first, iter->second);
        ++iter;
    }
}


void BaseRequest::addFormField(const std::string& name,
                               const std::string& value)
{
    _form.add(name, value);
}


void BaseRequest::setFormField(const std::string& name,
                               const std::string& value)
{
    _form.set(name, value);
}


void BaseRequest::setRequestId(const std::string& requestId)
{
    _requestId = requestId;
}


const std::string& BaseRequest::getRequestId() const
{
    return _requestId;
}


const Poco::Net::HTMLForm& BaseRequest::getForm() const
{
    return _form;
}


Poco::Net::HTMLForm& BaseRequest::getForm()
{
    return _form;
}


std::string BaseRequest::generateId()
{
    return Poco::UUIDGenerator::defaultGenerator().createOne().toString();
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


} } // namespace ofx::HTTP
