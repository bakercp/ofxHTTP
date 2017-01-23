//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
