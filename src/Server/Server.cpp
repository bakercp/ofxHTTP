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


#include "Server.h"


namespace ofx {
namespace HTTP {

    
// TODO: move these contstants to private enums
//------------------------------------------------------------------------------
Server::Settings::Settings(Poco::ThreadPool& _threadPool) :
    host("http://127.0.0.1"),
    port(8080),
#ifdef SSL_ENABLED
    bUseSSL(true),
#endif
    maxQueued(64),
    maxThreads(64),
    bKeepAlive(true),
    maxKeepAliveRequests(0), // 0 is unlimited
    keepAliveTimeout(Poco::Timespan(10*Poco::Timespan::SECONDS)), // 10 seconds
    name(""), // empty, will be auto generated
    softwareVersion("Server/1.0"),
    timeout(Poco::Timespan(60*Poco::Timespan::SECONDS)), // 60 seconds
    threadIdleTime(Poco::Timespan(10*Poco::Timespan::SECONDS)),
    threadPriority(Poco::Thread::PRIO_NORMAL),
    threadPool(_threadPool)
{
}

//------------------------------------------------------------------------------
Server::Server(const Settings& settings) :
    _server(NULL),
    _settings(settings)
{
    ofAddListener(ofEvents().exit,this,&Server::exit);
    
#ifdef SSL_ENABLED
    Poco::Net::initializeSSL();
#endif

}

//------------------------------------------------------------------------------
Server::~Server()
{
    if(_server != NULL) {
        stop();
    }
    ofLogVerbose("Server::~Server") << "Server destroyed.";

#ifdef SSL_ENABLED
    Poco::Net::uninitializeSSL();
#endif

}

//------------------------------------------------------------------------------
void Server::exit(ofEventArgs& args)
{
    ofLogVerbose("Server::exit") << "Waiting for server thread cleanup.";

    if(_server != NULL) {
        stop();
    }
    // it is ok to unregister an item that is not currently registered
    // POCO's internal loop won't complain or return errors
    // POCO stores the delegates in a std::vector and iterates through
    // deleting and returning on match, and doing nothing on a no-match condition
    ofRemoveListener(ofEvents().exit,this,&Server::exit);
    ofLogVerbose("Server::exit") << "Exiting server.";
    
}

//------------------------------------------------------------------------------
void Server::start()
{
    if(isRunning()) {
        ofLogWarning("Server::start") << "Server is already running.  Call stop() to stop.";
        return;
    }
    
    // all of these params are an attempt to make the server shut down VERY quickly.
    
    std::string serverName = _settings.name.empty() ? _settings.host + ":" + ofToString(_settings.port) : _settings.name;
        
    Poco::Net::HTTPServerParams* serverParams = new Poco::Net::HTTPServerParams();
    serverParams->setMaxQueued(_settings.maxQueued);
    serverParams->setMaxThreads(_settings.maxThreads);
    serverParams->setKeepAlive(_settings.bKeepAlive);
    serverParams->setMaxKeepAliveRequests(_settings.maxKeepAliveRequests);
    serverParams->setKeepAliveTimeout(_settings.keepAliveTimeout);
    serverParams->setServerName(serverName);
    serverParams->setTimeout(_settings.timeout);
    serverParams->setThreadIdleTime(_settings.threadIdleTime);
    serverParams->setThreadPriority(_settings.threadPriority);
    serverParams->setSoftwareVersion(_settings.softwareVersion);

#ifdef SSL_ENABLED
    if(true) {
        // we use the default thread pool
        
        try {
            
            
        Poco::AutoPtr<Poco::Net::Context> context(new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
                                        "",
                                        Poco::Net::Context::VERIFY_RELAXED,
                                        9,
                                        true,
                                        "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));
            

            Poco::Net::SecureServerSocket serverSocket(_settings.port,64,context);
            
            _server = new Poco::Net::HTTPServer(new ServerRouteManager(routes,true),
                                                _settings.threadPool,
                                                serverSocket,
                                                serverParams);
            
        } catch(const Poco::Exception& exc) {
            cout << "ERROR ERROR" << endl;
            cout << exc.name() << " " << exc.what() << " " << exc.message() << endl;
            
        }
    } else {
        // we use the default thread pool
        server = new Poco::Net::HTTPServer(new ServerRouteManager(routes,false),
                                settings.threadPool,
                                Poco::Net::ServerSocket(settings.port),
                                serverParams);
    }
#else
    // we use the default thread pool
    _server = new Poco::Net::HTTPServer(new ServerRouteManager(_routes,false),
                                        _settings.threadPool,
                                        Poco::Net::ServerSocket(_settings.port),
                                        serverParams);
#endif

    _errorHandler.setName(serverName);
    _previousErrorHandler = ErrorHandler::set(&_errorHandler);
    
    // start the http server
    _server->start();

}

//------------------------------------------------------------------------------
void Server::stop()
{
    if(!isRunning()) {
        ofLogWarning("Server::stop") << "Server is not running.  Call start() to start.";
        return;
    }

    _server->stop();
    
    // TODO: with 1.4.6+ upgrade use server->stopAll(true);
    // server->stopAll(true);
    
    // wait for all threads in the thread pool
    Poco::ThreadPool::defaultPool().joinAll(); // we gotta wait for all of them ... ugh.
    
    ErrorHandler::set(_previousErrorHandler);
    
    ofLogVerbose("Server::stop") << "Server successfully shut down.";
    
    delete _server;
    _server = NULL;
}

//------------------------------------------------------------------------------
std::string Server::getURL() const
{
    std::stringstream ss;
    ss << _settings.host << ":" << _settings.port << "/";
    return ss.str();
}

//------------------------------------------------------------------------------
unsigned short Server::getPort() const
{
    return _settings.port;
}

//------------------------------------------------------------------------------
void Server::clearRoutes()
{
    _routes.clear();
}

//------------------------------------------------------------------------------
void Server::addRoute(AbstractServerRoute::Ptr route)
{
    _routes.push_back(route);
}

//------------------------------------------------------------------------------
void Server::removeRoute(AbstractServerRoute::Ptr route)
{
    std::vector<AbstractServerRoute::Ptr>::iterator iter = _routes.begin();
    while(iter != _routes.end()) {
        if(*iter == route) {
            iter = _routes.erase(iter);
        } else {
            ++iter;
        }
    }
}

//------------------------------------------------------------------------------
bool Server::isRunning() const
{
    return _server != NULL;
}


} } // namespace ofx::HTTP
