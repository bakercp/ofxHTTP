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


#include "Utils.h"


namespace ofx {
namespace HTTP {

    
//------------------------------------------------------------------------------
Poco::Net::NameValueCollection ofGetQueryMap(const Poco::URI& uri) {
    Poco::Net::NameValueCollection nvc;
    
    if(uri.empty()) return nvc;

    string query = uri.getQuery();

    if(query.empty()) return nvc;

    vector<string> arguments = ofSplitString(query,"&",true);
    
    vector<string>::const_iterator iter = arguments.begin();
    
    while(iter != arguments.end()) {
        vector<string> tokens = ofSplitString(*iter,"=",true);
        if(tokens.size() > 0) {
            string key   = tokens[0];
            string value = "";
            if(tokens.size() > 1) {
                value = tokens[1];
            }
            nvc.add(key,value);
        }
        ++iter;
    }
    return nvc;
}

//------------------------------------------------------------------------------
void ofDumpRequestHeaders(const ServerExchange& exchange, ofLogLevel logLevel) {
    if(logLevel >= ofGetLogLevel()) {
        ofLog(logLevel) << "Being ofDumpRequestHeaders =================" << endl;
        Poco::Net::NameValueCollection::ConstIterator iter = exchange.request.begin();
        while(iter != exchange.request.end()) {
            ofLog(logLevel) << (*iter).first << ":" << (*iter).second;
            ++iter;
        }
        ofLog(logLevel) << "End ofDumpRequestHeaders =================" << endl;
    }
}

//------------------------------------------------------------------------------
void ofDumpReponseHeaders(const ServerExchange& exchange, ofLogLevel logLevel) {
    if(logLevel >= ofGetLogLevel()) {
        Poco::Net::NameValueCollection::ConstIterator iter = exchange.response.begin();
        ofLog(logLevel) << "Begin ofDumpReponseHeaders =================" << endl;
        while(iter != exchange.request.end()) {
            ofLog(logLevel) << (*iter).first << ":" << (*iter).second;
            ++iter;
        }
        ofLog(logLevel) << "End ofDumpReponseHeaders =================" << endl;
    }
}


} }
