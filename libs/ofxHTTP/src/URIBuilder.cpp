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


#include "ofx/HTTP/URIBuilder.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


URIBuilder::URIBuilder()
{
}


URIBuilder::URIBuilder(const Poco::URI& uri):
    Poco::Net::NameValueCollection(HTTPUtils::getQueryMap(uri))
{
    _uri.setScheme(uri.getScheme());
    _uri.setAuthority(uri.getAuthority());
    _uri.setPath(uri.getPath());
    _uri.setFragment(uri.getFragment());
}


URIBuilder::~URIBuilder()
{
}


Poco::URI URIBuilder::toURI() const
{
    Poco::URI uri(_uri);
    uri.setRawQuery(HTTPUtils::makeQueryString(*this));
    return uri;
}


void URIBuilder::set(const std::string& key, bool value, bool numerical)
{
    if (numerical)
    {
        Poco::Net::NameValueCollection::set(key, value ? "1" : "0");
    }
    else
    {
        Poco::Net::NameValueCollection::set(key, value ? "true" : "false");
    }
}


void URIBuilder::add(const std::string& key, bool value, bool numerical)
{
    if (numerical)
    {
        Poco::Net::NameValueCollection::add(key, value ? "1" : "0");
    }
    else
    {
        Poco::Net::NameValueCollection::add(key, value ? "true" : "false");
    }
}


std::string URIBuilder::toString() const
{
    return toURI().toString();
}


} } // namespace ofx::HTTP
