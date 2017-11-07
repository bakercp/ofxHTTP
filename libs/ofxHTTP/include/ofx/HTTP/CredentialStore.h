//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <map>
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPAuthenticationParams.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPDigestCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/NetException.h"
#include "ofLog.h"
#include "ofx/HTTP/AuthScope.h"
#include "ofx/HTTP/Credentials.h"
#include "ofx/HTTP/HTTPUtils.h"
#include "ofx/HTTP/AbstractClientTypes.h"


namespace ofx {
namespace HTTP {


class Request;
class Response;
class Context;


/// \brief Default credential store.
class DefaultCredentialStore: public AbstractRequestResponseFilter
{
public:
    DefaultCredentialStore();
    virtual ~DefaultCredentialStore();

    bool hasCredentials(const AuthScope& targetScope) const;

    bool getCredentials(const AuthScope& targetScope,
                        AuthScope& matchingScope,
                        Credentials& matchingCredentials) const;

    void setCredentials(const AuthScope& scope, const Credentials& credentials);

    void setCredentialsFromURI(const Poco::URI& uri);

    void setCredentials(const Poco::URI& uri, const Credentials& credentials);

    void setCredentials(const Poco::URI& uri,
                        const std::string& username,
                        const std::string& password);

    void clearCredentials(const AuthScope& scope);

    void clearCredentials(const Poco::URI& uri);

    void requestFilter(Context& context,
                       Request& request) const override;

    void responseFilter(Context& context,
                        Request& request,
                        Response& response) const override;

//    bool canFilterResponse(Request& request,
//                           Response& response,
//                           Context& context) const;

    void clear();

protected:
    bool authenticateWithCache(const AuthScope& scope,
                               Poco::Net::HTTPRequest& pRequest);

    // Get credentials, with mutex already locked.
    bool getCredentialsWithExistingLock(const AuthScope& targetScope,
                                        AuthScope& matchingScope,
                                        Credentials& matchingCredentials) const;

private:
    DefaultCredentialStore(const DefaultCredentialStore&);
    DefaultCredentialStore& operator = (const DefaultCredentialStore&);

    typedef std::map<AuthScope, Credentials> CredentialMap;
    typedef std::map<AuthScope, std::shared_ptr<Poco::Net::HTTPBasicCredentials>> BasicCredentialCacheMap;
    typedef std::map<AuthScope, std::shared_ptr<Poco::Net::HTTPDigestCredentials>> DigestCredentialCacheMap;

//    typedef CredentialMap::const_iterator HTTPCredentialMapIter;
//    typedef BasicCredentialCacheMap::iterator HTTPBasicCredentialCacheMapIter;
//    typedef DigestCredentialCacheMap::iterator HTTPDigestCredentialCacheMapIter;

    CredentialMap credentialMap;
    mutable BasicCredentialCacheMap basicCredentialCacheMap;
    mutable DigestCredentialCacheMap digestCredentialCacheMap;

    mutable std::mutex _mutex;

};


} } // namespace ofx::HTTP
