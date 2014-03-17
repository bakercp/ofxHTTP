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


#pragma once


#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "CookieStore.h"
#include "CredentialStore.h"
//#include "ofxHTTPCredentialStoreCache.h"
#include "SessionSettings.h"

// a thread-safe shared context
// -- in theory all members are thread safe
// -- and no members are reentrant
// setters are not allowed for thread safety.
// if you want a context with a new store, then you have to make one
// via constructors


namespace ofx {
namespace HTTP {
namespace Client {


class Context
{
public:
    typedef std::shared_ptr<Context> SharedPtr;

    Context();
    
    Context(SessionSettings& _settings);

    Context(SessionSettings& _settings,
            CredentialStore& _credentialStore);

    Context(SessionSettings& _settings,
            CredentialStore& _credentialStore,
            CookieStore& _cookieStore);
    
    Context(Context& that);
	Context& operator = (Context& that);
    
    virtual ~Context();
   
    SessionSettings getSessionSettings();
    SessionSettings& getSessionSettingsRef();
    
    CookieStore  getCookieStore();
    CookieStore& getCookieStoreRef();
    
    CredentialStore  getCredentialStore();
    CredentialStore& getCredentialStoreRef();
    
//    bool canAuthenticate(HTTPRequest& request);
//    bool canAuthenticate(HTTPRequest& request, HTTPResponse& response);
//
//    bool authenticate(HTTPRequest& request);
//    bool authenticate(HTTPRequest& request, HTTPResponse& response);
    
//    void setCredentials(const URI& uri, const ofxHTTPCredentials& credentials);
//    void setCredentials(const URI& uri, const string& username, const string& password);
    
//    static ClientContext::SharedPtr defaultClientContext();

    static SharedPtr makeShared()
    {
        return SharedPtr(new Context());
    }

private:
    Context(const Context& other);
	Context& operator = (const Context& other);

    // client settings (i.e. thread pools, # connections, etc).
    // vs. context-settings (num redirects, auth handlers, etc).
    
    SessionSettings _sessionSettings;
    CredentialStore _credentialStore;
    
    CookieStore _cookieStore;

};


} } } // namespace ofx::HTTP::Client
