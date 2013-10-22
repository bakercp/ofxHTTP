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
#include "Poco/RWLock.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/Timespan.h"
#include "Poco/Version.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "ofEvents.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/Server/BaseRoute.h"
#include "ofx/HTTP/Server/BaseRouteHandler.h"
#include "ofx/HTTP/Server/BaseServerSettings.h"
#include "ofx/HTTP/Types/ThreadErrorHandler.h"


namespace ofx {
namespace HTTP {


class MyErrorHandler: public Poco::ErrorHandler
{
public:
    void exception(const Poco::Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
    }
    void exception(const std::exception& exc)
    {
        std::cerr << exc.what() << std::endl;
    }
    void exception()
    {
        std::cerr << "unknown exception" << std::endl;
    }
};


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


template <typename SettingsType>
class BaseServer_:
    public Poco::Net::HTTPRequestHandlerFactory
{
public:
    typedef std::shared_ptr<BaseServer_<SettingsType> > SharedPtr;
    typedef std::weak_ptr<BaseServer_<SettingsType> >   WeakPtr;

    BaseServer_(const SettingsType& settings = SettingsType(),
                Poco::ThreadPool& threadPoolRef = Poco::ThreadPool::defaultPool());

    virtual ~BaseServer_();
        
    void start();
    void stop();
    bool isRunning() const;

    const SettingsType& getSettings() const;
    std::string getURL() const;

    void addRoute(AbstractRoute::SharedPtr route);
    void removeRoute(AbstractRoute::SharedPtr route);

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    void exit(ofEventArgs& args);

protected:
    virtual Poco::ThreadPool& getThreadPoolRef();

private:
    typedef std::shared_ptr<Poco::Net::HTTPServer> HTTPServerPtr;
    typedef std::vector<AbstractRoute::SharedPtr> Routes;

    BaseServer_(const BaseServer_&);
	BaseServer_& operator = (const BaseServer_&);

    // TODO: replace w/ std::unique_ptr?
    HTTPServerPtr _server;

    bool _isSecurePort;

    SettingsType _settings;

    Routes _routes;

    BaseRoute _baseRoute;

    Poco::Net::HTTPServerParams::Ptr getPocoHTTPServerParams(const HTTPServerParams& params);

    Poco::ThreadPool& _threadPoolRef;

    MyErrorHandler eh;
    Poco::ErrorHandler* pOldEH;

};


typedef BaseServer_<BaseServerSettings> BaseServer;


//------------------------------------------------------------------------------
template <typename SettingsType>
BaseServer_<SettingsType>::BaseServer_(const SettingsType& settings,
                                       Poco::ThreadPool& threadPoolRef):
    _settings(settings),
    _threadPoolRef(threadPoolRef), 
    _isSecurePort(false)
{
    ofAddListener(ofEvents().exit, this, &BaseServer_<SettingsType>::exit);
    Poco::Net::initializeSSL();

    pOldEH = Poco::ErrorHandler::set(&eh);

}

//------------------------------------------------------------------------------
template <typename SettingsType>
BaseServer_<SettingsType>::~BaseServer_()
{
    stop();
    Poco::Net::uninitializeSSL();

    Poco::ErrorHandler::set(pOldEH);
}

//------------------------------------------------------------------------------
template <typename SettingsType>
void BaseServer_<SettingsType>::exit(ofEventArgs& args)
{
    stop();
    ofRemoveListener(ofEvents().exit, this, &BaseServer_<SettingsType>::exit);
}

//------------------------------------------------------------------------------
template <typename SettingsType>
void BaseServer_<SettingsType>::start()
{
    if(isRunning())
    {
        ofLogWarning("BaseServer_::start") << "Server is already running.  Call stop() to stop.";
        return;
    }

    Poco::Net::ServerSocket socket;

    if(_settings.getUseSSL())
    {
        // we use the default thread pool
        try
        {
            // TODO: refactor and allow configuration
            Poco::Net::Context::Ptr context(new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
                                                                   "",
                                                                   Poco::Net::Context::VERIFY_RELAXED,
                                                                   9,
                                                                   true,
                                                                   "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));

            socket = Poco::Net::SecureServerSocket(_settings.getPort(),
                                                   64,
                                                   context);
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


    _server = HTTPServerPtr(new Poco::Net::HTTPServer(new BaseServerHandle(*this),
                                                      getThreadPoolRef(),
                                                      socket,
                                                      getPocoHTTPServerParams(_settings)));

    _isSecurePort = socket.secure();

#if defined(TARGET_OSX)
    // essential on a mac!  fixed in 1.4.6p2+ / 1.5.2+
    socket.setOption(SOL_SOCKET, SO_NOSIGPIPE, 1); // ignore SIGPIPE
#endif

    // start the http server
    _server->start();

}

//------------------------------------------------------------------------------
template <typename SettingsType>
void BaseServer_<SettingsType>::stop()
{
    if(!isRunning())
    {
        ofLogWarning("BaseServer_::stop") << "Server is not running.  Call start() to start.";
        return;
    }

    Routes::const_reverse_iterator iter = _routes.rbegin();
    while(iter != _routes.rend())
    {
        (*iter)->stop();
        ++iter;
    }

    _baseRoute.stop();

#if POCO_VERSION > 0x01040600
    _server->stopAll(true);
#else
    _server->stop();
#endif

    ofLogVerbose("BaseServer_<SettingsType>::stop") << "getThreadPoolRef().capacity() = " << getThreadPoolRef().capacity();
    ofLogVerbose("BaseServer_<SettingsType>::stop") << "getThreadPoolRef().getStackSize() =  " << getThreadPoolRef().getStackSize();
    ofLogVerbose("BaseServer_<SettingsType>::stop") << "getThreadPoolRef().used() = " << getThreadPoolRef().used();
    ofLogVerbose("BaseServer_<SettingsType>::stop") << "getThreadPoolRef().allocated() = " << getThreadPoolRef().allocated();
    ofLogVerbose("BaseServer_<SettingsType>::stop") << "getThreadPoolRef().available() = " << getThreadPoolRef().available();

    // wait for all threads in the thread pool
    // we gotta wait for all of them ... ugh.
    // Particularly troubling if we are sharing
    // this pool with other non-server-based-processes.
    // getThreadPoolRef().joinAll();

    getThreadPoolRef().stopAll(); // at least there's a chance of shutting down

    _server.reset();

    _isSecurePort = false;

}

//------------------------------------------------------------------------------
template <typename SettingsType>
std::string BaseServer_<SettingsType>::getURL() const
{
    return _settings.getURI().toString();
}

//------------------------------------------------------------------------------
template <typename SettingsType>
const SettingsType& BaseServer_<SettingsType>::getSettings() const
{
    return _settings;
}

//------------------------------------------------------------------------------
template <typename SettingsType>
void BaseServer_<SettingsType>::addRoute(AbstractRoute::SharedPtr route)
{
    _routes.push_back(route);
}

//------------------------------------------------------------------------------
template <typename SettingsType>
void BaseServer_<SettingsType>::removeRoute(AbstractRoute::SharedPtr route)
{
    _routes.erase(std::remove(_routes.begin(),_routes.end(),route),_routes.end());
}

//------------------------------------------------------------------------------
template <typename SettingsType>
bool BaseServer_<SettingsType>::isRunning() const
{
    return 0 != _server;
}

//------------------------------------------------------------------------------
template <typename SettingsType>
Poco::ThreadPool& BaseServer_<SettingsType>::getThreadPoolRef()
{
    return _threadPoolRef;
}

//------------------------------------------------------------------------------
template <typename SettingsType>
Poco::Net::HTTPRequestHandler* BaseServer_<SettingsType>::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    // We start with the last factory that was added.
    // Thus, factories with overlapping routes should be
    // carefully ordered.
    Routes::const_reverse_iterator iter = _routes.rbegin();
    while(iter != _routes.rend())
    {
        if((*iter)->canHandleRequest(request,_isSecurePort))
        {
            return (*iter)->createRequestHandler(request);
        }
        ++iter;
    }

    return _baseRoute.createRequestHandler(request);
}

//------------------------------------------------------------------------------
template <typename SettingsType>
Poco::Net::HTTPServerParams::Ptr BaseServer_<SettingsType>::getPocoHTTPServerParams(const HTTPServerParams& params)
{
    // hack!
    // all of these params are an attempt to make the server shut down VERY quickly.
    Poco::Net::HTTPServerParams::Ptr serverParams = new Poco::Net::HTTPServerParams();

    serverParams->setMaxQueued(params.getMaxQueued());

    // TODO: currently a bug (?) in void TCPServerParams::setMaxThreads(int count).
    // Poco::Net::HTTPServerParams::setMaxThreads() should be able to handle
    // a value of 0 (according to the documentation), but it is currently asserting
    // that the value must be > 0.
    if(params.getMaxThreads() <= 0)
    {
        serverParams->setMaxThreads(getThreadPoolRef().capacity());
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
