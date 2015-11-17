// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


const std::string Context::KEY_PREFIX_RESERVED    = "HTTP_";
const std::string Context::KEY_SESSION_SETTINGS   = "HTTP_SESSION_SETTINGS";
const std::string Context::KEY_COOKIE_STORE       = "HTTP_COOKIE_STORE";
const std::string Context::KEY_CREDENTIAL_STORE   = "HTTP_CREDENTIAL_STORE";
const std::string Context::KEY_RESOLVED_URI       = "HTTP_RESOLVED_URI";
const std::string Context::KEY_PROXY_REDIRECT_URI = "HTTP_PROXY_REDIRECT_URI";
const std::string Context::KEY_REDIRECTS          = "HTTP_REDIRECTS";
const std::string Context::KEY_SESSION            = "HTTP_SESSION";
const std::string Context::KEY_USE_ABSOLUTE_REQUEST_PATH = "USE_ABSOLUTE_REQUEST_PATH";


Context::Context(): _resubmit(false)
{
}


Context::~Context()
{
}


void Context::setClientSessionSettings(const ClientSessionSettings& clientSessionSettings)
{
    _clientSessionSettings = clientSessionSettings;
}


const ClientSessionSettings& Context::getClientSessionSettings() const
{
    return _clientSessionSettings;
}


//void Context::setCookieStore(CookieStore::SharedPtr cookieStore)
//{
//    _cookieStore = cookieStore;
//}
//
//
//CookieStore::WeakPtr Context::getCookieStore()
//{
//    return _cookieStore;
//}


void Context::setClientSession(ClientSession clientSession)
{
    _clientSession = clientSession;
}


Context::ClientSession& Context::getClientSession()
{
    return _clientSession;
}

//void Context::setResolvedURI(const Poco::URI& uri)
//{
//    _resolvedURI = uri;
//}
//
//
//const Poco::URI& Context::getResolvedURI() const
//{
//    return _resolvedURI;
//}


void Context::addRedirect(const Poco::URI& uri)
{
    _redirects.push_back(uri);
}


const std::vector<Poco::URI>& Context::getRedirects() const
{
    return _redirects;
}


void Context::setProxyRedirectURI(const Poco::URI& uri)
{
    _proxyRedirectURI = uri;
}


const Poco::URI& Context::getProxyRedirectURI() const
{
    return _proxyRedirectURI;
}


bool Context::getResubmit() const
{
    return _resubmit;
}

void Context::setResubmit(bool resubmit)
{
    _resubmit = resubmit;
}


} } // namespace ofx::HTTP
