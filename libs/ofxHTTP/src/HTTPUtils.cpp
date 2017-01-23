//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {


Poco::Net::NameValueCollection HTTPUtils::splitTextPlainPost(const std::string& textPlain)
{
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



Poco::Net::NameValueCollection HTTPUtils::splitAndURLDecode(const std::string& encoded)
{
    // TODO use code from Poco::Net::HTMLForm

    Poco::Net::NameValueCollection nvc;

    auto arguments = ofSplitString(encoded, "&", true);

    for (const auto& argument: arguments)
    {
        std::vector<std::string> tokens = ofSplitString(argument, "=", true);

        if (tokens.size() > 0)
        {
            std::string key;
            std::string value;

            Poco::URI::decode(tokens[0], key);

            if (tokens.size() > 1)
            {
                Poco::URI::decode(tokens[1], value);
            }

            nvc.add(key, value);
        }
    }

    return nvc;
}


Poco::Net::NameValueCollection HTTPUtils::getQueryMap(const Poco::URI& uri)
{
    if (uri.empty())
    {
        return Poco::Net::NameValueCollection();
    }

    std::string query = uri.getRawQuery(); // we will decode later

    if (query.empty())
    {
        return Poco::Net::NameValueCollection();
    }

    return splitAndURLDecode(query);
}



std::string HTTPUtils::makeQueryString(const Poco::Net::NameValueCollection& query)
{
    std::stringstream ostr;

    Poco::Net::NameValueCollection::ConstIterator it = query.begin();

    while (it != query.end())
    {
        if (it != query.begin())
            ostr << "&";

        std::string name;
        Poco::URI::encode(it->first, "=&+;", name);
        std::string value;
        Poco::URI::encode(it->second, "=&+;", value);
        ostr << name << "=" << value;
        ++it;
    }

    return ostr.str();
}


void HTTPUtils::dumpHeaders(const Poco::Net::HTTPRequest& request,
                            const Poco::Net::HTTPResponse& response,
                            ofLogLevel logLevel)
{
    dumpNameValueCollection(request, logLevel);
    dumpNameValueCollection(response, logLevel);
}


void HTTPUtils::dumpHeaders(const Poco::Net::HTTPRequest& request,
                            ofLogLevel logLevel)
{
    dumpNameValueCollection(request, logLevel);
}


void HTTPUtils::dumpHeaders(const Poco::Net::HTTPResponse& response,
                            ofLogLevel logLevel)
{
    dumpNameValueCollection(response, logLevel);
}


void HTTPUtils::dumpNameValueCollection(const Poco::Net::NameValueCollection& nvc,
                                        ofLogLevel logLevel)
{
    if (logLevel >= ofGetLogLevel())
    {
        for (const auto& entry: nvc)
        {
            ofLog(logLevel) << entry.first << ": " << entry.second;
        }
    }
}


std::streamsize HTTPUtils::consume(std::istream& stream)
{
    Poco::NullOutputStream nos;
    return Poco::StreamCopier::copyStream(stream, nos);
}


} } // namespace ofx::HTTP
