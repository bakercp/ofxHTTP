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


#pragma once


#include "Poco/Exception.h"
#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofx/HTTP/ClientState.h"
#include "ofx/HTTP/Progress.h"


namespace ofx {
namespace HTTP {


class BaseRequest;
class BaseResponse;
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
                           BaseRequest& request);

    virtual ~ClientRequestEventArgs();

    const BaseRequest& request() const;

protected:
    BaseRequest& _request;

};


class ClientResponseEventArgs: public ClientRequestEventArgs
{
public:
    ClientResponseEventArgs(Context& context,
                            BaseRequest& request,
                            BaseResponse& response);

    virtual ~ClientResponseEventArgs();

    const BaseResponse& response() const;

protected:
    BaseResponse& _response;
    
};


/// \brief A class for progress event data.
class ClientRequestProgressEventArgs: public ClientRequestEventArgs
{
public:
    ClientRequestProgressEventArgs(Context& context,
                                   BaseRequest& request,
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
                                    BaseRequest& request,
                                    BaseResponse& response,
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
                         BaseRequest& request,
                         BaseResponse* response,
                         const Poco::Exception& exception);

    virtual ~ClientErrorEventArgs();

    const BaseResponse* response() const;

    const Poco::Exception& exception() const;

protected:
    BaseResponse* _response = nullptr;

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
