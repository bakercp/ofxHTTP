#include "ofxHTTPUtils.h"

//------------------------------------------------------------------------------
NameValueCollection ofGetQueryMap(const URI& uri) {
    NameValueCollection nvc;
    
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
void ofDumpRequestHeaders(const ofxHTTPServerExchange& exchange, ofLogLevel logLevel) {
    if(logLevel >= ofGetLogLevel()) {
        ofLog(logLevel) << "Being ofDumpRequestHeaders =================" << endl;
        NameValueCollection::ConstIterator iter = exchange.request.begin();
        while(iter != exchange.request.end()) {
            ofLog(logLevel) << (*iter).first << ":" << (*iter).second;
            ++iter;
        }
        ofLog(logLevel) << "End ofDumpRequestHeaders =================" << endl;
    }
}

//------------------------------------------------------------------------------
void ofDumpReponseHeaders(const ofxHTTPServerExchange& exchange, ofLogLevel logLevel) {
    if(logLevel >= ofGetLogLevel()) {
        NameValueCollection::ConstIterator iter = exchange.response.begin();
        ofLog(logLevel) << "Begin ofDumpReponseHeaders =================" << endl;
        while(iter != exchange.request.end()) {
            ofLog(logLevel) << (*iter).first << ":" << (*iter).second;
            ++iter;
        }
        ofLog(logLevel) << "End ofDumpReponseHeaders =================" << endl;
    }
}