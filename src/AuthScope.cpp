/*==============================================================================

 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 =============================================================================*/


#include "AuthScope.h"


namespace ofx {
namespace HTTP {
        

//------------------------------------------------------------------------------
AuthScope::AuthScope()
: _bIsSchemeSet(false)
, _scheme("")
, _bIsHostSet(false)
, _host("")
, _bIsPortSet(false)
, _port(0)
, _bIsRealmSet(false)
, _realm("")
, _bIsAuthTypeSet(false)
, _authType(Authentication::BASIC)
{

}

//------------------------------------------------------------------------------
AuthScope::AuthScope(const std::string& host)
: _bIsSchemeSet(false)
, _scheme("")
, _bIsHostSet(true)
, _host(host)
, _bIsPortSet(false)
, _port(0)
, _bIsRealmSet(false)
, _realm("")
, _bIsAuthTypeSet(false)
, _authType(Authentication::BASIC)
{
    
}
    
//------------------------------------------------------------------------------
AuthScope::AuthScope(const std::string& host, unsigned short port)
: _bIsSchemeSet(false)
, _scheme("")
, _bIsHostSet(true)
, _host(host)
, _bIsPortSet(true)
, _port(port)
, _bIsRealmSet(false)
, _realm("")
, _bIsAuthTypeSet(false)
, _authType(Authentication::BASIC)
{

}

