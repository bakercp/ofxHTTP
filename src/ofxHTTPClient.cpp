#include "ofxHTTPClient.h"

//------------------------------------------------------------------------------
ofxHTTPResponseStream* ofxHTTPClient::openResponseStream(const ofxHTTPBaseRequest& request,
                                                         ofxHTTPClientContextPtr context) {
    
    URI uri(request.getURI());
    
    ofxHTTPSessionSettings& sessionSettings = context.get()->getSessionSettingsRef();  // get a copy of the session settings
    ofxHTTPCredentialStore& credentials     = context.get()->getCredentialStoreRef();
    ofxHTTPCookieStore&     cookies         = context.get()->getCookieStoreRef();
    
    int redirects = 0;
    
    HTTPResponse pResponse;
    
    if(!request.hasValidURI()) {
        ofLogVerbose("ofxHTTPClient::open") << "Invalid URI.";
        return new ofxHTTPResponseStream(pResponse,NULL,new SyntaxException("Invalid URI", request.invalidURI));
    }
    
    // ofLogVerbose("ofxHTTPClient::open") << "Beginning redirect loop. Max redirects = " << requestSettings.maxRedirects ;
    
    try {
        
        while (redirects < sessionSettings.getMaxRedirects()) {
//            URI resolvedURI(targetURI);
            
            URI redirectedProxyUri;
            HTTPClientSession* pSession = NULL;
            bool proxyRedirectRequested = false;
            
            bool authenticationRequested = false;
//            bool authenticationAttempted = false;
            
            
            ofLogVerbose("ofxHTTPClient::open") << "Beginning retry loop : redirect # " << redirects;
            
            do {
                
                // psession will be deleted if proxy is redirected
                if (!pSession) {
                    
                    pSession = new HTTPClientSession(uri.getHost(), uri.getPort());
                    
                    ofLogVerbose("ofxHTTPClient::open") << "New session created - host: " << pSession->getHost() << " port: " << pSession->getPort();
                    
                    if(sessionSettings.isProxyEnabled()) {
                        if(redirectedProxyUri.empty()) {
                            ofLogVerbose("ofxHTTPClient::open") << "Using proxy " << sessionSettings.getProxyHost() << " @ " << sessionSettings.getProxyPort();

                            pSession->setProxyHost(sessionSettings.getProxyHost());
                            pSession->setProxyPort(sessionSettings.getProxyPort());
                        } else {
                            ofLogVerbose("ofxHTTPClient::open") << "Using redirected proxy " << redirectedProxyUri.getHost() << " @ " << redirectedProxyUri.getPort();

                            // use the redirected proxy url, rather than the one provided in settings
                            pSession->setProxyHost(redirectedProxyUri.getHost());
                            pSession->setProxyPort(redirectedProxyUri.getPort());
                        }

                        if(sessionSettings.hasProxyCredentials()) {
                            ofLogVerbose("ofxHTTPClient::open") << "With proxy username:password : " << sessionSettings.getProxyUsername() << ":" << sessionSettings.getProxyPassword();
                            pSession->setProxyUsername(sessionSettings.getProxyUsername());
                            pSession->setProxyPassword(sessionSettings.getProxyPassword());
                        } else {
                            ofLogVerbose("ofxHTTPClient::open") << "Without proxy credentials.";
                        }
                    }
                    
                } else {
                    ofLogVerbose("ofxHTTPClient::open") << "Using existing session - host: " << pSession->getHost() << " port: " << pSession->getPort();
                }
                
                string path = uri.getPathAndQuery();
                
                if(path.empty()) path = "/";
                
                ofLogVerbose("ofxHTTPClient::open") << "Using path: " << path;
                
                HTTPRequest pRequest(request.getMethod(), path, request.getVersion());
                
                // apply defaults from the session first
                if(sessionSettings.hasDefaultHeaders()) {
                    ofLogVerbose("ofxHTTPClient::open") << "Writing default headers:";
                    NameValueCollection defaultHeaders = sessionSettings.getDefaultHeaders();
                    NameValueCollection::ConstIterator iter = defaultHeaders.begin();
                    while(iter != defaultHeaders.end()) {
                        ofLogVerbose("ofxHTTPClient::open") << (*iter).first << "=" << (*iter).second;
                        pRequest.set((*iter).first, (*iter).second);
                    }
                }
                
                
                if(!sessionSettings.getUserAgent().empty()) {
                    ofLogVerbose("ofxHTTPClient::open") << "Setting user agent to: " << sessionSettings.getUserAgent();
                    pRequest.set("User-Agent", sessionSettings.getUserAgent());
                }
                
                ////// ALL OF THIS NEEDS TO GO ELSEWHERE -- maybe inside the context?
                if(authenticationRequested) {
                    credentials.authenticate(*pSession, pRequest, pResponse);
                } else {
                    credentials.authenticate(*pSession, pRequest);//
                }
                
                
                // call back into the request to pull the request data
                ofLogVerbose("ofxHTTPClient::open") << "Preparing request.";
                request.prepareRequest(pRequest);
                
                
                //////////////////////////////////////////////////////////////////
                /////////////////////// -- SEND REQUEST -- ///////////////////////
                //////////////////////////////////////////////////////////////////
                ofLogVerbose("ofxHTTPClient::open") << "Sending request.";
                ostream& requestStream = pSession->sendRequest(pRequest);
                
                ofLogVerbose("ofxHTTPClient::open") << "Sending request body.";
                request.sendRequestBody(requestStream); // upload, put etc
                //////////////////////////////////////////////////////////////////
                ///////////////////////// -- RESPONSE -- /////////////////////////
                //////////////////////////////////////////////////////////////////
                ofLogVerbose("ofxHTTPClient::open") << "Receiving response.";
                istream& responseStream = pSession->receiveResponse(pResponse);
                
                
                cookies.store(pResponse);
                
                
                if (pResponse.getStatus() == HTTPResponse::HTTP_MOVED_PERMANENTLY ||
                    pResponse.getStatus() == HTTPResponse::HTTP_FOUND ||
                    pResponse.getStatus() == HTTPResponse::HTTP_SEE_OTHER ||
                    pResponse.getStatus() == HTTPResponse::HTTP_TEMPORARY_REDIRECT) {
                    
                    uri.resolve(pResponse.get("Location"));
                    
                    ++redirects;
                    delete pSession;
                    pSession = NULL;
                    
                    ofLogVerbose("ofxHTTPClient::open") << "Redirecting to: " << uri.toString();
                    
                } else if (pResponse.getStatus() == HTTPResponse::HTTP_OK) {
                    ofLogVerbose("ofxHTTPClient::open") << "Got valid stream, returning.";
                    
                    return new ofxHTTPResponseStream(pResponse, new HTTPResponseStream(responseStream, pSession));
                
                } else if (pResponse.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED && !authenticationRequested) {
                    authenticationRequested = true;
                    
                    streamsize n = ofxHTTPStreamUtils::consume(responseStream);
                    
                    ofLogVerbose("ofxHTTPClient::open") << "Got HTTP_UNAUTHORIZED, trying to authorize with response this time. (" << n << " bytes consumed)";
                    
                } else if (pResponse.getStatus() == HTTPResponse::HTTP_USEPROXY && !proxyRedirectRequested) {
                    // The requested resource MUST be accessed through the proxy
                    // given by the Location field. The Location field gives the
                    // URI of the proxy. The recipient is expected to repeat this
                    // single request via the proxy. 305 responses MUST only be generated by origin servers.
                    // only use for one single request!
                    redirectedProxyUri.resolve(pResponse.get("Location"));
                    delete pSession;
                    pSession = NULL;
                    proxyRedirectRequested = true; // only allow useproxy once

                    streamsize n = ofxHTTPStreamUtils::consume(responseStream);
                    
                    ofLogVerbose("ofxHTTPClient::open") << "Got HTTP_USEPROXY, trying to use redirected proxy. (" << n << " bytes consumed)";
                    
                } else {
                    ofLogVerbose("ofxHTTPClient::open") << "Got other response " << pResponse.getStatus() << " b/c " << pResponse.getReason();
                    return new ofxHTTPResponseStream(pResponse, new HTTPResponseStream(responseStream, pSession),
                                                     new Exception(pResponse.getReason(), uri.toString()));
                }
                
            } while (authenticationRequested || proxyRedirectRequested);
            
        }
    } catch(const HostNotFoundException& exc) {
        ofLogVerbose("ofxHTTPClient::open") << "Got exception " << exc.displayText();
        return new ofxHTTPResponseStream(pResponse, NULL, exc.clone());
    } catch(const Exception& exc) {
        ofLogVerbose("ofxHTTPClient::open") << "Got exception " << exc.displayText();
        return new ofxHTTPResponseStream(pResponse, NULL, exc.clone());
    } catch(...) {
        ofLogVerbose("ofxHTTPClient::open") << "Got unknown exception";
        return new ofxHTTPResponseStream(pResponse, NULL, new Exception("Unknown exception"));
    }
    
    return new ofxHTTPResponseStream(pResponse, NULL, new IOException("Too many redirects while opening URI", request.getURI().toString()));
}

