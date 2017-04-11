//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/OAuth10Credentials.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofx/IO/ByteBufferUtils.h"
#include "Poco/Exception.h"
#include "Poco/FileStream.h"
#include "ofLog.h"
#include "ofUtils.h"


namespace ofx {
namespace HTTP {


OAuth10Credentials::OAuth10Credentials()
{
}


OAuth10Credentials::OAuth10Credentials(const std::string& consumerKey,
                                       const std::string& consumerSecret):
    _consumerKey(consumerKey),
    _consumerSecret(consumerSecret)
{
}


OAuth10Credentials::OAuth10Credentials(const std::string& consumerKey,
                                       const std::string& consumerSecret,
                                       const std::string& accessToken,
                                       const std::string& accessTokenSecret):
    _consumerKey(consumerKey),
    _consumerSecret(consumerSecret),
    _accessToken(accessToken),
    _accessTokenSecret(accessTokenSecret)
{
}


OAuth10Credentials::~OAuth10Credentials()
{
}


std::string OAuth10Credentials::consumerKey() const
{
    return _consumerKey;
}


std::string OAuth10Credentials::consumerSecret() const
{
    return _consumerSecret;
}


std::string OAuth10Credentials::accessToken() const
{
    return _accessToken;
}


std::string OAuth10Credentials::accessTokenSecret() const
{
    return _accessTokenSecret;
}


std::string OAuth10Credentials::owner() const
{
    return _owner;
}


std::string OAuth10Credentials::ownerId() const
{
    return _ownerId;
}


OAuth10Credentials OAuth10Credentials::fromJSON(const ofJson& json)
{
    OAuth10Credentials credentials;

    auto iter = json.cbegin();
    while (iter != json.cend())
    {
        const auto& key = iter.key();
        const auto& value = iter.value();

        if (key == "consumerKey") credentials._consumerKey = value;
        else if (key == "consumerSecret" || key == "consumer_secret") credentials._consumerSecret = value;
        else if (key == "accessToken" || key == "access_token") credentials._accessToken = value;
        else if (key == "accessTokenSecret" || key == "access_token_secret") credentials._accessTokenSecret = value;
        else if (key == "owner") credentials._owner = value;
        else if (key == "ownerId" || key == "owner_id" ) credentials._ownerId = value;
        else ofLogWarning("Credentials::fromJSON") << "Unknown key: " << key << std::endl << value.dump(4);
        ++iter;
    }

    return credentials;
}


ofJson OAuth10Credentials::toJSON(const OAuth10Credentials& credentials)
{
    ofJson json;

    json["consumer_key"] = credentials._consumerKey;
    json["consumer_secret"] = credentials._consumerSecret;
    json["access_token"] = credentials._accessToken;
    json["access_token_secret"] = credentials._accessTokenSecret;
    json["owner"] = credentials._owner;
    json["owner_id"] = credentials._ownerId;

    return json;
}


OAuth10Credentials OAuth10Credentials::fromFile(const std::filesystem::path& credentialsFile)
{
    OAuth10Credentials credentials;

    try
    {
        credentials = fromJSON(ofJson::parse(ofBufferFromFile(credentialsFile)));
    }
    catch (const std::exception& exception)
    {
        ofLogError("Credentials::fromFile") << exception.what();
    }

    return credentials;
}


bool OAuth10Credentials::toFile(const OAuth10Credentials& credentials,
                                const std::filesystem::path& credentialsFile)
{
    ofJson json = toJSON(credentials);
    return ofSaveJson(credentialsFile.string(), json);
}


} } // namespace ofx::HTTP
