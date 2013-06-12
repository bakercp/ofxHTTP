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


#include "ClientContext.h"

namespace ofx {
namespace HTTP {

    
//------------------------------------------------------------------------------
ClientContext::ClientContext()
{
}

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& sessionSettings) :
    _sessionSettings(sessionSettings)
{
}

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& sessionSettings,
                             CredentialStore& credentialStore) :
    _sessionSettings(sessionSettings),
    _credentialStore(credentialStore)
{
}

//------------------------------------------------------------------------------
ClientContext::ClientContext(SessionSettings& sessionSettings,
                             CredentialStore& credentialStore,
                             CookieStore& cookieStore) :
    _sessionSettings(sessionSettings),
    _credentialStore(credentialStore),
    _cookieStore(cookieStore)
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
