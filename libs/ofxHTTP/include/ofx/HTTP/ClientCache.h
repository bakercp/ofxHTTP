//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include <map>
#include "Poco/Timestamp.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofx/HTTP/AbstractClientTypes.h"


namespace ofx {
namespace HTTP {


class ClientCacheEntry
{
public:


private:
    // static const std::string REQUEST_METHOD_HEADER_NAME = "Hc-Request-Method";
    Poco::Timestamp _requestDate;
    Poco::Timestamp _responseDate;
    int _statusCode;
    std::string _reasonForStatus;
    Poco::Net::NameValueCollection _responseHeaders;
    std::string _resource;
    std::map<std::string, std::string> _variantMap;
    Poco::Timestamp _date;

};



class SQLiteClientCacheStorage: public AbstractClientCacheStorage
{
public:
    virtual ~SQLiteClientCacheStorage();

    void put(const std::string& key, ClientCacheEntry value) override;

    ClientCacheEntry get(const std::string& key) override;

    void remove(const std::string& key) override;

    void update(const std::string& key) override;

};



class ClientCache
{
    public:
};



//// a validator is an "ETag" or "Last-modified" header
//// if there is no validator or and a lack of explicit freshness info, generally the element is uncachable.
//
//    {
//
//        Cache-control
//    };
//
////    https://www.keycdn.com/blog/http-cache-headers/
//
////    Cache-control
////    Every resource can define its own caching policy via the Cache-Control HTTP header.
////    Cache-Control directives control who caches the response, under what conditions and for how long.
//
//
//
////    Cache-control: private, max-age=0, no-cache
////    response directives are
//        // - Public vs. Private
//// public isn't always available, but can be inferred from other info like max-age
//
////    On the contrary, a response marked “private” can be cached (by the browser) but such responses are typically intended for single users hence they aren’t cacheable by intermediate caches (e.g. HTML pages with private user info can be cached by a user’s browser but not by a CDN).
////
//
//// No-cache - responses can't be used for subsequent requests before checking w/ the server to see if it's changed.
//
//    // if an etag is present, then it must be chcked, but the server can return a 304 (not modified)
//    // This means that the item can be cached, but it must be validated each time it is accessed.
//
//    // no-store is simpler.  simply, the data cannot be stored.  usually used for private information.
//
//
//    // max-age - the maximum time in seconds that the fetched responses are allowed to be used again.
//    //
//
////    s-maxage - used for CDNs (shared caches)
//
//// older is
//
////    pragma: no-cache --> is like cache-control: no-cache
//// Expires: --  It is however important to note that cache-control headers, max-age and s-maxage still take precedence on most modern systems, servers should set matching values for the sake of compatibility
//
////    To avoid breaking the specification, avoid setting the date value to more than a year.
//
//
//// ETag token automatically within the “If-None-Match” HTTP
//
////    a renewal for another 90 seconds
//
//// Validators
//
//// ETag
//    // 1.1+
//
//
////    If-Modified-Since
//
//
//








} } // namespace ofx::HTTP
