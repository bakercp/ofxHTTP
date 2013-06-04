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
#include <vector>
#include "Poco/Net/MediaType.h"
#include "ofUtils.h"


namespace ofx {
namespace HTTP {


class Compression {
public:
    enum Type {
        DEFLATE,
        GZIP
    };

    //    static compress(xxx, ofxHTTPCompressionType type);
    //    static decompress(xxx, ofxHTTPCompressionType type);
    // TODO

};


class ofxHTTPCompressorEntry {
public:
    ofxHTTPCompressorEntry(const Poco::Net::MediaType& _mediaType) : mediaType(_mediaType) { }
    virtual ~ofxHTTPCompressorEntry() { }

    void addCompressionType(Compression::Type compressionType) {
        removeCompressionType(compressionType);
        validCompressionTypes.push_back(compressionType);
    }
    
    void removeCompressionType(Compression::Type compressionType) {
        remove(validCompressionTypes.begin(),validCompressionTypes.end(),compressionType);
    }
    
    void clearCompressionTypes() {
        validCompressionTypes.clear();
    }
    
    Poco::Net::MediaType getMediaType() const { return mediaType; }
    vector<Compression::Type> getValidCompressionTypes() const { return validCompressionTypes; }
    
    string getValidCompressionTypesAsCSV() const {
        string outputString;
        vector<string> compressionTypes;
        vector<Compression::Type>::const_iterator iter = validCompressionTypes.begin();
        while(iter != validCompressionTypes.end()) {
            switch(*iter) {
                case Compression::DEFLATE:
                    outputString += "deflate";
                    break;
                case Compression::GZIP:
                    outputString += "gzip";
                    break;
                default:
                    break;
            }
            
            if(iter + 1 != validCompressionTypes.end()) {
                outputString += ", ";
            }
            ++iter;
        }
        return outputString;
    }
    
private:
    Poco::Net::MediaType mediaType;
    vector<Compression::Type> validCompressionTypes;
};


} }
