//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/JSONRequest.h"
#include "Poco/StreamCopier.h"


namespace ofx {
namespace HTTP {


const std::string JSONRequest::JSON_MEDIA_TYPE = "application/json";


JSONRequest::JSONRequest(const std::string& uri,
                         const std::string& httpVersion):
    JSONRequest(uri, ofJson(), httpVersion)
{
}


JSONRequest::JSONRequest(const std::string& uri,
                         const ofJson& json,
                         const std::string& httpVersion):
    Request(Poco::Net::HTTPRequest::HTTP_POST, uri, httpVersion),
    _json(json)
{
    setContentType(JSON_MEDIA_TYPE);
}


JSONRequest::~JSONRequest()
{
}


void JSONRequest::setJSON(const ofJson& json)
{
    _json = json;
}


const ofJson& JSONRequest::getJSON() const
{
    return _json;
}


void JSONRequest::prepareRequest()
{
    _outBuffer.clear();
    _outBuffer << getJSON(); // Call a subclass if needed.
    setContentLength(_outBuffer.str().length());
}


void JSONRequest::writeRequestBody(std::ostream& requestStream)
{
    Poco::StreamCopier::copyStream(_outBuffer, requestStream);
}


} } // namespace ofx::HTTP
