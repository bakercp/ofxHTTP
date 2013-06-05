/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 =============================================================================*/


#pragma once


#include <set>
#include "Poco/RegularExpression.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/NetException.h"
#include "ServerExchange.h"
#include "Credentials.h"
#include "ResponseStream.h"
#include "Authentication.h"


namespace ofx {
namespace HTTP {


class BaseRouteSettings {
public:
    BaseRouteSettings(const std::string& route) : _route(route)
    {
    }

    virtual ~BaseRouteSettings()
    {
    }

    std::string getRoute() const
    {
        return _route;
    }

    void setRoute(const std::string& route)
    {
        _route = route;
    }

protected:
    std::string _route;

};


class BaseServerRoute : public Poco::Net::HTTPRequestHandlerFactory {
public:
    typedef ofPtr<BaseServerRoute> Ptr;
    
    BaseServerRoute()
    {
    }

    virtual ~BaseServerRoute()
    {
    }
    
    virtual bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                  bool bIsSecurePort) = 0;
    
//    virtual string getRoute() const = 0;
    
};

class BaseServerAuthenticationManager {
public:
    BaseServerAuthenticationManager()
    {
    }

    virtual ~BaseServerAuthenticationManager()
    {
    }
    
    // checkes the request for the correct authentication headers
    virtual Authentication::Status authenticate(Poco::Net::HTTPServerRequest& request) = 0;
    
    // writes the required authentication headers to the response
    virtual void setAuthenticationRequiredHeaders(Poco::Net::HTTPServerResponse& response)   = 0;
    
};

class BaseServerBasicAuthenticationManager : public BaseServerAuthenticationManager {
public:
    BaseServerBasicAuthenticationManager(const string& realm) : _realm(realm)
    {
    }

    virtual ~BaseServerBasicAuthenticationManager()
    {
    }
    
    Authentication::Status authenticate(Poco::Net::HTTPServerRequest& request)
    {
        if(request.hasCredentials()) {
            if(Poco::Net::HTTPCredentials::hasBasicCredentials(request)) {
                Poco::Net::HTTPBasicCredentials pCredentials(request);
                Credentials credentials(pCredentials.getUsername(),pCredentials.getPassword());
                if(checkCredentials(credentials)) {
                    return Authentication::OK;
                } else {
                    return Authentication::UNAUTHORIZED;
                }
            } else if(Poco::Net::HTTPCredentials::hasDigestCredentials(request)) {
                ofLogWarning("ofxBaseHTTPServerBasicAuthenticationManager::authenticate") << "HTTPServerResponse does not currently support DIGEST Auththentication";
                return Authentication::UNAUTHORIZED;
            } else {
                std::string scheme;
                std::string authInfo;

                try {
                    request.getCredentials(scheme, authInfo);
                } catch(const Poco::Net::NotAuthenticatedException& exc) {
                    ofLogWarning("ofxBaseHTTPServerBasicAuthenticationManager::authenticate") << "Unrecognizable authentication scheme.";
                    return Authentication::UNAUTHORIZED;
                }
                
                ofLogWarning("ofxBaseHTTPServerAuthenticationManager::authenticate") << "Unrecognized authentication scheme : " << scheme;
                return Authentication::UNAUTHORIZED;
            }
        } else {
            return Authentication::NO_CREDENTIALS;
        }
    }

    std::string getRealm()
    {
        return _realm;
    };
    
protected:
    std::string _realm;

    virtual bool checkCredentials(const Credentials& credentials) = 0;
    
    void setAuthenticationRequiredHeaders(Poco::Net::HTTPServerResponse& response) {
        // similar to response.requireAuthentication(realm),
        // but we want to send our own content after the
        // headers have been sent.
        string auth("Basic realm=\"");
        auth.append(_realm);
        auth.append("\"");
        response.set("WWW-Authenticate", auth);
    }
};

class BaseServerSessionManager {
public:
    BaseServerSessionManager()
    {
    }

    virtual ~BaseServerSessionManager()
    {
    }

    virtual bool update(Poco::Net::HTTPServerRequest& request,
                        Poco::Net::HTTPServerResponse& response) = 0;
};

class BaseResponseStreamConsumer {
public:
    BaseResponseStreamConsumer()
    {
    }

    virtual ~BaseResponseStreamConsumer()
    {
    }

    // the consumer subclass must free the response stream
    virtual void consume(ResponseStream* reponseStream) = 0;

};


} }