    //------------------------------------------------------------------------------
AuthScope::AuthScope(const std::string& host, Authentication::Type authType)
: _bIsSchemeSet(false)
, _scheme("")
, _bIsHostSet(true)
, _host(host)
, _bIsPortSet(false)
, _port(0)
, _bIsRealmSet(false)
, _realm("")
, _bIsAuthTypeSet(true)
, _authType(authType)
{

}

//------------------------------------------------------------------------------
AuthScope::AuthScope(const std::string& host,
                     unsigned short port,
                     Authentication::Type authType)
: _bIsSchemeSet(false)
, _scheme("")
, _bIsHostSet(true)
, _host(host)
, _bIsPortSet(true)
, _port(port)
, _bIsRealmSet(false)
, _realm("")
, _bIsAuthTypeSet(true)
, _authType(authType)
{

}

//------------------------------------------------------------------------------
AuthScope::AuthScope(const std::string& scheme,
                     const std::string& host,
                     unsigned short port,
                     const std::string& realm,
                     Authentication::Type authType)
: _bIsSchemeSet(true)
, _scheme(scheme)
, _bIsHostSet(true)
, _host(host)
, _bIsPortSet(true)
, _port(port)
, _bIsRealmSet(true)
, _realm(realm)
, _bIsAuthTypeSet(true)
, _authType(authType)
{
    
}

//------------------------------------------------------------------------------
AuthScope::AuthScope(const Poco::URI& uri)
: _bIsSchemeSet(false)
, _scheme("")
, _bIsHostSet(false)
, _host("")
, _bIsPortSet(false)
, _port(0)
, _bIsRealmSet(false)
, _realm("")
, _bIsAuthTypeSet(false)
, _authType(Authentication::BASIC)
{
    if(!uri.getScheme().empty()) setScheme(uri.getScheme());
    if(!uri.getHost().empty())   setHost(uri.getHost());
    if(uri.getPort() > 0)        setPort(uri.getPort());

    // realm and auth type cannot be determined from a URI
}

//------------------------------------------------------------------------------
bool AuthScope::isSchemeSet() const
{
    return _bIsSchemeSet;
}

//------------------------------------------------------------------------------
void AuthScope::clearScheme()
{
    _scheme.clear();
    _bIsSchemeSet = false;
}

//------------------------------------------------------------------------------
void AuthScope::setScheme(const std::string& scheme)
{
    _scheme = scheme;
    _bIsSchemeSet = true;
}

//------------------------------------------------------------------------------
std::string AuthScope::getScheme() const
{
    return _scheme;
}

//------------------------------------------------------------------------------
bool AuthScope::isHostSet() const
{
    return _bIsHostSet;
}

//------------------------------------------------------------------------------
void AuthScope::clearHost()
{
    _host.clear();
    _bIsHostSet = false;
}

//------------------------------------------------------------------------------
void AuthScope::setHost(const std::string& host)
{
    _host = host;
    _bIsHostSet = true;
}

//------------------------------------------------------------------------------
std::string AuthScope::getHost() const
{
    return _host;
}

//------------------------------------------------------------------------------
bool AuthScope::isPortSet() const
{
    return _bIsPortSet;
}

//------------------------------------------------------------------------------
void AuthScope::setPort(unsigned short port)
{
    _port = port;
    _bIsPortSet = true;
}

//------------------------------------------------------------------------------
void AuthScope::clearPort()
{
    _port = 0;
    _bIsPortSet = false;
}

//------------------------------------------------------------------------------
unsigned short AuthScope::getPort() const
{
    return _port;
}

//------------------------------------------------------------------------------
bool AuthScope::isRealmSet() const
{
    return _bIsRealmSet;
}

//------------------------------------------------------------------------------
void AuthScope::setRealm(const std::string& realm)
{
    _realm = realm;
    _bIsRealmSet = true;
}

//------------------------------------------------------------------------------
void AuthScope::clearRealm() {
    _realm.clear();
    _bIsRealmSet = false;
}

//------------------------------------------------------------------------------
std::string AuthScope::getRealm() const
{
    return _realm;
}

//------------------------------------------------------------------------------
bool AuthScope::isAuthTypeSet() const
{
    return _bIsAuthTypeSet;
}

//------------------------------------------------------------------------------
void AuthScope::setAuthType(Authentication::Type authType)
{
    _authType = authType;
    _bIsAuthTypeSet = true;
}

//------------------------------------------------------------------------------
void AuthScope::clearAuthType()
{
    _authType = Authentication::BASIC;
    _bIsAuthTypeSet = false;
}

//------------------------------------------------------------------------------
Authentication::Type AuthScope::getAuthType() const
{
    return _authType;
}

//------------------------------------------------------------------------------
bool AuthScope::operator== (const AuthScope& scope) const
{
    return  _bIsSchemeSet   == scope._bIsSchemeSet      &&
            _scheme         == scope._scheme            &&
            _bIsHostSet     == scope._bIsHostSet        &&
            _host           == scope._host              &&
            _bIsPortSet     == scope._bIsPortSet        &&
            _port           == scope._port              &&
            _bIsRealmSet    == scope._bIsRealmSet       &&
            _realm          == scope._realm             &&
            _bIsAuthTypeSet == scope._bIsAuthTypeSet    &&
            _authType       == scope._authType;
}

//------------------------------------------------------------------------------
int AuthScope::match(const AuthScope& scope) const
{
    // from org/apache/http/auth/AuthScope.html
    int factor = 0;
    
    if(_bIsAuthTypeSet && scope._bIsAuthTypeSet && _authType == scope._authType)
    {
        factor += 1;
    }
    else if(_bIsAuthTypeSet && scope._bIsAuthTypeSet)
    {
        return -1;
    }
    
    if(_bIsRealmSet && scope._bIsRealmSet && _realm == scope._realm)
    {
        factor += 2;
    }
    else if(_bIsRealmSet && scope._bIsRealmSet)
    {
        return -1;
    }

    if (_bIsSchemeSet && scope._bIsSchemeSet && _scheme == scope._scheme)
    {
        // not sure this is the best ...
        // why is this more important than host?
        factor += 3;
    }
    else if(_bIsSchemeSet && scope._bIsSchemeSet)
    {
        return -1;
    }

    if(_bIsPortSet && scope._bIsPortSet && _port == scope._port)
    {
        factor += 4;
    }
    else if(_bIsPortSet && scope._bIsPortSet)
    {
        return -1;
    }
    
    if (_bIsHostSet && scope._bIsHostSet && _host == scope._host)
    {
        factor += 8;
    }
    else if(_bIsHostSet && scope._bIsHostSet)
    {
        return -1;
    }

    return factor;
}

//------------------------------------------------------------------------------
bool AuthScope::operator!= (const AuthScope& scope) const
{
    return !((*this)==scope);
}

//------------------------------------------------------------------------------
bool AuthScope::operator< (const AuthScope& scope) const
{
    return match(scope) < 0;
}

//------------------------------------------------------------------------------
std::string AuthScope::toString() const
{
    std::stringstream ss;

    ss << "Scheme: " << (_bIsSchemeSet ? getScheme() : "Any");

    ss << " Host: " << (_bIsHostSet ? getHost() : "Any");

    ss << " Port: ";
    
    if(_bIsPortSet)
    {
        ss << getPort();
    }
    else
    {
        ss << "Any";
    }

    ss << " Realm: " << (_bIsRealmSet ? getRealm() : "Any");
    
    ss << " AuthType: ";

    if(_bIsAuthTypeSet)
    {
        switch(getAuthType())
        {
            case Authentication::BASIC:
                ss << "BASIC";
                break;
            case Authentication::DIGEST:
                ss << "DIGEST";
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


} }