//    if(requestSettings.bHandleRedirects && ofxHTTPResponse::isRedirectRequested(pResponse)) {
//        
//        if(request.redirectCount >= requestSettings.maxRedirects) {
//            ofLogError("ofxHTTPClient::getRedirectionURI") << "Maximum redirects were exceeded.";
//            
//        } else if(pResponse.has("Location")) {
//            string location = pResponse.get("Location");
//            
//            URI redirectURI(location);
//            
//            
//            if(requestSettings.bRejectRelativeRedirects &&
//               redirectURI.isRelative()) {
//                ofLogError("ofxHTTPClient::getRedirectionURI") << "Relative Redirects are Prohibited.";
//                // then fail becaues we don't allow them
//            } else {
//                redirectURI = uri;
//                redirectURI.resolve(location);
//            }
//            
//            if(requestSettings.bRequireHTTPRedirects &&
//               redirectURI.getScheme() != "http" &&
//               redirectURI.getScheme() != "https") {
//                ofLogError("ofxHTTPClient::getRedirectionURI") << "Invalid redirect scheme: " << redirectURI.getScheme();
//            }
//            
//            if(!requestSettings.bAllowCircularRedirects && request.redirectCount > 0) {
//                vector<URI>::const_iterator iter = request.redirectHistory.begin();
//                while(iter != request.redirectHistory.end()) {
//                    URI hURI = (*iter);
//                    if(hURI == redirectURI) {
//                        ofLogError("ofxHTTPClient::getRedirectionURI") << "Circular Redirect Detected.";
//                    }
//                    ++iter;
//                }
//            }
//



