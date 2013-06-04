/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 =============================================================================*/

#pragma once

#include <string>

#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/Timespan.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/ServerSocket.h"

#include "ofEvents.h"
#include "ofThread.h"
#include "ofTypes.h"
#include "ofUtils.h"

#include "BaseTypes.h"
#include "ofxThreadErrorHandler.h"

#include "ServerRouteManager.h"

// TODO: OF_VERSION_MAJOR/MINOR check
// TODO: undef the ssl stuff

#define SSL_ENABLED 1

#ifdef SSL_ENABLED
//#include "Poco/Net/ConsoleCertificateHandler.h"
//#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/SecureServerSocket.h"
//#include "Poco/Net/SSLManager.h"
#endif

namespace ofx {
namespace HTTP {

    
//------------------------------------------------------------------------------
class Server {
public:
    typedef ofPtr<Server> Ptr;

    struct Settings;

    Server(const Settings& _settings);
    
    virtual ~Server();
        
    void start();
    void stop();
    void threadedFunction();
    
    void exit(ofEventArgs& args);
        
    string getURL() const; // TODO: POCO URI
    int    getPort() const;
    
    void clearRoutes();
    
    void addRoute(BaseServerRoute::Ptr route);
    void removeRoute(BaseServerRoute::Ptr route);
    
    bool isRunning() const;
    
    static Ptr instance(const Settings& _settings) {
        return Ptr(new Server(_settings));
    }

    struct Settings {

        string           host;
        unsigned short   port;
#ifdef SSL_ENABLED
        bool             bUseSSL;
#endif
        int              maxQueued;
        int              maxThreads;
        bool             bKeepAlive;
        int              maxKeepAliveRequests;
        Poco::Timespan   keepAliveTimeout;
        string           name;
        Poco::Timespan   timeout;
        Poco::Timespan   threadIdleTime;
        Poco::Thread::Priority threadPriority;
        string           softwareVersion;
        
        Poco::ThreadPool& threadPool;


        Settings(Poco::ThreadPool& _threadPool = Poco::ThreadPool::defaultPool());
    };


protected:

    Poco::Net::HTTPServer* server;
    
    Settings settings;

    vector<BaseServerRoute::Ptr> routes;
    
    ofThreadErrorHandler errorHandler;
    ErrorHandler* previousErrorHandler;

};


} }
