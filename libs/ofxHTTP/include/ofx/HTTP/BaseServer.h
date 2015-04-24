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


#include <string>
#include "Poco/ErrorHandler.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/RWLock.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/Timespan.h"
#include "Poco/Version.h"
#include "ofEvents.h"
#include "ofSSLManager.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/BaseRoute.h"
#include "ofx/HTTP/BaseServerSettings.h"
#include "ofx/HTTP/BaseSessionManager.h"
#include "ofx/HTTP/ThreadErrorHandler.h"


namespace ofx {
namespace HTTP {


class BaseServerHandle: public Poco::Net::HTTPRequestHandlerFactory
{
public:
    BaseServerHandle(Poco::Net::HTTPRequestHandlerFactory& factory):
        _factory(factory)
    {
    }

    virtual ~BaseServerHandle()
    {
    }

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request)
    {
        return _factory.createRequestHandler(request);
    }

private:
    Poco::Net::HTTPRequestHandlerFactory& _factory;

};


/// \brief The BaseServer class defines a default server.
///
/// The BaseServer template must be initialized with a settings type.
template <typename SettingsType>
class BaseServer_: public AbstractServer
{
public:
    BaseServer_(const SettingsType& settings = SettingsType(),
                Poco::ThreadPool& rThreadPool = Poco::ThreadPool::defaultPool());

    virtual ~BaseServer_();
        
    void start();
    void stop();
    void restart();
    bool isRunning() const;

    virtual void setup(const SettingsType& settings);

    const SettingsType& getSettings() const;

    std::string getURL() const;

    void addRoute(AbstractRoute* pRoute);
    void removeRoute(AbstractRoute* pRoute);

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    void exit(ofEventArgs& args);

    virtual AbstractSessionManager& getSessionManager();

protected:
    virtual Poco::ThreadPool& getThreadPool();

    enum
    {
        DEFAULT_BACKLOG = 63
    };

private:
    BaseServer_(const BaseServer_&);
	BaseServer_& operator = (const BaseServer_&);

    typedef std::vector<AbstractRoute*> Routes;

    // TODO: replace w/ std::unique_ptr?
    // For some reason, shared pointers prevent hangups on close,
    // vs. using a raw pointer and delete in the (seemingly) correct location.
    std::shared_ptr<Poco::Net::HTTPServer> _server;

    DefaultSessionManager _sessionManager;

    bool _isSecurePort;

    SettingsType _settings;

    Routes _routes;

    DefaultRoute _defaultRoute;

    bool acceptConnection(const Poco::Net::HTTPServerRequest& request);

    Poco::ThreadPool& _rThreadPool;

    ThreadErrorHandler eh;
    Poco::ErrorHandler* pOldEH;

    Poco::Net::HTTPServerParams::Ptr _toPoco(const HTTPServerParams& params);

};


template <typename SettingsType>
BaseServer_<SettingsType>::BaseServer_(const SettingsType& settings,
                                       Poco::ThreadPool& rThreadPool):
    _isSecurePort(false),
    _settings(settings),
    _rThreadPool(rThreadPool)
{
    ofAddListener(ofEvents().exit, this, &BaseServer_<SettingsType>::exit);
    Poco::Net::initializeSSL();

    pOldEH = Poco::ErrorHandler::set(&eh);

}


template <typename SettingsType>
BaseServer_<SettingsType>::~BaseServer_()
{
    stop();
    Poco::Net::uninitializeSSL();

    Poco::ErrorHandler::set(pOldEH);
}


template <typename SettingsType>
void BaseServer_<SettingsType>::exit(ofEventArgs& args)
{
    stop();
    ofRemoveListener(ofEvents().exit, this, &BaseServer_<SettingsType>::exit);
}


template <typename SettingsType>
void BaseServer_<SettingsType>::start()
{
    if (isRunning())
    {
        ofLogWarning("BaseServer_::start") << "Server is already running.  Call stop() to stop.";
        return;
    }

    Poco::Net::ServerSocket socket;

    if (_settings.getUseSSL())
    {
        // we use the default thread pool
        try
        {
            socket = Poco::Net::SecureServerSocket(_settings.getPort(),
                                                   DEFAULT_BACKLOG,
                                                   ofSSLManager::getDefaultServerContext());

        }
        catch (const Poco::Exception& exc)
        {
            ofLogError("BaseServer_::start") << "Poco::Exception: " << exc.name() << " " << exc.what() << " " << exc.message();
        }
    }
    else
    {
        socket = Poco::Net::ServerSocket(_settings.getPort());
    }


    try
    {
        _server = std::shared_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(new BaseServerHandle(*this),
                                                                                   getThreadPool(),
                                                                                   socket,
                                                                                   _toPoco(_settings)));

        _isSecurePort = socket.secure();

    #if defined(TARGET_OSX)
        // essential on a mac!  fixed in 1.4.6p2+ / 1.5.2+
        socket.setOption(SOL_SOCKET, SO_NOSIGPIPE, 1); // ignore SIGPIPE
    #endif

        ofLogNotice("BaseServer_::start()") << "Starting server on port: " << _settings.getPort();

        // start the http server
        _server->start();
    }
    catch (const Poco::Net::InvalidSocketException& exc)
    {
        ofLogError("BaseServer_::start()") << exc.displayText();
    }

}


