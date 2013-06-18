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
//
//
//#pragma once
//
//
//#include <vector>
//#include "ofxBaseHTTPServerAuthenticationEngine.h"
//#include "ofxHTTPCredentials.h"
//
//
//class BasicServerAuthenticator : public ofxBaseHTTPServerAuthenticationEngine {
//public:
//    struct Settings;
//    
//    ServerBasicAuthenticator(const Settings& _settings = Settings()) : settings(_settings) { }
//    virtual ~ServerBasicAuthenticator() { }
//    
//    bool isAuthenticated(HTTPServerRequest& request, HTTPServerResponse& response) {
//        URI uri(request.getURI());
//        
//        const string path = uri.getPath(); // just get the path
//        
//        if(request.hasCredentials()) {
//            HTTPBasicCredentials credentials(request);
//            
//            vector<ofxHTTPCredentials>::const_iterator iter = settings.credentials.begin();
//
//            while(iter != settings.credentials.end()) {
//                if((*iter).getUsername() == credentials.getUsername() &&
//                   (*iter).getPassword() == credentials.getPassword()) {
//                    // add an authentication cookie?
//                    return true;
//                }
//                ++iter;
//            }
//            
//            // couldn't find a match
//            response.setStatusAndReason(HTTPResponse::HTTP_UNAUTHORIZED);
//            sendErrorResponse(response);
//            return false;
//        } else {
//            // no credentials?  require authentication
//            response.requireAuthentication(settings.realm);
//            response.setContentLength(0);
//            response.send();
//            return false;
//        }
//
//    }
//    
//    struct Settings {
//        string realm;
//        vector<ofxHTTPCredentials> credentials;
//        Settings();
//    };
//
//protected:
//    Settings settings;
//
//};
//