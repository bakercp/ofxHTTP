//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/HTTPHost.h"


namespace ofx {
namespace HTTP {


//class ClientSession: public AbstractClientSession
//{
//public:
//    ClientSession(const HTTPHost& host);
//
//    virtual ~ClientSession();
//
//    virtual AbstractClientSessionProvider* parent() const;
//
//private:
//    ClientSessionProvider* _parent = nullptr;
//
//};



/// \brief Set up a client session based on the requested URI scheme and host.
class ClientSessionProvider: public AbstractClientSessionProvider
{
public:
    ClientSessionProvider();

    virtual ~ClientSessionProvider();

    virtual void requestFilter(Context& context,
                               Request& request) const override;

    virtual void returnClientSession(std::unique_ptr<Poco::Net::HTTPClientSession> session) const override;

private:
    /// \brief Attempt to determine the host information from a request or context.
    static HTTPHost _extractHost(const Context& context,
                                 const Request& request);

    static HTTPHost _extractHost(const Poco::Net::HTTPClientSession& session);

    /// \brief Attempt to determine the can be used to contact the given host.
    static bool _isValidSessionForHost(const Poco::Net::HTTPClientSession& session,
                                       const HTTPHost& host);

    /// \brief Get session for the given host and context.
    void _getSessionForRequest(Context& context, const HTTPHost& host) const;

    /// \brief The session pool. It is effectively a cache, and thus mutable.
    mutable std::vector<std::unique_ptr<Poco::Net::HTTPClientSession>> _sessions;

    /// \brief The mutex for multi-threaded access.
    mutable std::mutex _mutex;

};


} } // namespace ofx::HTTP
