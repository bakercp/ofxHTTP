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


#include "ofx/HTTP/Utils/Utils.h"


namespace ofx {
namespace HTTP {

    
//------------------------------------------------------------------------------
Poco::Net::NameValueCollection Utils::getQueryMap(const Poco::URI& uri)
{
    Poco::Net::NameValueCollection nvc;
    
    if(uri.empty())
    {
        return nvc;
    }

    std::string query = uri.getQuery();

    if(query.empty())
    {
        return nvc;
    }

    std::vector<std::string> arguments = ofSplitString(query,"&",true);
    
    std::vector<std::string>::const_iterator iter = arguments.begin();
    
    while(iter != arguments.end())
    {
        std::vector<std::string> tokens = ofSplitString(*iter,"=",true);
        if(tokens.size() > 0)
        {
            std::string key   = tokens[0];
            std::string value = "";
            if(tokens.size() > 1)
            {
                value = tokens[1];
            }
            nvc.add(key,value);
        }
        ++iter;
    }
    
    return nvc;
}


//------------------------------------------------------------------------------
void Utils::dumpHeaders(const Poco::Net::HTTPServerRequest& request,
                        const Poco::Net::HTTPServerResponse& response,
                        ofLogLevel logLevel)
{
    dumpHeaders(request,logLevel);
    dumpHeaders(response,logLevel);
}

//------------------------------------------------------------------------------
void Utils::dumpHeaders(const Poco::Net::HTTPServerRequest& request,
                        ofLogLevel logLevel)
{
    if(logLevel >= ofGetLogLevel())
    {
        ofLog(logLevel) << "Being ofDumpRequestHeaders =================" << endl;
        Poco::Net::NameValueCollection::ConstIterator iter = request.begin();

        while(iter != request.end())
        {
            ofLog(logLevel) << (*iter).first << ":" << (*iter).second;
            ++iter;
        }

        ofLog(logLevel) << "End ofDumpRequestHeaders =================" << endl;
    }
}

//------------------------------------------------------------------------------
void Utils::dumpHeaders(const Poco::Net::HTTPServerResponse& response,
                        ofLogLevel logLevel)
{
    if(logLevel >= ofGetLogLevel())
    {
        Poco::Net::NameValueCollection::ConstIterator iter = response.begin();
        ofLog(logLevel) << "Begin ofDumpReponseHeaders =================" << endl;
        while(iter != response.end())
        {
            ofLog(logLevel) << (*iter).first << ":" << (*iter).second;
            ++iter;
        }
        ofLog(logLevel) << "End ofDumpReponseHeaders =================" << endl;
    }
}


} } // namespace ofx::HTTP
