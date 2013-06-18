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


#include "BaseServerBasicAuthenticationManager.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
BaseBasicServerAuthenticationManager::BaseBasicServerAuthenticationManager(const std::string& realm) :
    _realm(realm)
{
}

//------------------------------------------------------------------------------
BaseBasicServerAuthenticationManager::~BaseBasicServerAuthenticationManager()
{
}

//------------------------------------------------------------------------------
Authentication::Status BaseBasicServerAuthenticationManager::authenticate(HTTPServerRequest& request)
{
    if(request.hasCredentials()) {
        if(Poco::Net::HTTPCredentials::hasBasicCredentials(request)) {
            Poco::Net::HTTPBasicCredentials pCredentials(request);
            Credentials credentials(pCredentials.getUsername(),pCredentials.getPassword());
            if(checkCredentials(credentials)) {
                return Authentication::OK;
            } else {
                return Authentication::UNAUTHORIZED;
            }
        } else if(Poco::Net::HTTPCredentials::hasDigestCredentials(request)) {
            ofLogWarning("ofxBaseHTTPServerBasicAuthenticationManager::authenticate") << "HTTPServerResponse does not currently support DIGEST Auththentication";
            return Authentication::UNAUTHORIZED;
        } else {
            std::string scheme;
            std::string authInfo;

            try {
                request.getCredentials(scheme, authInfo);
            } catch(const Poco::Net::NotAuthenticatedException& exc) {
                ofLogWarning("ofxBaseHTTPServerBasicAuthenticationManager::authenticate") << "Unrecognizable authentication scheme.";
                return Authentication::UNAUTHORIZED;
            }

            ofLogWarning("ofxBaseHTTPServerAuthenticationManager::authenticate") << "Unrecognized authentication scheme : " << scheme;
            return Authentication::UNAUTHORIZED;
        }
    } else {
        return Authentication::NO_CREDENTIALS;
    }
}

//------------------------------------------------------------------------------
std::string BaseBasicServerAuthenticationManager::getRealm()
{
    return _realm;
}

//------------------------------------------------------------------------------
void BaseBasicServerAuthenticationManager::setAuthenticationRequiredHeaders(HTTPServerResponse& response)
{
    // similar to response.requireAuthentication(realm),
    // but we want to send our own content after the
    // headers have been sent.
    std::string auth("Basic realm=\"");
    auth.append(_realm);
    auth.append("\"");
    response.set("WWW-Authenticate", auth);
}
    

} } // namespace ofx::HTTP
