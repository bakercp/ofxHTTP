#include "ofxHTTPStreamUtils.h"

//------------------------------------------------------------------------------
streamsize ofxHTTPStreamUtils::consume(ofxHTTPResponseStream* responseStream, size_t bufferSize) {
    if(responseStream != NULL && responseStream->hasResponseStream()) {
        return consume(*responseStream->getResponseStream(), bufferSize);
    } else {
        return 0; // error
    }
}

//------------------------------------------------------------------------------
streamsize ofxHTTPStreamUtils::consume(istream& istr, size_t bufferSize) {
    NullOutputStream nos;
    return StreamCopier::copyStream(istr, nos, bufferSize);
}

//------------------------------------------------------------------------------
streamsize ofxHTTPStreamUtils::copyToString(ofxHTTPResponseStream* responseStream, string& str, size_t bufferSize) {
    if(responseStream != NULL && responseStream->hasResponseStream()) {
        return copyToString(*responseStream->getResponseStream(),str,bufferSize);
    } else {
        return 0; // error
    }
}

//------------------------------------------------------------------------------
streamsize ofxHTTPStreamUtils::copyToString(istream& istr, string& str, size_t bufferSize) {
    return StreamCopier::copyToString(istr,str,bufferSize);
}

//------------------------------------------------------------------------------
streamsize ofxHTTPStreamUtils::copyTo(ofxHTTPResponseStream* responseStream, ostream& ostr, size_t bufferSize) {
    if(responseStream != NULL && responseStream->hasResponseStream()) {
        return copyTo(*responseStream->getResponseStream(),ostr,bufferSize);
    } else {
        return 0; // error
    }
}
//------------------------------------------------------------------------------
streamsize ofxHTTPStreamUtils::copyTo(istream& istr, ostream& ostr, size_t bufferSize) {
    return StreamCopier::copyStream(istr,ostr,bufferSize);
}