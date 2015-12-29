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


#include <string>
#include "Poco/ErrorHandler.h"
#include "Poco/URI.h"
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
#include "ofx/HTTP/ThreadErrorHandler.h"
#include "ofx/HTTP/ServerEvents.h"
#include "ofx/HTTP/SessionStore.h"
#include "ofx/Net/IPAddressRange.h"


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


/// \brief This class mirrors Poco::Net::TCPServerParams, without ref counting.
class TCPServerParams
{
public:
    TCPServerParams();

    virtual ~TCPServerParams();

    void setThreadIdleTime(const Poco::Timespan& idleTime);
    const Poco::Timespan& getThreadIdleTime() const;
    void setMaxQueued(int count);
    int getMaxQueued() const;
    void setMaxThreads(int count);
    int getMaxThreads() const;
    void setThreadPriority(Poco::Thread::Priority prio);
    Poco::Thread::Priority getThreadPriority() const;

private:
    Poco::Timespan _threadIdleTime;
    int _maxThreads;
    int _maxQueued;
    Poco::Thread::Priority _threadPriority;

};


/// \brief This class mirrors Poco::Net::HTTPServerParams, without ref counting.
class HTTPServerParams: public TCPServerParams
{
public:
    HTTPServerParams();
    virtual ~HTTPServerParams();

    void setServerName(const std::string& serverName);
    const std::string& getServerName() const;
    void setSoftwareVersion(const std::string& softwareVersion);
    const std::string& getSoftwareVersion() const;
    void setTimeout(const Poco::Timespan& timeout);
    const Poco::Timespan& getTimeout() const;
    void setKeepAlive(bool keepAlive);
    bool getKeepAlive() const;
    void setKeepAliveTimeout(const Poco::Timespan& timeout);
    const Poco::Timespan& getKeepAliveTimeout() const;
    void setMaxKeepAliveRequests(int maxKeepAliveRequests);
    int getMaxKeepAliveRequests() const;

private:
    std::string _serverName;
    std::string _softwareVersion;
    Poco::Timespan _timeout;
    bool _keepAlive;
    int _maxKeepAliveRequests;
    Poco::Timespan _keepAliveTimeout;

};


class BaseServerSettings: public HTTPServerParams
{
public:
    BaseServerSettings(const std::string& host = DEFAULT_HOST,
                       uint16_t port = DEFAULT_PORT,
                       bool useSSL = DEFAULT_USE_SSL,
                       bool useSessions = DEFAULT_USE_SESSIONS);

    virtual ~BaseServerSettings();

    void setHost(const std::string& host);
    std::string getHost() const;

    void setPort(uint16_t port);
    uint16_t getPort() const;

    void setUseSSL(bool useSSL);
    bool useSSL() const;

    void setUseSessions(bool useSession);
    bool useSessions() const;

    Poco::URI getURI() const;


    const Net::IPAddressRange::List& getWhitelist() const;
    void setWhitelist(const Net::IPAddressRange::List& whitelist);

    const Net::IPAddressRange::List& getBlacklist() const;
    void setBlacklist(const Net::IPAddressRange::List& blacklist);
    
    const static std::string DEFAULT_HOST;
    const static uint16_t DEFAULT_PORT;
    const static bool DEFAULT_USE_SSL;
    const static bool DEFAULT_USE_SESSIONS;
    
private:
    std::string _host;
    uint16_t _port;
    bool _useSSL;
    bool _useSessions;
    
    Net::IPAddressRange::List _whitelist;
    Net::IPAddressRange::List _blacklist;

};


/// \brief The BaseServer class defines a default server.
///
/// The BaseServer template must be initialized with a settings type.
template <typename SettingsType, typename SessionStoreType>
class BaseServer_: public AbstractServer
{
public:
    BaseServer_(const SettingsType& settings = SettingsType(),
                Poco::ThreadPool& rThreadPool = Poco::ThreadPool::defaultPool()):
        _isSecurePort(false),
        _settings(settings),
        _rThreadPool(rThreadPool)
    {
        _defaultRoute.setServer(this);

        ofAddListener(ofEvents().exit, this, &BaseServer_::exit);
        Poco::Net::initializeSSL();

        pOldEH = Poco::ErrorHandler::set(&eh);
    }

    virtual ~BaseServer_()
    {
        stop();
        Poco::Net::uninitializeSSL();
        Poco::ErrorHandler::set(pOldEH);
    }
        
    void start()
    {
        if (isRunning())
        {
            ofLogWarning("BaseServer_::start") << "Server is already running.  Call stop() to stop.";
            return;
        }

        Poco::Net::ServerSocket socket;

        if (_settings.useSSL())
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
            _server = std::make_shared<Poco::Net::HTTPServer>(new BaseServerHandle(*this),
                                                              getThreadPool(),
                                                              socket,
                                                              _toPoco(_settings));

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


    void stop()
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

    void restart()
    {
        stop();
        start();
    }

    bool isRunning() const
    {
        return 0 != _server;
    }

    virtual void setup(const SettingsType& settings)
    {
        _settings = settings;

        if (isRunning())
        {
            ofLogWarning("BaseServer_::stop") << "Server running, restart to load new settings.";
        }
    }

    const SettingsType& getSettings() const
    {
        return _settings;
    }

    std::string getURL() const
    {
        return _settings.getURI().toString();
    }

    void addRoute(AbstractRoute* pRoute)
    {
        pRoute->setServer(this);
        _routes.push_back(pRoute);
    }

    void removeRoute(AbstractRoute* pRoute)
    {
        _routes.erase(std::remove(_routes.begin(), _routes.end(), pRoute), _routes.end());
        pRoute->setServer(0);
    }


    void onHTTPServerEvent(const void* pSender, ServerEventArgs& evt)
    {
        ofNotifyEvent(events.onHTTPServerEvent, evt, pSender);
    }


    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request)
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

    void exit(ofEventArgs&)
    {
        stop();
        ofRemoveListener(ofEvents().exit, this, &BaseServer_::exit);
    }

    SessionStoreType& getSessionStore()
    {
        return _sessionStore;
    }

    ServerEvents events;

protected:
    virtual Poco::ThreadPool& getThreadPool()
    {
        return _rThreadPool;
    }

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

    SessionStoreType _sessionStore;

    bool _isSecurePort;

    SettingsType _settings;

    Routes _routes;

    DefaultRoute _defaultRoute;

    bool acceptConnection(const Poco::Net::HTTPServerRequest& request)
    {
#if POCO_VERSION > 0x01050000
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
#endif

        return true;
    }

    Poco::ThreadPool& _rThreadPool;

    ThreadErrorHandler eh;
    Poco::ErrorHandler* pOldEH;

    Poco::Net::HTTPServerParams::Ptr _toPoco(const HTTPServerParams& params)
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
};


} } // namespace ofx::HTTP
