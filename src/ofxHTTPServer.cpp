#include "ofxHTTPServer.h"

//------------------------------------------------------------------------------
ofxHTTPServer::ofxHTTPServer(const Settings& _settings) : server(NULL), settings(_settings){
    ofAddListener(ofEvents().exit,this,&ofxHTTPServer::exit);
    
#ifdef SSL_ENABLED
    Poco::Net::initializeSSL();
#endif
}

//------------------------------------------------------------------------------
ofxHTTPServer::~ofxHTTPServer() {
    if(server != NULL) {
        stop();
    }
    ofLogVerbose("ofxHTTPServer::~ofxHTTPServer") << "Server destroyed.";

#ifdef SSL_ENABLED
    Poco::Net::uninitializeSSL();
#endif

}

void ofxHTTPServer::exit(ofEventArgs& args) {
    ofLogVerbose("ofxHTTPServer::exit") << "Waiting for server thread cleanup.";

    if(server != NULL) {
        stop();
    }
    // it is ok to unregister an item that is not currently registered
    // POCO's internal loop won't complain or return errors
    // POCO stores the delegates in a std::vector and iterates through
    // deleting and returning on match, and doing nothing on a no-match condition
    ofRemoveListener(ofEvents().exit,this,&ofxHTTPServer::exit);
    ofLogVerbose("ofxHTTPServer::exit") << "Exiting server.";
    
}

//------------------------------------------------------------------------------
void ofxHTTPServer::start(){
    if(isRunning()) {
        ofLogWarning("ofxHTTPServer::start") << "Server is already running.  Call stop() to stop.";
        return;
    }
    
    // all of these params are an attempt to make the server shut down VERY quickly.
    
    string serverName = settings.name.empty() ? settings.host + ":" + ofToString(settings.port) : settings.name;
        
    HTTPServerParams* serverParams = new HTTPServerParams();
    serverParams->setMaxQueued(settings.maxQueued);
    serverParams->setMaxThreads(settings.maxThreads);
    serverParams->setKeepAlive(settings.bKeepAlive);
    serverParams->setMaxKeepAliveRequests(settings.maxKeepAliveRequests);
    serverParams->setKeepAliveTimeout(settings.keepAliveTimeout);
    serverParams->setServerName(serverName);
    serverParams->setTimeout(settings.timeout);
    serverParams->setThreadIdleTime(settings.threadIdleTime);
    serverParams->setThreadPriority(settings.threadPriority);
    serverParams->setSoftwareVersion(settings.softwareVersion);
    
#ifdef SSL_ENABLED
    if(true) {
        // we use the default thread pool
        
        try {
            
            
        Poco::AutoPtr<Context> context(new Context(Context::SERVER_USE,
                                          "",
                                          Context::VERIFY_RELAXED,
                                          9,
                                          true,
                                          "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));
            

            SecureServerSocket serverSocket(settings.port,64,context);
            
            server = new HTTPServer(new ofxHTTPServerRouteManager(routes,true),
                                    threadPool,
                                    serverSocket,
                                    serverParams);
            
        } catch(const Exception& exc) {
            cout << "ERROR ERROR" << endl;
            cout << exc.name() << " " << exc.what() << " " << exc.message() << endl;
            
        }
    } else {
        // we use the default thread pool
        server = new HTTPServer(new ofxHTTPServerRouteManager(routes,false),
                                settings.threadPool,
                                ServerSocket(settings.port),
                                serverParams);
    }
#else
    // we use the default thread pool
    server = new HTTPServer(new ofxHTTPServerRouteManager(routes,false),
                            settings.threadPool,
                            ServerSocket(settings.port),
                            serverParams);
#endif
    
    errorHandler.setName(serverName);
    previousErrorHandler = ErrorHandler::set(&errorHandler);
    
    // start the http server
    server->start();

}

//------------------------------------------------------------------------------
void ofxHTTPServer::stop() {
    if(!isRunning()) {
        ofLogWarning("ofxHTTPServer::stop") << "Server is not running.  Call start() to start.";
        return;
    }

    server->stop();
    
    // TODO: with 1.4.6+ upgrade use server->stopAll(true);
    // server->stopAll(true);
    
    // wait for all threads in the thread pool
    ThreadPool::defaultPool().joinAll(); // we gotta wait for all of them ... ugh.
    
    ErrorHandler::set(previousErrorHandler);
    
    ofLogVerbose("ofxHTTPServer::stop") << "Server successfully shut down.";
    
    delete server;
    server = NULL;
}

//------------------------------------------------------------------------------
string ofxHTTPServer::getURL() const {
    stringstream ss;
    ss << settings.host << ":" << settings.port << "/";
    return ss.str();
}

//------------------------------------------------------------------------------
int ofxHTTPServer::getPort() const {
    return settings.port;
}

//------------------------------------------------------------------------------
void ofxHTTPServer::clearRoutes() {
    routes.clear();
}

//------------------------------------------------------------------------------
void ofxHTTPServer::addRoute(ofxBaseHTTPServerRoute::Ptr route) {
    routes.push_back(route);
}

//------------------------------------------------------------------------------
void ofxHTTPServer::removeRoute(ofxBaseHTTPServerRoute::Ptr route) {
    vector<ofxBaseHTTPServerRoute::Ptr>::iterator iter = routes.begin();
    while(iter != routes.end()) {
        if(*iter == route) {
            iter = routes.erase(iter);
        } else {
            ++iter;
        }
    }
}

//------------------------------------------------------------------------------
bool ofxHTTPServer::isRunning() const {
    return server != NULL;
}

