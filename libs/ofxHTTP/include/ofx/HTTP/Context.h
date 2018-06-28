//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/Any.h"
#include "Poco/URI.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "ofEvents.h"
#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/ClientSessionSettings.h"
#include "ofx/HTTP/ClientState.h"
#include "ofx/HTTP/CookieStore.h"
#include "ofx/HTTP/CredentialStore.h"
#include "ofx/HTTP/Progress.h"

// a thread-safe shared context
// -- in theory all members are thread safe
// -- and no members are reentrant
// setters are not allowed for thread safety.
// if you want a context with a new store, then you have to make one
// via constructors

namespace ofx {
namespace HTTP {


class ClientEvents;


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

    /// \brief Set the client session to use.
    ///
    /// This is typically provided by the DefaultClientSessionProvider. The
    /// Context will take ownership of the HTTPClientSession.
    ///
    /// \param clientSession The client session to take ownership of.
    void setClientSession(std::unique_ptr<Poco::Net::HTTPClientSession> clientSession);

    /// \brief Release the client session.
    ///
    /// This is typically recalled by the client session provider to (possibly)
    /// be reused.
    ///
    /// \returns ownership of the client session via a unique_ptr.
    std::unique_ptr<Poco::Net::HTTPClientSession> releaseClientSession();

    /// \brief Get a pointer to the current HTTP session for this context.
    ///
    /// The context will retain ownership of the HTTP session.
    ///
    /// \returns a pointer to the current client session or nullptr if none set.
    Poco::Net::HTTPClientSession* clientSession();

    /// \brief Get a const pointer to the current HTTP session for this context.
    ///
    /// The context will retain ownership of the HTTP session.
    ///
    /// \returns a const pointer to the current client session or nullptr if none set.
    const Poco::Net::HTTPClientSession* clientSession() const;

    /// \brief Add to the history of redirects.
    /// \param uri The URI to add.
    void addRedirect(const Poco::URI& uri);

    /// \returns A list of all redirects followed by this client session.
    const std::vector<Poco::URI>& getRedirects() const;

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

    /// \returns true if the current transaction needs to be resubmitted.
    bool getResubmit() const;

    /// \returns the current state of the
    ClientState getState() const;

    void setState(ClientState state);

    /// \brief Client events.
    ClientEvents events;

private:
    /// \brief A copy of the client's session settings.
    ClientSessionSettings _sessionSettings;

    /// \brief The current client state.
    ClientState _state = ClientState::NONE;

    /// \brief A list of all redirects followed by this session.
    std::vector<Poco::URI> _redirects;

    /// \brief The URI that should be used if a proxy is required.
    /// \sa DefaultProxyProcessor::responseFilter.
    Poco::URI _proxyRedirectURI;

    /// \brief True if the current redirect should be pursued.
    bool _resubmit = false;

    /// \brief The client session that may be reused.
    std::unique_ptr<Poco::Net::HTTPClientSession> _clientSession = nullptr;

    std::unique_ptr<IO::FilteredInputStream> _responseStream = nullptr;

    friend class Client;
    friend class ClientSessionProvider;

};


} } // namespace ofx::HTTP
