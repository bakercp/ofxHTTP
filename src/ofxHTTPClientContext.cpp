#include "ofxHTTPClientContext.h"

//DefaultClientContext = 

static ofxHTTPClientContextPtr DefaultClientContext = ofxHTTPClientContextPtr(new ofxHTTPClientContext());

//------------------------------------------------------------------------------
ofxHTTPClientContextPtr ofxHTTPClientContext::defaultClientContext() {
    return DefaultClientContext;
}

//------------------------------------------------------------------------------
ofxHTTPClientContext::ofxHTTPClientContext() { }

//------------------------------------------------------------------------------
ofxHTTPClientContext::ofxHTTPClientContext(ofxHTTPSessionSettings& _sessionSettings) :
sessionSettings(_sessionSettings)
{ }

//------------------------------------------------------------------------------
ofxHTTPClientContext::ofxHTTPClientContext(ofxHTTPSessionSettings& _sessionSettings,
                                           ofxHTTPCredentialStore& _credentialStore) :
sessionSettings(_sessionSettings),
credentialStore(_credentialStore)
{ }

//------------------------------------------------------------------------------
ofxHTTPClientContext::ofxHTTPClientContext(ofxHTTPSessionSettings& _sessionSettings,
                                           ofxHTTPCredentialStore& _credentialStore,
                                           ofxHTTPCookieStore& _cookieStore) :
sessionSettings(_sessionSettings),
credentialStore(_credentialStore),
cookieStore(_cookieStore)
{ }

//------------------------------------------------------------------------------
ofxHTTPClientContext::ofxHTTPClientContext(ofxHTTPClientContext& that) {
    sessionSettings = that.sessionSettings;
    credentialStore = that.credentialStore;
    cookieStore     = that.cookieStore;
}

//------------------------------------------------------------------------------
ofxHTTPClientContext& ofxHTTPClientContext::operator = (ofxHTTPClientContext& that) {
    sessionSettings = that.sessionSettings;
    credentialStore = that.credentialStore;
    cookieStore     = that.cookieStore;
}

//------------------------------------------------------------------------------
ofxHTTPClientContext::ofxHTTPClientContext(const ofxHTTPClientContext& that) { }

//------------------------------------------------------------------------------
ofxHTTPClientContext& ofxHTTPClientContext::operator = (const ofxHTTPClientContext& that) { }

//------------------------------------------------------------------------------
ofxHTTPClientContext::~ofxHTTPClientContext() { }

//------------------------------------------------------------------------------
ofxHTTPSessionSettings ofxHTTPClientContext::getSessionSettings() {
    return sessionSettings;
}

//------------------------------------------------------------------------------
ofxHTTPSessionSettings& ofxHTTPClientContext::getSessionSettingsRef() {
    return sessionSettings;
}

//------------------------------------------------------------------------------
ofxHTTPCookieStore ofxHTTPClientContext::getCookieStore() {
    return cookieStore;
}

//------------------------------------------------------------------------------
ofxHTTPCookieStore& ofxHTTPClientContext::getCookieStoreRef() {
    return cookieStore;
}

//------------------------------------------------------------------------------
ofxHTTPCredentialStore ofxHTTPClientContext::getCredentialStore() {
    return credentialStore;
}

//------------------------------------------------------------------------------
ofxHTTPCredentialStore& ofxHTTPClientContext::getCredentialStoreRef() {
    return credentialStore;
}

////------------------------------------------------------------------------------
//bool ofxHTTPClientContext::canAuthenticate(HTTPRequest& request) {
//    
//}
//
////------------------------------------------------------------------------------
//bool ofxHTTPClientContext::canAuthenticate(HTTPRequest& request, HTTPResponse& response) {
//    
//}
//
////------------------------------------------------------------------------------
//bool ofxHTTPClientContext::authenticate(HTTPRequest& request) {
//    
//}
//
////------------------------------------------------------------------------------
//bool ofxHTTPClientContext::authenticate(HTTPRequest& request, HTTPResponse& response) {
//    
//}
//
