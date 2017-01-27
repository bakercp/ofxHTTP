//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/JSONWebToken.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "ofx/HTTP/HTTPUtils.h"
#include "ofx/IO/Base64Encoding.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofLog.h"


namespace ofx {
namespace HTTP {


JSONWebTokenData::JSONWebTokenData()
{
}


JSONWebTokenData::~JSONWebTokenData()
{
}


std::string JSONWebTokenData::asBase64URLEncodedString() const
{
    return IO::Base64Encoding::encode(_data.dump(), true);
}


std::string JSONWebTokenData::asString(int indent) const
{
    return _data.dump(indent);
}


const ofJson& JSONWebTokenData::data() const
{
    return _data;
}


void JSONWebTokenData::clear(const std::string& name)
{
    _data.erase(_data.find(name));
}
    

const std::string JSONWebTokenHeader::TYP = "typ";
const std::string JSONWebTokenHeader::CTY = "cty";


JSONWebTokenHeader::JSONWebTokenHeader()
{
}


JSONWebTokenHeader::~JSONWebTokenHeader()
{
}


void JSONWebTokenHeader::setType(const std::string& type)
{
    set(TYP, type);
}


void JSONWebTokenHeader::setContentType(const std::string& contentType)
{
    set(CTY, contentType);
}


const std::string JSONWebSignatureHeader::ALG = "alg";
const std::string JSONWebSignatureHeader::JKU = "jku";
const std::string JSONWebSignatureHeader::JWK = "jwk";
const std::string JSONWebSignatureHeader::KID = "kid";
const std::string JSONWebSignatureHeader::X5U = "x5u";
const std::string JSONWebSignatureHeader::X5T = "x5t";
const std::string JSONWebSignatureHeader::X5C = "x5c";
const std::string JSONWebSignatureHeader::CRIT = "crit";


JSONWebSignatureHeader::JSONWebSignatureHeader()
{
}


JSONWebSignatureHeader::~JSONWebSignatureHeader()
{
}


void JSONWebSignatureHeader::setAlgorithm(Algorithm algorithm)
{
    set(ALG, toString(algorithm));
}


void JSONWebSignatureHeader::setKeyId(const std::string& keyId)
{
    set(KID, keyId);
}


std::string JSONWebSignatureHeader::toString(Algorithm algorithm)
{
    switch (algorithm)
    {
        case Algorithm::RS256:
            return "RS256";
        default:
            return "UNKNOWN";
    }

    return "UNKNOWN";
}


const std::string JSONWebTokenPayload::ISS = "iss";
const std::string JSONWebTokenPayload::AUD = "aud";
const std::string JSONWebTokenPayload::JTI = "jti";
const std::string JSONWebTokenPayload::IAT = "iat";
const std::string JSONWebTokenPayload::EXP = "exp";
const std::string JSONWebTokenPayload::NBF = "nbf";
const std::string JSONWebTokenPayload::TYP = "typ";
const std::string JSONWebTokenPayload::SUB = "sub";
//const std::string JSONWebTokenPayload::SCOPE = "scope";


JSONWebTokenPayload::JSONWebTokenPayload()
{
}


JSONWebTokenPayload::~JSONWebTokenPayload()
{
}


void JSONWebTokenPayload::setIssuer(const std::string& issuer)
{
    set(ISS, issuer);
}


void JSONWebTokenPayload::setAudience(const std::vector<std::string>& audience)
{
    // Find remove duplicate scopes.
    std::vector<std::string> _audience = audience;
    auto it = std::unique(_audience.begin(), _audience.end());
    _audience.resize(std::distance(_audience.begin(), it));
    setAudience(HTTPUtils::join(_audience, " "));
}


void JSONWebTokenPayload::setAudience(const std::string& audience)
{
    set(AUD, audience);
}


void JSONWebTokenPayload::setId(const std::string& id)
{
    set(JTI, id);
}


void JSONWebTokenPayload::setIssuedAtTime(uint64_t time)
{
    set(IAT, time);
}


void JSONWebTokenPayload::setExpirationTime(uint64_t time)
{
    set(EXP, time);
}



void JSONWebTokenPayload::setNotBeforeTime(uint64_t time)
{
    set(NBF, time);
}


void JSONWebTokenPayload::setType(const std::string& type)
{
    set(TYP, type);
}



void JSONWebTokenPayload::setSubject(const std::string& subject)
{
    set(SUB, subject);
}


std::string JSONWebToken::generateToken(const std::string& privateKey,
                                        const std::string& privateKeyPassphrase,
                                        const JSONWebSignatureHeader& header,
                                        const JSONWebTokenPayload& payload)
{
    if (privateKey.empty())
    {
        ofLogError("JSONWebToken::generateToken") << "Private key is empty.";
        return "";
    }

    if (header.data()[JSONWebSignatureHeader::ALG].is_string())
    {
        std::string alg = header.data()[JSONWebSignatureHeader::ALG].get<std::string>();

        if (alg.compare("RS256") != 0)
        {
            ofLogError("JSONWebToken::generateToken") << "Signature algorithm: " << alg << " not supported.";
            return "";
        }
    }
    else
    {
        ofLogError("JSONWebToken::generateToken") << "No signature algorithm selected.";
        return "";
    }

    std::string encodedHeaderAndClaims;

    encodedHeaderAndClaims += header.asBase64URLEncodedString();
    encodedHeaderAndClaims += ".";
    encodedHeaderAndClaims += payload.asBase64URLEncodedString();

    std::istringstream _privateKey(privateKey);

    Poco::Crypto::RSAKey key(nullptr,
                             &_privateKey,
                             privateKeyPassphrase);

    Poco::Crypto::RSADigestEngine digestEngine(key, "SHA256");

    digestEngine.update(encodedHeaderAndClaims.data(),
                        encodedHeaderAndClaims.size());

    const Poco::DigestEngine::Digest& signature = digestEngine.signature();

    ofx::IO::ByteBuffer signatureBuffer(signature);
    ofx::IO::ByteBuffer encodedSignatureBuffer;

    ofx::IO::Base64Encoding encoder(true);
    encoder.encode(signatureBuffer, encodedSignatureBuffer);

    return encodedHeaderAndClaims + "." + encodedSignatureBuffer.toString();
}


} } // namespace ofx::HTTP
