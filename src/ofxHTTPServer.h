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
 
 ==============================================================================*/

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

#include "ofxHTTPConstants.h"
#include "ofxHTTPBaseTypes.h"
#include "ofxThreadErrorHandler.h"

#include "ofxHTTPServerRouteManager.h"

using std::string;

using Poco::Thread;
using Poco::ThreadPool;
using Poco::Timespan;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandlerFactory;

#ifdef SSL_ENABLED
//#include "Poco/Net/ConsoleCertificateHandler.h"
//#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/SecureServerSocket.h"
//#include "Poco/Net/SSLManager.h"

//using Poco::Net::ConsoleCertificateHandler;
//using Poco::Net::InvalidCertificateHandler;
using Poco::Net::Context;
using Poco::Net::KeyConsoleHandler;
using Poco::Net::PrivateKeyPassphraseHandler;
using Poco::Net::SecureServerSocket;
//using Poco::Net::SSLManager;
#endif


//------------------------------------------------------------------------------
class ofxHTTPServer {
public:
    struct Settings;

    ofxHTTPServer(ThreadPool& _threadPool = ThreadPool::defaultPool());
    
    virtual ~ofxHTTPServer();
    
    virtual void loadSettings(Settings settings = Settings());
    
    void start();
    void stop();
    void threadedFunction();
    
    void exit(ofEventArgs& args);
        
    string getURL() const; // TODO: POCO URI
    int    getPort() const;
    
    void clearRoutes();
    
    void addRoute(ofxBaseHTTPServerRoute::Ptr route);
    void removeRoute(ofxBaseHTTPServerRoute::Ptr route);
    
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
        Timespan         keepAliveTimeout;
        string           name;
        Timespan         timeout;
        Timespan         threadIdleTime;
        Thread::Priority threadPriority;
        string           softwareVersion;
                
		Settings();
	};
    
protected:
    ThreadPool& threadPool;
    
    HTTPServer* server;
    
    bool bSettingsLoaded;
    Settings settings;

    vector<ofxBaseHTTPServerRoute::Ptr> routes;
    
    ofThreadErrorHandler errorHandler;
    ErrorHandler* previousErrorHandler;

};
