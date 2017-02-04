//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
#include "ofTypes.h"
#include "ofx/HTTP/Cookie.h"
#include "ofx/IO/FilteredStreams.h"


namespace ofx {
namespace HTTP {


class Request;
class Response;
class Context;


/// \brief Represents an abstract request filter.
///
/// The implementation must be thread-safe, as multiple clients may call the
/// request filter methods.
class AbstractRequestFilter
{
public:
    virtual ~AbstractRequestFilter()
    {
    }

    virtual void requestFilter(Context& context,
                               Request& request) const = 0;

};


/// Some response filters will modify the request for resubmission, thus the
/// 
class AbstractResponseFilter
{
public:
    virtual ~AbstractResponseFilter()
    {
    }
    
    virtual void responseFilter(Context& context,
                                Request& request,
                                Response& response) const = 0;

};


class AbstractRequestResponseFilter:
    public AbstractRequestFilter,
    public AbstractResponseFilter
{
public:
    virtual ~AbstractRequestResponseFilter()
    {
    }

};


class AbstractRequestStreamFilter: public AbstractRequestFilter
{
public:
    virtual ~AbstractRequestStreamFilter()
    {
    }

    virtual void requestStreamFilter(Context& context,
                                     const Request& request,
                                     IO::FilteredOutputStream& requestStream) const = 0;
    
};


class AbstractResponseStreamFilter: public AbstractRequestResponseFilter
{
public:
    virtual ~AbstractResponseStreamFilter()
    {
    }

    virtual void responseStreamFilter(Context& context,
                                      const Request& request,
                                      const Response& response,
                                      IO::FilteredInputStream& responseStream) const = 0;

};


//class AbstractClientSessionProvider;
//
//
//class AbstractClientSession: public Poco::Net::HTTPClientSession
//{
//public:
//    virtual ~AbstractClientSession()
//    {
//    }
//
//    /// \returns the Abstract client session provider or nullptr if none exists.
//    virtual AbstractClientSessionProvider* parent() const;
//
//};


class AbstractClientSessionProvider: public AbstractRequestFilter
{
public:
    virtual ~AbstractClientSessionProvider()
    {
    }

    virtual void returnClientSession(std::unique_ptr<Poco::Net::HTTPClientSession> session) const = 0;
    
};


class ClientCacheEntry;


class AbstractClientCacheStorage
{
public:
    virtual ~AbstractClientCacheStorage()
    {
    }

    virtual void put(const std::string& key, ClientCacheEntry value) = 0;

    virtual ClientCacheEntry get(const std::string& key) = 0;

    virtual void remove(const std::string& key) = 0;

    virtual void update(const std::string& key) = 0;

};


} } // namespace ofx::HTTP
