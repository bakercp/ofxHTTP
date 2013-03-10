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
 
 ==============================================================================*/

#pragma once

#include <string>
#include <vector>

#include "Poco/Net/MediaType.h"

#include "ofUtils.h"

using std::remove;
using std::string;
using std::vector;

using Poco::Net::MediaType;

enum ofxHTTPCompressionType {
    DEFLATE,
    GZIP
};

class ofxHTTPCompressorEntry {
public:
    ofxHTTPCompressorEntry(const MediaType& _mediaType) : mediaType(_mediaType) { }
    virtual ~ofxHTTPCompressorEntry() { }

    void addCompressionType(ofxHTTPCompressionType compressionType) {
        removeCompressionType(compressionType);
        validCompressionTypes.push_back(compressionType);
    }
    
    void removeCompressionType(ofxHTTPCompressionType compressionType) {
        remove(validCompressionTypes.begin(),validCompressionTypes.end(),compressionType);
    }
    
    void clearCompressionTypes() {
        validCompressionTypes.clear();
    }
    
    MediaType getMediaType() const { return mediaType; }
    vector<ofxHTTPCompressionType> getValidCompressionTypes() const { return validCompressionTypes; }
    
    string getValidCompressionTypesAsCSV() const {
        string outputString;
        vector<string> compressionTypes;
        vector<ofxHTTPCompressionType>::const_iterator iter = validCompressionTypes.begin();
        while(iter != validCompressionTypes.end()) {
            switch(*iter) {
                case DEFLATE:
                    outputString += "deflate";
                    break;
                case GZIP:
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
    MediaType mediaType;
    vector<ofxHTTPCompressionType> validCompressionTypes;
};

class ofxHTTPCompression {
public:

//    static compress(xxx, ofxHTTPCompressionType type);
//    static decompress(xxx, ofxHTTPCompressionType type);
    // TODO
    
};


