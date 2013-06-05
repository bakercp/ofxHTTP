#include "ClientContext.h"

namespace ofx {
namespace HTTP {

    
//------------------------------------------------------------------------------
ClientContext::ClientContext()
{
}

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& sessionSettings)
: _sessionSettings(sessionSettings)
{
}

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& sessionSettings,
                             CredentialStore& credentialStore)
: _sessionSettings(sessionSettings)
, _credentialStore(credentialStore)
{
}

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& sessionSettings,
                             CredentialStore& credentialStore,
                             CookieStore& cookieStore)
: _sessionSettings(sessionSettings)
, _credentialStore(credentialStore)
, _cookieStore(cookieStore)
{

}

//------------------------------------------------------------------------------
ClientContext::ClientContext(ClientContext& that)
{
    _sessionSettings = that._sessionSettings;
    _credentialStore = that._credentialStore;
    _cookieStore     = that._cookieStore;
}

//------------------------------------------------------------------------------
ClientContext& ClientContext::operator = (ClientContext& that)
{
    _sessionSettings = that._sessionSettings;
    _credentialStore = that._credentialStore;
    _cookieStore     = that._cookieStore;
}

//------------------------------------------------------------------------------
ClientContext::~ClientContext()
{
}

//------------------------------------------------------------------------------
SessionSettings ClientContext::getSessionSettings()
{
    return _sessionSettings;
}

//------------------------------------------------------------------------------
SessionSettings& ClientContext::getSessionSettingsRef()
{
    return _sessionSettings;
}

//------------------------------------------------------------------------------
CookieStore ClientContext::getCookieStore()
{
    return _cookieStore;
}

//------------------------------------------------------------------------------
CookieStore& ClientContext::getCookieStoreRef()
{
    return _cookieStore;
}

//------------------------------------------------------------------------------
ClientContext::Ptr ClientContext::defaultClientContext() {
    static ClientContext::Ptr DefaultClientContext = ClientContext::Ptr(new ClientContext());

    return DefaultClientContext;
}

//------------------------------------------------------------------------------
CredentialStore ClientContext::getCredentialStore()
{
    return _credentialStore;
}

//------------------------------------------------------------------------------
CredentialStore& ClientContext::getCredentialStoreRef()
{
    return _credentialStore;
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
