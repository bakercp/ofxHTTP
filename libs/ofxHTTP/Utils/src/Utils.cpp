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



Poco::Net::NameValueCollection Utils::splitTextPlainPost(const std::string& textPlain)
{
    // Here we handle
    Poco::Net::NameValueCollection nvc;
    std::string result;
    std::stringstream ss;

    ss << textPlain;

    while (std::getline(ss, result))
    {
        std::size_t pos = result.find("=");

        if (pos != std::string::npos)
        {
            std::string key = result.substr(0, pos);
            std::string value = result.substr(pos + 1);
            nvc.add(key, value);
        }
    }

    return nvc;
}



Poco::Net::NameValueCollection Utils::splitAndURLDecode(const std::string& encoded)
{
    Poco::Net::NameValueCollection nvc;

    std::vector<std::string> arguments = ofSplitString(encoded, "&", true);

    std::vector<std::string>::const_iterator iter = arguments.begin();

    while(iter != arguments.end())
    {
        std::vector<std::string> tokens = ofSplitString(*iter, "=", true);

        if(tokens.size() > 0)
        {
            std::string key;
            Poco::URI::decode(tokens[0], key);
            std::string value = "";

            if(tokens.size() > 1)
            {
                Poco::URI::decode(tokens[1], value);
            }

            nvc.add(key, value);
        }
        ++iter;
    }

    return nvc;
}


Poco::Net::NameValueCollection Utils::getQueryMap(const Poco::URI& uri)
{
    if(uri.empty())
    {
        return Poco::Net::NameValueCollection();
    }

    std::string query = uri.getRawQuery(); // we will decode later

    if(query.empty())
    {
        return Poco::Net::NameValueCollection();
    }

    return splitAndURLDecode(query);
}


void Utils::dumpHeaders(const Poco::Net::HTTPRequest& request,
                        const Poco::Net::HTTPResponse& response,
                        ofLogLevel logLevel)
{
    dumpNameValueCollection(request,logLevel);
    dumpNameValueCollection(response,logLevel);
}


void Utils::dumpHeaders(const Poco::Net::HTTPRequest& request,
                        ofLogLevel logLevel)
{
    dumpNameValueCollection(request, logLevel);
}


void Utils::dumpHeaders(const Poco::Net::HTTPResponse& response,
                        ofLogLevel logLevel)
{
    dumpNameValueCollection(response, logLevel);
}


void Utils::dumpNameValueCollection(const Poco::Net::NameValueCollection& nvc,
                                    ofLogLevel logLevel)
{
    if(logLevel >= ofGetLogLevel())
    {
        Poco::Net::NameValueCollection::ConstIterator iter = nvc.begin();
        ofLog(logLevel) << "Begin NameValueCollection =================";
        while(iter != nvc.end())
        {
            ofLog(logLevel) << (*iter).first << ":" << (*iter).second;
            ++iter;
        }
        ofLog(logLevel) << "End NameValueCollection =================";
    }
}


std::streamsize Utils::consume(std::istream& stream)
{
    Poco::NullOutputStream nos;
    return Poco::StreamCopier::copyStream(stream, std::cout);
}


} } // namespace ofx::HTTP
