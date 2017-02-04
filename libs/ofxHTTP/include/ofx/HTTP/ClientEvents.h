//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Exception.h"
#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofx/HTTP/ClientState.h"
#include "ofx/HTTP/Progress.h"


namespace ofx {
namespace HTTP {


class Request;
class Response;
class Context;


class ClientContextEventArgs
{
public:
    ClientContextEventArgs(Context& context);

    virtual ~ClientContextEventArgs();

    const Context& context() const;

protected:
    Context& _context;
    
};



class ClientStateChangeEventArgs: public ClientContextEventArgs
{
public:
    using ClientContextEventArgs::ClientContextEventArgs;

    virtual ~ClientStateChangeEventArgs();

    ClientState state() const;

};


class ClientRequestEventArgs: public ClientContextEventArgs
{
public:
    ClientRequestEventArgs(Context& context,
                           Request& request);

    virtual ~ClientRequestEventArgs();

    const Request& request() const;

protected:
    Request& _request;

};


class ClientResponseEventArgs: public ClientRequestEventArgs
{
public:
    ClientResponseEventArgs(Context& context,
                            Request& request,
                            Response& response);

    virtual ~ClientResponseEventArgs();

    const Response& response() const;

protected:
    Response& _response;
    
};


/// \brief A class for progress event data.
class ClientRequestProgressEventArgs: public ClientRequestEventArgs
{
public:
    ClientRequestProgressEventArgs(Context& context,
                                   Request& request,
                                   Progress& progress);

    /// \brief Destroy the ClientRequestProgressEventArgs.
    virtual ~ClientRequestProgressEventArgs();

    const Progress& progress() const;

private:
    Progress& _progress;

};


/// \brief A class for progress event data.
class ClientResponseProgressEventArgs: public ClientResponseEventArgs
{
public:
    ClientResponseProgressEventArgs(Context& context,
                                    Request& request,
                                    Response& response,
                                    Progress& progress);

    /// \brief Destroy the ClientResponseProgressEventArgs.
    virtual ~ClientResponseProgressEventArgs();
    
    const Progress& progress() const;

private:
    Progress& _progress;

};


class ClientResponseStreamEventArgs: public ClientResponseEventArgs
{
public:
    using ClientResponseEventArgs::ClientResponseEventArgs;

    virtual ~ClientResponseStreamEventArgs();

    std::istream& stream();

};


class ClientErrorEventArgs: public ClientRequestEventArgs
{
public:
    ClientErrorEventArgs(Context& context,
                         Request& request,
                         Response* response,
                         const Poco::Exception& exception);

    virtual ~ClientErrorEventArgs();

    const Response* response() const;

    const Poco::Exception& exception() const;

protected:
    Response* _response = nullptr;

    const Poco::Exception& _exception;

};


class ClientEvents
{
public:
    ofEvent<ClientStateChangeEventArgs> onHTTPClientStateChange;
    ofEvent<ClientRequestProgressEventArgs> onHTTPClientRequestProgress;
    ofEvent<ClientResponseProgressEventArgs> onHTTPClientResponseProgress;
    ofEvent<ClientResponseStreamEventArgs> onHTTPClientResponseStream;
    ofEvent<ClientErrorEventArgs> onHTTPClientError;

};


} } // namespace ofx::HTTP