//------------------------------------------------------------------------------
ofxHTTPClient::ofxHTTPClient(ofxHTTPClientContextPtr _context, ofThreadPool& _threadPool) :
context(_context),
threadPool(_threadPool)
{
    ofAddListener(ofEvents().update,this,&ofxHTTPClient::update);
    
    
    cout << "MAXR=" << context.get()->getSessionSettingsRef().getMaxRedirects() << endl;
}

//------------------------------------------------------------------------------
ofxHTTPClient::~ofxHTTPClient() {
    ofRemoveListener(ofEvents().update,this,&ofxHTTPClient::update);
}


//------------------------------------------------------------------------------
void ofxHTTPClient::update(ofEventArgs& args) {
    // keep an eye on the queue
    
//    bool threadPoolAvailable = true;
//    while(!executorQueue.empty() && threadPoolAvailable) {
//        try {
//            
//            ofPtr< ofxHTTPRequestExecutor >& task = executorQueue.front();
//            ofxThreadSettings settings = task->getThreadSettings();
//
//            threadPool.startWithPriority(settings.getPriority(), *task.get(), settings.getName());
//
//            // if successful, then we will pop it off.
//            executorQueue.pop();
//            
//        } catch(const NoThreadAvailableException& exc) {
//            threadPoolAvailable = false;
//        }
//    }
}
        
//------------------------------------------------------------------------------
ofxHTTPResponseStream* ofxHTTPClient::exectute(const ofxHTTPBaseRequest& _request) {
    return openResponseStream(_request,context);
}

//------------------------------------------------------------------------------
void ofxHTTPClient::exectute(const ofxHTTPBaseRequest& _request, ofxBaseHTTPResponseStreamConsumer* consumer) {
    if(consumer != NULL) {
        consumer->consume(openResponseStream(_request));
    } else {
        ofLogError("ofxHTTPClient::exectute") << "consumer was NULL";
    }
}

//------------------------------------------------------------------------------
ThreadPool& ofxHTTPClient::getThreadPoolRef() {
    return threadPool;
}

//------------------------------------------------------------------------------
ofxHTTPClientContextPtr ofxHTTPClient::getClientContext() {
    return context;
}

//------------------------------------------------------------------------------
void ofxHTTPClient::setCredentials(const ofxHTTPAuthScope& scope, const ofxHTTPCredentials& credentials) {
    context->getCredentialStoreRef().setCredentials(scope, credentials);
}

//------------------------------------------------------------------------------
void ofxHTTPClient::setCredentials(const URI& uri, const string& username, const string& password) {
    context->getCredentialStoreRef().setCredentials(uri, username, password);
}

//------------------------------------------------------------------------------
//void ofxHTTPClient::setClientContext(const ofxHTTPClientContext& _context) {
//    context = _context;
//}
//
////------------------------------------------------------------------------------
//ofxHTTPClientContext& ofxHTTPClient::getClientContextRef() {
//    return context;
//}
//
////------------------------------------------------------------------------------
//ofxHTTPClientContext ofxHTTPClient::getClientContext() const {
//    return context;
//}


