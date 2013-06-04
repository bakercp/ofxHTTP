#include "ClientContext.h"

namespace ofx {
namespace HTTP {

    
static ClientContext::Ptr DefaultClientContext = ClientContext::Ptr(new ClientContext());

//------------------------------------------------------------------------------
ClientContext::Ptr ClientContext::defaultClientContext() {
    return DefaultClientContext;
}

//------------------------------------------------------------------------------
ClientContext::ClientContext() { }

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& _sessionSettings)
: sessionSettings(_sessionSettings)
{

}

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& _sessionSettings,
                             CredentialStore& _credentialStore)
: sessionSettings(_sessionSettings)
, credentialStore(_credentialStore)
{

}

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& _sessionSettings,
                             CredentialStore& _credentialStore,
                             CookieStore& _cookieStore)
: sessionSettings(_sessionSettings)
, credentialStore(_credentialStore)
, cookieStore(_cookieStore)
{

}

//------------------------------------------------------------------------------
ClientContext::ClientContext(ClientContext& that)
{
    sessionSettings = that.sessionSettings;
    credentialStore = that.credentialStore;
    cookieStore     = that.cookieStore;
}

//------------------------------------------------------------------------------
ClientContext& ClientContext::operator = (ClientContext& that)
{
    sessionSettings = that.sessionSettings;
    credentialStore = that.credentialStore;
    cookieStore     = that.cookieStore;
}

//------------------------------------------------------------------------------
ClientContext::~ClientContext()
{

}

//------------------------------------------------------------------------------
SessionSettings ClientContext::getSessionSettings()
{
    return sessionSettings;
}

//------------------------------------------------------------------------------
SessionSettings& ClientContext::getSessionSettingsRef()
{
    return sessionSettings;
}

//------------------------------------------------------------------------------
CookieStore ClientContext::getCookieStore()
{
    return cookieStore;
}

//------------------------------------------------------------------------------
CookieStore& ClientContext::getCookieStoreRef()
{
    return cookieStore;
}

//------------------------------------------------------------------------------
CredentialStore ClientContext::getCredentialStore()
{
    return credentialStore;
}

//------------------------------------------------------------------------------
CredentialStore& ClientContext::getCredentialStoreRef()
{
    return credentialStore;
}

////------------------------------------------------------------------------------
//bool ClientContext::canAuthenticate(HTTPRequest& request) {
//    
//}
//
////------------------------------------------------------------------------------
//bool ClientContext::canAuthenticate(HTTPRequest& request, HTTPResponse& response) {
//    
//}
//
////------------------------------------------------------------------------------
//bool ClientContext::authenticate(HTTPRequest& request) {
//    
//}
//
////------------------------------------------------------------------------------
//bool ClientContext::authenticate(HTTPRequest& request, HTTPResponse& response) {
//
//}
//

} }
