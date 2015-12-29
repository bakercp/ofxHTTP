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


#include "ofx/HTTP/AuthScope.h"


namespace ofx {
namespace HTTP {


AuthScope::AuthScope():
    _hasScheme(false),
    _scheme(""),
    _hasHost(false),
    _host(""),
    _hasPort(false),
    _port(0),
    _hasRealm(false),
    _realm(""),
    _hasAuthType(false),
    _authType(NONE)
{
}


AuthScope::AuthScope(const std::string& host):
    _hasScheme(false),
    _scheme(""),
    _hasHost(true),
    _host(host),
    _hasPort(false),
    _port(0),
    _hasRealm(false),
    _realm(""),
    _hasAuthType(false),
    _authType(NONE)
{
}


AuthScope::AuthScope(const std::string& scheme,
                     const std::string& host,
                     uint16_t port):
    _hasScheme(true),
    _scheme(scheme),
    _hasHost(true),
    _host(host),
    _hasPort(true),
    _port(port),
    _hasRealm(false),
    _realm(""),
    _hasAuthType(false),
    _authType(NONE)
{
}


AuthScope::AuthScope(const std::string& host, uint16_t port):
    _hasScheme(false),
    _scheme(""),
    _hasHost(true),
    _host(host),
    _hasPort(true),
    _port(port),
    _hasRealm(false),
    _realm(""),
    _hasAuthType(false),
    _authType(NONE)
{
}

    
AuthScope::AuthScope(const std::string& host, AuthenticationType authType):
    _hasScheme(false),
    _scheme(""),
    _hasHost(true),
    _host(host),
    _hasPort(false),
    _port(0),
    _hasRealm(false),
    _realm(""),
    _hasAuthType(true),
    _authType(authType)
{
}


AuthScope::AuthScope(const std::string& host,
                     uint16_t port,
                     AuthenticationType authType):
    _hasScheme(false),
    _scheme(""),
    _hasHost(true),
    _host(host),
    _hasPort(true),
    _port(port),
    _hasRealm(false),
    _realm(""),
    _hasAuthType(true),
    _authType(authType)
{
}


AuthScope::AuthScope(const std::string& scheme,
                     const std::string& host,
                     uint16_t port,
                     const std::string& realm,
                     AuthenticationType authType):
    _hasScheme(true),
    _scheme(scheme),
    _hasHost(true),
    _host(host),
    _hasPort(true),
    _port(port),
    _hasRealm(true),
    _realm(realm),
    _hasAuthType(true),
    _authType(authType)
{
}


AuthScope::AuthScope(const Poco::URI& uri):
    _hasScheme(false),
    _scheme(""),
    _hasHost(false),
    _host(""),
    _hasPort(false),
    _port(0),
    _hasRealm(false),
    _realm(""),
    _hasAuthType(false),
    _authType(BASIC)
{
    if (!uri.getScheme().empty()) setScheme(uri.getScheme());
    if (!uri.getHost().empty())   setHost(uri.getHost());
    if (uri.getPort() > 0)        setPort(uri.getPort());

    // realm and auth type cannot be determined from a URI
}


bool AuthScope::hasScheme() const
{
    return _hasScheme;
}


void AuthScope::clearScheme()
{
    _scheme.clear();
    _hasScheme = false;
}


void AuthScope::setScheme(const std::string& scheme)
{
    if (scheme.empty())
    {
        clearScheme();
    }
    else
    {
        _scheme = scheme;
        _hasScheme = true;
    }
}


std::string AuthScope::getScheme() const
{
    return _scheme;
}


bool AuthScope::hasHost() const
{
    return _hasHost;
}


void AuthScope::clearHost()
{
    _host.clear();
    _hasHost = false;
}


void AuthScope::setHost(const std::string& host)
{
    if (host.empty())
    {
        clearHost();
    }
    else
    {
        _host = host;
        _hasHost = true;
    }
}


std::string AuthScope::getHost() const
{
    return _host;
}


bool AuthScope::hasPort() const
{
    return _hasPort;
}


void AuthScope::setPort(uint16_t port)
{
    if (0 == port)
    {
        clearPort();
    }
    else
    {
        _port = port;
        _hasPort = true;
    }
}


void AuthScope::clearPort()
{
    _port = 0;
    _hasPort = false;
}


uint16_t AuthScope::getPort() const
{
    return _port;
}


bool AuthScope::hasRealm() const
{
    return _hasRealm;
}


void AuthScope::setRealm(const std::string& realm)
{
    if (realm.empty())
    {
        clearRealm();
    }
    else
    {
        _realm = realm;
        _hasRealm = true;
    }
}


void AuthScope::clearRealm() {
    _realm.clear();
    _hasRealm = false;
}


std::string AuthScope::getRealm() const
{
    return _realm;
}


bool AuthScope::hasAuthType() const
{
    return _hasAuthType;
}


void AuthScope::setAuthType(AuthenticationType authType)
{
    if (NONE == authType)
    {
        clearAuthType();
    }
    else
    {
        _authType = authType;
        _hasAuthType = true;
    }
}


void AuthScope::clearAuthType()
{
    _authType = NONE;
    _hasAuthType = false;
}


AuthenticationType AuthScope::getAuthType() const
{
    return _authType;
}


bool AuthScope::operator == (const AuthScope& scope) const
{
    return  _hasScheme   == scope._hasScheme
        &&  _scheme      == scope._scheme
        &&  _hasHost     == scope._hasHost
        &&  _host        == scope._host
        &&  _hasPort     == scope._hasPort
        &&  _port        == scope._port
        &&  _hasRealm    == scope._hasRealm
        &&  _realm       == scope._realm
        &&  _hasAuthType == scope._hasAuthType
        && _authType     == scope._authType;
}


int AuthScope::match(const AuthScope& scope) const
{
    // from org/apache/http/auth/AuthScope.html
    int factor = 0;
    
    if (_hasAuthType && scope._hasAuthType && _authType == scope._authType)
    {
        factor += 1;
    }
    else if (_hasAuthType && scope._hasAuthType)
    {
        return -1;
    }
    
    if (_hasRealm && scope._hasRealm && _realm == scope._realm)
    {
        factor += 2;
    }
    else if (_hasRealm && scope._hasRealm)
    {
        return -1;
    }

    if (_hasScheme && scope._hasScheme && _scheme == scope._scheme)
    {
        // not sure this is the best ...
        // why is this more important than host?
        factor += 3;
    }
    else if (_hasScheme && scope._hasScheme)
    {
        return -1;
    }

    if (_hasPort && scope._hasPort && _port == scope._port)
    {
        factor += 4;
    }
    else if (_hasPort && scope._hasPort)
    {
        return -1;
    }
    
    if (_hasHost && scope._hasHost && _host == scope._host)
    {
        factor += 8;
    }
    else if (_hasHost && scope._hasHost)
    {
        return -1;
    }

    return factor;
}


bool AuthScope::operator != (const AuthScope& scope) const
{
    return !((*this)==scope);
}


bool AuthScope::operator < (const AuthScope& scope) const
{
    return match(scope) < 0;
}


std::string AuthScope::toString() const
{
    std::stringstream ss;

    ss << "Scheme: " << (_hasScheme ? getScheme() : "Any");
    ss << " Host: " << (_hasHost ? getHost() : "Any");
    ss << " Port: ";
    if (_hasPort)
    {
        ss << getPort();
    }
    else
    {
        ss << "Any";
    }

    ss << " Realm: " << (_hasRealm ? getRealm() : "Any");
    ss << " AuthType: ";

    if (_hasAuthType)
    {
        switch (getAuthType())
        {
            case BASIC:
                ss << "BASIC";
                break;
            case DIGEST:
                ss << "DIGEST";
                break;
            case NONE:
                ss << "NONE";
                break;
            default:
                ss << "UNKNOWN";
        }
    }
    else
    {
        ss << "Any";
    }
    
    return ss.str();
}


} } // namespace ofx::HTTP