template <typename SettingsType>
void BaseServer_<SettingsType>::stop()
{
    if (!isRunning())
    {
        ofLogVerbose("BaseServer_::stop") << "Server is not running.  Call start() to start.";
        return;
    }

    Routes::const_reverse_iterator iter = _routes.rbegin();

    while (iter != _routes.rend())
    {
        (*iter)->stop();
        ++iter;
    }

    // Stop our default route.
    _defaultRoute.stop();

#if POCO_VERSION > 0x01040600
    _server->stopAll(true);
#else
    _server->stop();
#endif

    ofLogVerbose("BaseServer_::stop") << "getThreadPool().capacity() = " << getThreadPool().capacity();
    ofLogVerbose("BaseServer_::stop") << "getThreadPool().getStackSize() =  " << getThreadPool().getStackSize();
    ofLogVerbose("BaseServer_::stop") << "getThreadPool().used() = " << getThreadPool().used();
    ofLogVerbose("BaseServer_::stop") << "getThreadPool().allocated() = " << getThreadPool().allocated();
    ofLogVerbose("BaseServer_::stop") << "getThreadPool().available() = " << getThreadPool().available();

    // Wait for all threads in the thread pool.
    // Particularly troubling if we are sharing
    // this pool with other non-server-based-processes.
    // getThreadPool().joinAll();

    getThreadPool().stopAll();

    _server.reset();

    _isSecurePort = false;

}


template <typename SettingsType>
void BaseServer_<SettingsType>::restart()
{
    stop();
    start();
}


template <typename SettingsType>
std::string BaseServer_<SettingsType>::getURL() const
{
    return _settings.getURI().toString();
}


template <typename SettingsType>
void BaseServer_<SettingsType>::setup(const SettingsType& settings)
{
    _settings = settings;

    if (isRunning())
    {
        ofLogWarning("BaseServer_::stop") << "Server running, restart to load new settings.";
    }
}


template <typename SettingsType>
const SettingsType& BaseServer_<SettingsType>::getSettings() const
{
    return _settings;
}


template <typename SettingsType>
void BaseServer_<SettingsType>::addRoute(AbstractRoute* pRoute)
{
    pRoute->setServer(this);
    _routes.push_back(pRoute);
}


template <typename SettingsType>
void BaseServer_<SettingsType>::removeRoute(AbstractRoute* pRoute)
{
    _routes.erase(std::remove(_routes.begin(), _routes.end(), pRoute), _routes.end());
    pRoute->setServer(0);
}


template <typename SettingsType>
bool BaseServer_<SettingsType>::isRunning() const
{
    return 0 != _server;
}


template <typename SettingsType>
Poco::ThreadPool& BaseServer_<SettingsType>::getThreadPool()
{
    return _rThreadPool;
}


template <typename SettingsType>
bool BaseServer_<SettingsType>::acceptConnection(const Poco::Net::HTTPServerRequest& request)
{
    /*

    const Poco::Net::IPAddress& host = request.clientAddress().host();


    // If a whitelist is defined then you _must_ be on the whitelist.
    const Net::IPAddressRange::List& whitelist = _settings.getWhitelist();

    if (!whitelist.empty())
    {
        bool isWhitelisted = false;

        Net::IPAddressRange::List::const_iterator iter = whitelist.begin();

        while (iter != whitelist.end())
        {
            if (iter->contains(host))
            {
                return true;
            }

            ++iter;
        }

        if (!isWhitelisted)
        {
            return false;
        }
    }

    // If a blacklist is defined then you _must not_ be on the blacklist.
    const Net::IPAddressRange::List& blacklist = _settings.getBlacklist();

    if (!blacklist.empty())
    {
        Net::IPAddressRange::List::const_iterator iter = blacklist.begin();

        while (iter != blacklist.end())
        {
            if (iter->contains(request.clientAddress().host()))
            {
                return false;
            }
            
            ++iter;
        }
    }
     */

    return true;
}



template <typename SettingsType>
Poco::Net::HTTPRequestHandler* BaseServer_<SettingsType>::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    if (acceptConnection(request))
    {
        // We start with the last factory that was added.
        // Thus, factories with overlapping routes should be
        // carefully ordered.
        Routes::const_reverse_iterator iter = _routes.rbegin();

        while (iter != _routes.rend())
        {
            if ((*iter)->canHandleRequest(request, _isSecurePort))
            {
                return (*iter)->createRequestHandler(request);
            }

            ++iter;
        }
    }

    return _defaultRoute.createRequestHandler(request);
}


template<typename SettingsType>
AbstractSessionManager& BaseServer_<SettingsType>::getSessionManager()
{
    return _sessionManager;
}


template <typename SettingsType>
Poco::Net::HTTPServerParams::Ptr BaseServer_<SettingsType>::_toPoco(const HTTPServerParams& params)
{
    // hack!
    // all of these params are an attempt to make the server shut down VERY quickly.
    Poco::Net::HTTPServerParams::Ptr serverParams = new Poco::Net::HTTPServerParams();

    serverParams->setMaxQueued(params.getMaxQueued());

    // TODO: currently a bug (?) in void TCPServerParams::setMaxThreads(int count).
    // Poco::Net::HTTPServerParams::setMaxThreads() should be able to handle
    // a value of 0 (according to the documentation), but it is currently asserting
    // that the value must be > 0.  This is fixed in Poco 1.6+.
    if (params.getMaxThreads() <= 0)
    {
        serverParams->setMaxThreads(getThreadPool().capacity());
    }
    else
    {
        serverParams->setMaxThreads(params.getMaxThreads());
    }

    serverParams->setKeepAlive(params.getKeepAlive());
    serverParams->setMaxKeepAliveRequests(params.getMaxKeepAliveRequests());
    serverParams->setKeepAliveTimeout(params.getKeepAliveTimeout());
    serverParams->setServerName(params.getServerName());
    serverParams->setTimeout(params.getTimeout());
    serverParams->setThreadIdleTime(params.getThreadIdleTime());
    serverParams->setThreadPriority(params.getThreadPriority());
    serverParams->setSoftwareVersion(params.getSoftwareVersion());

    return serverParams;
}


} } // namespace ofx::HTTP
