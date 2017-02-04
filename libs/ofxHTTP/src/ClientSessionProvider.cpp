//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/ClientSessionProvider.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Response.h"
#include "ofx/HTTP/Context.h"
#include "ofSSLManager.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"


namespace ofx {
namespace HTTP {


//ClientSession::ClientSession(const HTTPHost& host):
//    AbstractClientSession(host.host(), host.port())
//{
//}
//
//
//ClientSession::~ClientSession()
//{
//}
//
//
//AbstractClientSessionProvider* ClientSession::parent() const
//{
//    return _parent;
//}
//


ClientSessionProvider::ClientSessionProvider()
{
}


ClientSessionProvider::~ClientSessionProvider()
{
}


void ClientSessionProvider::requestFilter(Context& context,
                                          Request& request) const
{
    ofLogVerbose("ClientSessionProvider::requestFilter") << "Session needed for " << request.getURI();

    // Get the host parameters in order to validate or create a client session.
    // This will throw exceptions if it is not possible to determine valid host
    // information.
    HTTPHost host = _extractHost(context, request);

    ofLogVerbose("ClientSessionProvider::requestFilter") << "Host extracted as " << host.toString();


    if (context.clientSession() != nullptr)
    {
        if (_isValidSessionForHost(*context.clientSession(), host))
        {
            if (context._responseStream != nullptr)
            {
                // A valid session is already in use and was resubmitted.
                // To re-use it, we must consume the existing response stream.
                HTTPUtils::consume(*context._responseStream);
            }

            ofLogVerbose("ClientSessionProvider::requestFilter") << "Using existing session matching " << host.toString();
            return;
        }
        else if (context.clientSession()->connected())
        {
            returnClientSession(context.releaseClientSession());
        }
        else
        {
            ofLogVerbose("ClientSessionProvider::requestFilter") << "The active session is no longer connected, so destroying it.";
            context.setClientSession(nullptr);
        }
    }

    // If we get to this point, we do not have a valid session because we
    // have not yet returned from this function.

    // So we must try to get an existing client session or make a new one.
    _getSessionForRequest(context, host);
}


void ClientSessionProvider::returnClientSession(std::unique_ptr<Poco::Net::HTTPClientSession> session) const
{
    ofLogVerbose("ClientSessionProvider::requestFilter") << "The active session is connected, but no longer valid, so returning it.";
    std::unique_lock<std::mutex> lock(_mutex);
    _sessions.push_back(std::move(session));
}


HTTPHost ClientSessionProvider::_extractHost(const Context& context,
                                             const Request& request)
{
    // Determine the scheme, host and port required for this request.
    Poco::URI uri = Poco::URI(request.getURI());

    std::string scheme = uri.getScheme();

    if (scheme.empty())
    {
        if (context.clientSession() != nullptr)
        {
            // Use the scheme from the existing session.
            // It may be possible that this is actually a ws or wss scheme,
            // but from an HTTPClientSession perspective, it doesn't matter.
            // The only place it might matter is to a request / response filter
            // but they will have to deal with that unlikely situation
            // themselves.
            if (context.clientSession()->secure())
            {
                scheme = "https";
            }
            else
            {
                scheme = "http";
            }
        }
        else
        {
            throw Poco::IOException("No scheme specified for request.");
        }
    }

    std::string host = uri.getHost();

    // If there is no host in the URI ...
    if (host.empty())
    {
        try
        {
            // ... then check the request headers.
            host = request.getHost();
        }
        catch (const Poco::NotFoundException& exc)
        {
            // If there aren't any, check the existing session ...
            if (context.clientSession() != nullptr && !context.clientSession()->getHost().empty())
            {
                host = context.clientSession()->getHost();
            }
            else
            {
                // ... otherwise, there is no way to know it.
                throw Poco::IOException("No host specified for request.");
            }
        }
    }

    uint16_t port = uri.getPort();

    // Validate port.
    if (port == 0)
    {
        // This means that the scheme must not be well-known, so check the
        // existing client session ...
        if (context.clientSession() != nullptr)
        {
            port = context.clientSession()->getPort();
        }
        else
        {
            // ... otherwise, there is no way to know it.
            throw Poco::IOException("No port can be determined for request.");
        }
    }

    return HTTPHost(scheme, host, port);
}


HTTPHost ClientSessionProvider::_extractHost(const Poco::Net::HTTPClientSession& session)
{
    return HTTPHost(session.secure() ? "https" : "http", session.getHost(), session.getPort());
}


bool ClientSessionProvider::_isValidSessionForHost(const Poco::Net::HTTPClientSession& session,
                                                   const HTTPHost& host)
{
    return host.secure() == session.secure()
        && host.host() == session.getHost()
        && host.port() == host.port();
}


void ClientSessionProvider::_getSessionForRequest(Context& context,
                                                  const HTTPHost& host) const
{
    ofLogVerbose("ClientSessionProvider::_getSessionForRequest") << "Getting session for " << host;

    {
        // Lock on access to the sessions.
        std::unique_lock<std::mutex> lock(_mutex);
        auto iter = _sessions.begin();

        while (iter != _sessions.end())
        {
            const auto& session = *iter->get();

            if (!session.connected())
            {
                ofLogVerbose("ClientSessionProvider::_getSessionForRequest") << "Deleting session " << _extractHost(session);
                iter = _sessions.erase(iter);
            }
            else if (_isValidSessionForHost(session, host))
            {
                ofLogVerbose("ClientSessionProvider::_getSessionForRequest") << "Found matching session " << _extractHost(session);
                context.setClientSession(std::move(*iter));
                iter = _sessions.erase(iter);
                return;
            }
            else
            {
                ++iter;
            }
        }
    }

    ofLogVerbose("ClientSessionProvider::_getSessionForRequest") << "No existing sessions to reuse.";

    if (host.secure())
    {
        ofSSLManager::initializeClient(); // Initialize SSL context if needed.
        // TODO: provide an opportunity to use an existing SSL context / session if desired.
        context.setClientSession(std::make_unique<Poco::Net::HTTPSClientSession>(host.host(), host.port()));
    }
    else
    {
        context.setClientSession(std::make_unique<Poco::Net::HTTPClientSession>(host.host(), host.port()));
    }

    // Apply session settings.
    context.clientSession()->setKeepAlive(context.getClientSessionSettings().getKeepAlive());
    context.clientSession()->setKeepAliveTimeout(context.getClientSessionSettings().getKeepAliveTimeout());
    context.clientSession()->setTimeout(context.getClientSessionSettings().getTimeout());

    ofLogVerbose("ClientSessionProvider::_getSessionForRequest") << "Created new session " << _extractHost(*context.clientSession());
}


} } // namespace ofx::HTTP
