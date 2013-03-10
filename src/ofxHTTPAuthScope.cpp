#include "ofxHTTPAuthScope.h"

//------------------------------------------------------------------------------
ofxHTTPAuthScope::ofxHTTPAuthScope()
{
    init();
}

//------------------------------------------------------------------------------
ofxHTTPAuthScope::ofxHTTPAuthScope(const string& _host)
{
    init();
    setHost(_host);
}

//------------------------------------------------------------------------------
ofxHTTPAuthScope::ofxHTTPAuthScope(const string& _host,
                                   unsigned short _port)
{
    init();
    setHost(_host);
    setPort(_port);
}

//------------------------------------------------------------------------------
ofxHTTPAuthScope::ofxHTTPAuthScope(const string& _host,
                                   ofxHTTPAuthType _type)
{
    init();
    setHost(_host);
    setAuthType(_type);
}

//------------------------------------------------------------------------------
ofxHTTPAuthScope::ofxHTTPAuthScope(const string& _host,
                                   unsigned short _port,
                                   ofxHTTPAuthType _type)
{
    init();
    setHost(_host);
    setPort(_port);
    setAuthType(_type);
}

//------------------------------------------------------------------------------
ofxHTTPAuthScope::ofxHTTPAuthScope(const string& _scheme,
                                   const string& _host,
                                   unsigned short _port,
                                   const string& _realm,
                                   ofxHTTPAuthType _authType)
{
    init();
    setScheme(_scheme);
    setHost(_host);
    setPort(_port);
    setRealm(_realm);
    setAuthType(_authType);
}

//------------------------------------------------------------------------------
ofxHTTPAuthScope::ofxHTTPAuthScope(const URI& uri)
{
    init();
    if(!uri.getScheme().empty()) setScheme(uri.getScheme());
    if(!uri.getHost().empty())   setHost(uri.getHost());
    if(uri.getPort() > 0)        setPort(uri.getPort());

    // realm and auth type cannot be determined from a URI
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::init() {
    bIsSchemeSet   = false;;
    scheme         = "";

    bIsHostSet     = false;;
    host           = "";

    bIsHostSet     = false;;
    host           = "";
    
    bIsPortSet     = false;
    port           = 0;
    
    bIsRealmSet    = false;;
    realm          = "";
    
    bIsAuthTypeSet = false;
    authType = BASIC;
}

//------------------------------------------------------------------------------
bool ofxHTTPAuthScope::isSchemeSet() const {
    return bIsSchemeSet;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::clearScheme() {
    scheme.clear();
    bIsSchemeSet = false;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::setScheme(const string& _scheme) {
    scheme = _scheme;
    bIsSchemeSet = true;
}

//------------------------------------------------------------------------------
string ofxHTTPAuthScope::getScheme() const {
    return scheme;
}

//------------------------------------------------------------------------------
bool ofxHTTPAuthScope::isHostSet() const {
    return bIsHostSet;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::clearHost() {
    host.clear();
    bIsHostSet = false;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::setHost(const string& _host) {
    host = _host;
    bIsHostSet = true;
}

//------------------------------------------------------------------------------
string ofxHTTPAuthScope::getHost() const {
    return host;
}

//------------------------------------------------------------------------------
bool ofxHTTPAuthScope::isPortSet() const {
    return bIsPortSet;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::setPort(unsigned short _port) {
    port = _port;
    bIsPortSet = true;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::clearPort() {
    port = 0;
    bIsPortSet = false;
}

//------------------------------------------------------------------------------
unsigned short ofxHTTPAuthScope::getPort() const {
    return port;
}

//------------------------------------------------------------------------------
bool ofxHTTPAuthScope::isRealmSet() const {
    return bIsRealmSet;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::setRealm(const string& _realm) {
    realm = _realm;
    bIsRealmSet = true;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::clearRealm() {
    realm.clear();
    bIsRealmSet = false;
}

//------------------------------------------------------------------------------
string ofxHTTPAuthScope::getRealm() const {
    return realm;
}

//------------------------------------------------------------------------------
bool ofxHTTPAuthScope::isAuthTypeSet() const {
    return bIsAuthTypeSet;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::setAuthType(ofxHTTPAuthType _authType) {
    authType = _authType;
    bIsAuthTypeSet = true;
}

//------------------------------------------------------------------------------
void ofxHTTPAuthScope::clearAuthType() {
    authType = BASIC;
    bIsAuthTypeSet = false;
}

//------------------------------------------------------------------------------
ofxHTTPAuthType ofxHTTPAuthScope::getAuthType() const {
    return authType;
}

//------------------------------------------------------------------------------
bool ofxHTTPAuthScope::operator== (const ofxHTTPAuthScope& scope) const {
    if((isSchemeSet()   != scope.isSchemeSet())   || (scheme   != scope.scheme))   return false;
    if((isHostSet()     != scope.isHostSet())     || (host     != scope.host))     return false;
    if((isPortSet()     != scope.isPortSet())     || (port     != scope.port))     return false;
    if((isRealmSet()    != scope.isRealmSet())    || (realm    != scope.realm))    return false;
    if((isAuthTypeSet() != scope.isAuthTypeSet()) || (authType != scope.authType)) return false;
    return true;
}

//------------------------------------------------------------------------------
int ofxHTTPAuthScope::match(const ofxHTTPAuthScope& scope) const {
    // from org/apache/http/auth/AuthScope.html
    int factor = 0;
    
    if((isAuthTypeSet() && scope.isAuthTypeSet() && authType == scope.authType)) {
        factor += 1;
    } else if(isAuthTypeSet() && scope.isAuthTypeSet()) {
        return -1;
    }
    
    if((isRealmSet() && scope.isRealmSet() && realm == scope.realm)) {
        factor += 2;
    } else if(isRealmSet() && scope.isRealmSet()) {
        return -1;
    }

    if (isSchemeSet() && scope.isSchemeSet() && scheme == scope.scheme) {
        // not sure this is the best ...
        // why is this more important than host?
        factor += 3;
    } else if(isSchemeSet() && scope.isSchemeSet()) {
        return -1;
    }

    if((isPortSet() && scope.isPortSet() && port == scope.port)) {
        factor += 4;
    } else if(isPortSet() && scope.isPortSet()) {
        return -1;
    }
    
    if (isHostSet() && scope.isHostSet() && host == scope.host) {
        factor += 8;
    } else if(isHostSet() && scope.isHostSet()) {
        return -1;
    }

    return factor;
}

//------------------------------------------------------------------------------
bool ofxHTTPAuthScope::operator!= (const ofxHTTPAuthScope& scope) const {
    return !((*this)==scope);
}

//------------------------------------------------------------------------------
bool ofxHTTPAuthScope::operator< (const ofxHTTPAuthScope& scope) const {
    return match(scope) < 0;
}

//------------------------------------------------------------------------------
string ofxHTTPAuthScope::toString() const {
    stringstream ss;

    ss << "Scheme: " << (bIsSchemeSet ? getScheme() : "Any");

    ss << " Host: " << (bIsHostSet ? getHost() : "Any");

    ss << " Port: ";
    
    if(bIsPortSet) {
        ss << getPort();
    } else {
        ss << "Any";
    }

    ss << " Realm: " << (bIsRealmSet ? getRealm() : "Any");
    
    ss << " AuthType: ";

    if(bIsAuthTypeSet) {
        switch(getAuthType()) {
            case BASIC:
                ss << "BASIC";
                break;
            case DIGEST:
                ss << "DIGEST";
                break;
            default:
                ss << "UNKNOWN";
        }
    } else {
        ss << "Any";
    }
    
    return ss.str();
}

