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


#include "ofx/HTTP/JSONRequest.h"
#include "Poco/CountingStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/PartSource.h"
#include "Poco/Net/StringPartSource.h"


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
    BaseRequest(Poco::Net::HTTPRequest::HTTP_POST, uri, httpVersion),
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

    set("Content-Length", std::to_string(_outBuffer.str().length()));
}


void JSONRequest::writeRequestBody(std::ostream& requestStream)
{
    Poco::StreamCopier::copyStream(_outBuffer, requestStream);
}


} } // namespace ofx::HTTP
