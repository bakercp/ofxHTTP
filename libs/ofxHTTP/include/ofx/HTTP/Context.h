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


/// \brief A Context is a collection of data that supports a client session.
///
/// This Context stores various attributes including session settings, redirects
/// cookie stores, etc.
class Context
{
public:
    /// \brief Create a default Context.
    Context();

    /// \brief Create a Context with the given settings.
    /// \param settings The ClientSessionSettings to use.
    Context(const ClientSessionSettings& sessionSettings);

    /// \brief Destroy the Context.
    virtual ~Context();

    /// \brief Set the ClientSessionSettings.
    /// \param settings The ClientSessionSettings to set.
    void setClientSessionSettings(const ClientSessionSettings& sessionSettings);

    /// \returns the ClientSessionSettings.
    const ClientSessionSettings& getClientSessionSettings() const;

//    void setCookieStore(CookieStore::SharedPtr cookieStore);
//    CookieStore::WeakPtr getCookieStore();

    /// \brief Set the client session to use.
    ///
    /// This is typically provided by the DefaultClientSessionProvider.
    ///
    /// \param clientSession The client session to use.
    void setClientSession(std::shared_ptr<Poco::Net::HTTPClientSession> clientSession);

    /// \returns the current client session, or nullptr if it is not set.
    std::shared_ptr<Poco::Net::HTTPClientSession> getClientSession();

    /// \brief Destroy the session to force a new session to be generated.
    void destroyClientSession();

    /// \brief Add to the history of redirects.
    /// \param uri The URI to add.
    void addRedirect(const Poco::URI& uri);

    /// \returns A list of all redurects followed by this client session.
    const std::vector<Poco::URI>& getRedirects() const;

//    void setResolvedURI(const Poco::URI& uri);
//    const Poco::URI& getResolvedURI() const;

    /// \brief Set the URI that should be used if a proxy is required.
    /// \param uri The proxy redirect URI.
    /// \sa DefaultProxyProcessor::responseFilter.
    void setProxyRedirectURI(const Poco::URI& uri);

    /// \returns the proxy redirect URI.
    /// \sa DefaultProxyProcessor::requestFilter
    const Poco::URI& getProxyRedirectURI() const;

    /// \brief Set whether the current session should be resubmitted.
    ///
    /// Resubmits can be set if request filters require it (e.g. Proxy filters).
    ///
    /// \param resubmit true if the associated request should be resubmitted.
    void setResubmit(bool resubmit);

    /// \brief
    bool getResubmit() const;

//    static const std::string KEY_PREFIX_RESERVED;
//    static const std::string KEY_SESSION_SETTINGS;
//    static const std::string KEY_COOKIE_STORE;
//    static const std::string KEY_CREDENTIAL_STORE;
//    static const std::string KEY_RESOLVED_URI;
//    static const std::string KEY_PROXY_REDIRECT_URI;
//    static const std::string KEY_REDIRECTS;
//    static const std::string KEY_SESSION;
//    static const std::string KEY_USE_ABSOLUTE_REQUEST_PATH;

//    template <typename TypeName>
//    bool getValue(const std::string& key, TypeName& value) const
//    {
//        auto iter = _map.find(key);
//
//        if (iter != _map.end())
//        {
//            try
//            {
//                value = Poco::AnyCast<TypeName>(iter->second);
//                return true;
//            }
//            catch (const Poco::BadCastException& exc)
//            {
//                ofLogError("Context::getValue") << "Unable to cast value for key : " << key;
//                return false;
//            }
//        }
//        else
//        {
//            return false;
//        }
//    }

private:
//    /// \brief A collection of additional session parameters.
//    std::map<std::string, Poco::Any> _map;

    /// \brief A copy of the client's session settings.
    ClientSessionSettings _sessionSettings;


//    CookieStore::WeakPtr _cookieStore;

    /// \brief A list of all redirects followed by this context.
    std::vector<Poco::URI> _redirects;

//    Poco::URI _resolvedURI;

    /// \brief The URI that should be used if a proxy is required.
    /// \sa DefaultProxyProcessor::responseFilter.
    Poco::URI _proxyRedirectURI;

    /// \brief The client session that may be reused.
    std::shared_ptr<Poco::Net::HTTPClientSession> _clientSession;

    /// \brief True if the current redirect should be pursued.
    bool _resubmit = false;

};


} } // namespace ofx::HTTP
