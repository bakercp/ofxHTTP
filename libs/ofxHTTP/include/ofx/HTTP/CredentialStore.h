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


class BaseRequest;
class BaseResponse;
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
                        const string& username,
                        const string& password);
    
    void clearCredentials(const AuthScope& scope);

    void clearCredentials(const Poco::URI& uri);

    void requestFilter(BaseRequest& request,
                       Context& context);

    void responseFilter(BaseRequest& request,
                        BaseResponse& response,
                        Context& context);

    bool canFilterResponse(BaseRequest& request,
                           BaseResponse& response,
                           Context& context) const;

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

    typedef std::shared_ptr<Poco::Net::HTTPBasicCredentials>  HTTPBasicCredentialsSharedPtr;
    typedef std::shared_ptr<Poco::Net::HTTPDigestCredentials> HTTPDigestCredentialsSharedPtr;

    typedef std::map<AuthScope, Credentials>                    CredentialMap;
    typedef std::map<AuthScope, HTTPBasicCredentialsSharedPtr>  BasicCredentialCacheMap;
    typedef std::map<AuthScope, HTTPDigestCredentialsSharedPtr> DigestCredentialCacheMap;


    typedef CredentialMap::const_iterator      HTTPCredentialMapIter;
    typedef BasicCredentialCacheMap::iterator  HTTPBasicCredentialCacheMapIter;
    typedef DigestCredentialCacheMap::iterator HTTPDigestCredentialCacheMapIter;

    CredentialMap            credentialMap;
    BasicCredentialCacheMap  basicCredentialCacheMap;
    DigestCredentialCacheMap digestCredentialCacheMap;

    mutable std::mutex _mutex;
    
};


} } // namespace ofx::HTTP
