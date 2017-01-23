//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


//const std::string Context::KEY_PREFIX_RESERVED    = "HTTP_";
//const std::string Context::KEY_SESSION_SETTINGS   = "HTTP_SESSION_SETTINGS";
//const std::string Context::KEY_COOKIE_STORE       = "HTTP_COOKIE_STORE";
//const std::string Context::KEY_CREDENTIAL_STORE   = "HTTP_CREDENTIAL_STORE";
//const std::string Context::KEY_RESOLVED_URI       = "HTTP_RESOLVED_URI";
//const std::string Context::KEY_PROXY_REDIRECT_URI = "HTTP_PROXY_REDIRECT_URI";
//const std::string Context::KEY_REDIRECTS          = "HTTP_REDIRECTS";
//const std::string Context::KEY_SESSION            = "HTTP_SESSION";
//const std::string Context::KEY_USE_ABSOLUTE_REQUEST_PATH = "USE_ABSOLUTE_REQUEST_PATH";



Context::Context()
{
}


Context::Context(const ClientSessionSettings& sessionSettings):
    _sessionSettings(sessionSettings)
{
}


Context::~Context()
{
}


void Context::setClientSessionSettings(const ClientSessionSettings& sessionSettings)
{
    _sessionSettings = sessionSettings;
//    _requestStream = nullptr;
//    _responseStream = nullptr;
}


const ClientSessionSettings& Context::getClientSessionSettings() const
{
    return _sessionSettings;
}


void Context::setClientSession(std::unique_ptr<Poco::Net::HTTPClientSession> clientSession)
{
    _clientSession = std::move(clientSession);
}


Poco::Net::HTTPClientSession* Context::clientSession()
{
    return _clientSession.get();
}


const Poco::Net::HTTPClientSession* Context::clientSession() const
{
    return _clientSession.get();
}


std::unique_ptr<Poco::Net::HTTPClientSession> Context::releaseClientSession()
{
    // Moving sets _clientSession to nullptr ¤20.8.1/4.
    return std::move(_clientSession);
}


void Context::addRedirect(const Poco::URI& uri)
{
    _redirects.push_back(uri);
}


const std::vector<Poco::URI>& Context::getRedirects() const
{
    return _redirects;
}


void Context::setProxyRedirectURI(const Poco::URI& uri)
{
    _proxyRedirectURI = uri;
}


const Poco::URI& Context::getProxyRedirectURI() const
{
    return _proxyRedirectURI;
}


void Context::setResubmit(bool resubmit)
{
    _resubmit = resubmit;
}


bool Context::getResubmit() const
{
    return _resubmit;
}


ClientState Context::getState() const
{
    return _state;
}


void Context::setState(ClientState state)
{
    ofLogVerbose("Context::setState") << "Setting state: " << to_string(_state);
    _state = state;
    ClientStateChangeEventArgs args(*this);
    ofNotifyEvent(events.onHTTPClientStateChange, args, this);
}


} } // namespace ofx::HTTP
