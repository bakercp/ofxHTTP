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

#pragma once

#include <string>
#include <map>

#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "ofUtils.h"

#include "ofxBaseHTTPSessionManager.h"

using std::string;
using std::map;

using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::MD5Engine;
using Poco::Net::HTTPServerRequest;

template<class T, class B>
struct DerivedFrom {
    static void constraints(T* p) { B* pb = p; }
    DerivedFrom() { void(*p)(T*) = constraints; }
};

class ofxBaseHTTPServerSessionData {
public:
    ofxBaseHTTPServerSessionData() { }
    virtual ~ofxBaseHTTPServerSessionData() { }
    
    virtual bool update(HTTPServerRequest& request, HTTPServerResponse& response) = 0;
};

//class ofxDefaultHTTPServerSession {
//public:
//    ofxDefaultHTTPServerSession(const string& _sessionId, unsigned long long _lastConnected) :
//    sessionId(_sessionId),
//    lastConnected(_lastConnected)
//    { }
//    virtual ~ofxDefaultHTTPServerSession() { }
//
//    string getSessionId() const { return sessionId; }
//    void setSessionId(const string& _sessionId) { sessionId = _sessionId; }
//    
//    unsigned long long getLastConnected() const { return lastConnected; }
//    void setLastConnected(unsigned long long _lastConnected) { lastConnected = _lastConnected; }
//    
//protected:
//    string sessionId;
//    unsigned long long lastConnected;
//};

template<typename SessionDataType>
class ServerDefaultSessionManager : public ofxBaseHTTPServerSessionManager {
public:
    ServerDefaultSessionManager(const string& _sessionKeyName = "session_key");
    virtual ~ServerDefaultSessionManager();
    
    bool update(HTTPServerRequest& request, HTTPServerResponse& response);

    void hasSessionData(const string& sessionKey);
    void setSessionData(const string& sessionKey, const SessionDataType& sessionData);
    SessionDataType getSessionData(const string& sessionKey);
    
    string getSessionKey(const HTTPServerRequest& request);

protected:
    ofMutex mutex;
    
    string sessionKeyName;
    
    map<string,SessionDataType> sessions;
    
};

template<typename SessionDataType>
ServerDefaultSessionManager<SessionDataType>::ServerDefaultSessionManager(const string& _sessionKeyName)
: sessionKeyName(_sessionKeyName) {
    // we use this check to make sure that SessionDataType extends ofxBaseHTTPServerSessionData
    // if it does not extend ofxBaseHTTPServerSessionData, then the compiler will complain
    DerivedFrom<SessionDataType,ofxBaseHTTPServerSessionData>();
}

template<typename SessionDataType>
ServerDefaultSessionManager<SessionDataType>::~ServerDefaultSessionManager() { }

template<typename SessionDataType>
bool ServerDefaultSessionManager<SessionDataType>::update(HTTPServerRequest& request,
                                                                 HTTPServerResponse& response) {
    ofScopedLock lock(mutex);
    
    
//    string sessionKey = getSessionKey(request);
//    if(hasSessionData(sessionKey)) {
//        SessionDataType
//    }
//    
//    hasSessionData()
//    
//    update(request,response)
    
    return true;
}

template<typename SessionDataType>
void ServerDefaultSessionManager<SessionDataType>::setSessionData(const string& sessionKey,
                                                                         const SessionDataType& sessionData) {
    ofScopedLock lock(mutex);
    sessions[sessionKey] = sessionData;
}

template<typename SessionDataType>
SessionDataType ServerDefaultSessionManager<SessionDataType>::getSessionData(const string& sessionKey) {
    ofScopedLock lock(mutex);
    
    map<string,SessionDataType>::iterator iter = sessions.find(sessionKey);
   
    
    if(iter != sessions.end()) {
        return *iter;
    } else {
        SessionDataType emptyData;
        return emptyData;
    }
}


template<typename SessionDataType>
string ServerDefaultSessionManager<SessionDataType>::getSessionKey(const HTTPServerRequest& request) {
    
    
    // TODO: use Poco::UDID?
    MD5Engine md5;
    DigestOutputStream ostr(md5);
    // md5(micros + client address)
    ostr << ofGetSystemTimeMicros() << request.clientAddress().host().toString();
    ostr.flush(); // Ensure everything gets passed to the digest engine
    const DigestEngine::Digest& digest = md5.digest(); // obtain result
    return DigestEngine::digestToHex(digest);
}