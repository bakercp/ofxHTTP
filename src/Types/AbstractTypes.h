// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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


#include <set>
#include "Poco/RegularExpression.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/NetException.h"
#include "Authentication.h"
#include "Credentials.h"
#include "ResponseStream.h"
#include "ServerExchange.h"


namespace ofx {
namespace HTTP {


using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;


class AbstractHasRoute {
public:
    AbstractHasRoute()
    {
    }

    virtual ~AbstractHasRoute()
    {
    }

    virtual std::string getRoute() const = 0;

};

class AbstractServerRoute : public HTTPRequestHandlerFactory {
public:
    typedef ofPtr<AbstractServerRoute> Ptr;

    AbstractServerRoute()
    {
    }

    virtual ~AbstractServerRoute()
    {
    }
    
    virtual bool canHandleRequest(const HTTPServerRequest& request,
                                  bool bIsSecurePort) = 0;

};


class AbstractServerRouteHandler : public HTTPRequestHandler {
public:
    AbstractServerRouteHandler()
    {
    }

    virtual ~AbstractServerRouteHandler()
    {
    }

protected:
    // authenticate is expected to check authentication and also
    // set any response headers if authentication has failed.
    virtual Authentication::Status authenticate(ServerExchange& exchange) = 0;
    virtual void updateSession(ServerExchange& exchange) = 0;
    virtual void handleExchange(ServerExchange& exchange) = 0;
    virtual void sendErrorResponse(HTTPServerResponse& response) = 0;

};


class AbstractServerSessionData {
public:
    AbstractServerSessionData()
    {
    }

    virtual ~AbstractServerSessionData()
    {
    }

    virtual bool update(HTTPServerRequest& request,
                        HTTPServerResponse& response) = 0;
    
};


class AbstractBasicCredentialsAuthenticator {
public:
    AbstractBasicCredentialsAuthenticator()
    {
    }

    virtual ~AbstractBasicCredentialsAuthenticator()
    {
    }

    virtual bool isAuthenticated(const Credentials& credentials) = 0;
};


class AbstractServerAuthenticationManager {
public:
    AbstractServerAuthenticationManager()
    {
    }

    virtual ~AbstractServerAuthenticationManager()
    {
    }
    
    // checks the request for the correct authentication headers
    virtual Authentication::Status authenticate(HTTPServerRequest& request) = 0;
    
    // writes the required authentication headers to the response
    virtual void setAuthenticationRequiredHeaders(HTTPServerResponse& response) = 0;
    
};


class AbstractServerSessionManager {
public:
    AbstractServerSessionManager()
    {
    }

    virtual ~AbstractServerSessionManager()
    {
    }

    virtual bool update(HTTPServerRequest& request,
                        HTTPServerResponse& response) = 0;
};

class AbstractResponseStreamConsumer {
public:
    AbstractResponseStreamConsumer()
    {
    }

    virtual ~AbstractResponseStreamConsumer()
    {
    }

    // the consumer subclass must free the response stream
    virtual void consume(ResponseStream* reponseStream) = 0;

};


} } // namespace ofx::HTTP
