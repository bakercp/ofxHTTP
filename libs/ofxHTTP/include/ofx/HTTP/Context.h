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


#include "Poco/Any.h"
#include "Poco/URI.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "ofx/HTTP/CookieStore.h"
#include "ofx/HTTP/CredentialStore.h"
#include "ofx/HTTP/ClientSessionSettings.h"

// a thread-safe shared context
// -- in theory all members are thread safe
// -- and no members are reentrant
// setters are not allowed for thread safety.
// if you want a context with a new store, then you have to make one
// via constructors

namespace ofx {
namespace HTTP {


class Context
{
public:
    typedef std::shared_ptr<Poco::Net::HTTPClientSession> ClientSession;

    Context();
    virtual ~Context();
   
    void setClientSessionSettings(const ClientSessionSettings& settings);
    const ClientSessionSettings& getClientSessionSettings() const;

//    void setCookieStore(CookieStore::SharedPtr cookieStore);
//    CookieStore::WeakPtr getCookieStore();

    void setClientSession(ClientSession clientSession);
    ClientSession& getClientSession();

    void addRedirect(const Poco::URI& uri);
    const std::vector<Poco::URI>& getRedirects() const;

//    void setResolvedURI(const Poco::URI& uri);
//    const Poco::URI& getResolvedURI() const;

    void setProxyRedirectURI(const Poco::URI& uri);
    const Poco::URI& getProxyRedirectURI() const;

    bool getResubmit() const;
    void setResubmit(bool resubmit);

    const static std::string KEY_PREFIX_RESERVED;
    const static std::string KEY_SESSION_SETTINGS;
    const static std::string KEY_COOKIE_STORE;
    const static std::string KEY_CREDENTIAL_STORE;
    const static std::string KEY_RESOLVED_URI;
    const static std::string KEY_PROXY_REDIRECT_URI;
    const static std::string KEY_REDIRECTS;
    const static std::string KEY_SESSION;
    const static std::string KEY_USE_ABSOLUTE_REQUEST_PATH;

    template <typename TypeName>
    bool getValue(const std::string& key, TypeName& value) const
    {
        std::map<std::string, Poco::Any>::const_iterator iter = _map.find(key);

        if (iter != _map.end())
        {
            try
            {
                value = Poco::AnyCast<TypeName>(iter->second);
                return true;
            }
            catch (Poco::BadCastException& exc)
            {
                ofLogError("Context::getValue") << "Unable to cast value for key : " << key;
                return false;
            }
        }
        else
        {
            return false;
        }
    }

private:
    std::map<std::string, Poco::Any> _map;

    ClientSessionSettings _clientSessionSettings;
//    CookieStore::WeakPtr _cookieStore;

    std::vector<Poco::URI> _redirects;

//    Poco::URI _resolvedURI;
    Poco::URI _proxyRedirectURI;

    ClientSession _clientSession;

    bool _resubmit;

};


} } // namespace ofx::HTTP
