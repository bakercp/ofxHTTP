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

#include "Poco/Net.h"
//#include "Poco/SingletonHolder.h"

using Poco::Net;
//using Poco::SingletonHolder;

class ofxSSLManager {
public:
    struct Settings;
    
    ofxSSLManager() {
//		Net::initializeSSL();
//        
//        Poco::Path mycert( Poco::Util::ServerApplication::instance().config().getString(“application.dir”) );
//        
//        
//        
//        mycert.append(“mycert.pem”);
//        Poco::SharedPtr pConsoleHandler = new KeyConsoleHandler(true);
//        Poco::SharedPtr pCertHandler = new AcceptCertificateHandler(true);
//        SSLManager::instance().initializeServer(pConsoleHandler, pCertHandler, NULL);
//        Context::Ptr pContext = new Context(Context::SERVER_USE, mycert.toString(), mycert.toString(), “”, Context::VERIFY_RELAXED, 9, false, “ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH”);
//        SSLManager::instance().initializeServer(pConsoleHandler, pCertHandler, pContext);

        
	}
	
	~ofxSSLManager() {
        Net::uninitializeSSL();
    }
    
//    static ofxSSLManager& instance()
//    {
//        static SingletonHolder<ofxSSLManager> sh;
//        return *sh.get();
//    }
    
    struct Settings {
        string privateKeyFile;
        string certificateFile;
        string caLocation;
        VerificationMode verificationMode;
        int verificationDepth;
        bool bLoadDefaultCAs;
        string cipherList;
        
//		const std::string& privateKeyFile,
//		const std::string& certificateFile,
//		const std::string& caLocation,
//		VerificationMode verificationMode = VERIFY_RELAXED,
//		int verificationDepth = 9,
//		bool loadDefaultCAs = false,
//		const std::string& cipherList = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

        bool bCacheSessions;
        string sessionIdContext;
        int sessionCacheSize;
        bool bRequireTLSv1;
        
//        openSSL.server.privateKeyPassphraseHandler.name = KeyFileHandler
//        openSSL.server.privateKeyPassphraseHandler.options.password = secret
//        openSSL.server.invalidCertificateHandler.name = AcceptCertificateHandler
//        openSSL.server.extendedVerification = false
//        openSSL.server.cacheSessions = true
//        openSSL.server.sessionIdContext = ${application.name}
//        openSSL.server.sessionCacheSize = 100
//        openSSL.server.requireTLSv1 = false

        
        
        Settings();
    };
    
    
};

