//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "ofConstants.h"
#include "ofJson.h"


namespace ofx {
namespace HTTP {


/// \brief A class representing generic JSON Web Token (JWT) data.
class JSONWebTokenData
{
public:
    /// \brief Create a default piece of JSON Web Token data.
    JSONWebTokenData();

    /// \brief Destroy the JSONWebTokenData.
    virtual ~JSONWebTokenData();

    /// \brief Get the data as a Base64-encoded URL safe string.
    ///
    /// Applies the `base64url` encoding to the data.
    ///
    /// \sa https://tools.ietf.org/html/rfc4648#page-7
    std::string asBase64URLEncodedString() const;

    /// \brief Get the JSON data as a string.
    /// \param indent The number of spaces to indent fields (-1 is no indentation).
    /// \returns the data as a UTF-8 encoded std::string.
    std::string asString(int indent = -1) const;

    /// \brief Return the raw json data.
    const ofJson& data() const;

    /// \brief Clear the given field name.
    /// \param name The field name to clear.
    void clear(const std::string& name);

    /// \brief Set the given field name.
    /// \param name The field name to set.
    /// \param value The value to set.
    /// \tparam The Data type to set.
    template<typename Type>
    void set(const std::string& name, const Type& value)
    {
        _data[name] = value;
    }

protected:
    /// \brief The internal JSON data.
    ofJson _data;

};


/// \brief A JSON Web Token (JWT) Headeer.
class JSONWebTokenHeader: public JSONWebTokenData
{
public:
    /// \brief Create an empty JSONWebTokenHeader.
    JSONWebTokenHeader();

    /// \brief Destroy the JSONWebTokenHeader.
    virtual ~JSONWebTokenHeader();

    /// \brief Set the type `type` field with the given \p type.
    /// \param type The header type.
    void setType(const std::string& type);

    /// \brief Set the content type `cty` with the given \p contentType.
    /// \param contentType The content type to set.
    void setContentType(const std::string& contentType);

    /// \brief JSON Web Token header type.
    static const std::string TYP;

    /// \brief The content type.
    static const std::string CTY;

};


/// \brief A JSON Web Signature (JWT) Header.
class JSONWebSignatureHeader: public JSONWebTokenHeader
{
public:
    /// \brief The hashing algorithm to use.
    enum class Algorithm : uint8_t
    {
        /// \brief RSA SHA-256 (aka SHA256withRSA or RSASSA-PKCS1-V1_5-SIGN).
        RS256
    };

    /// \brief Create an empty JSONWebTokenHeader.
    JSONWebSignatureHeader();

    /// \brief Destroy the JSONWebTokenHeader.
    virtual ~JSONWebSignatureHeader();

    /// \brief Set the algorithm `alg` field with the given \p algorithm.
    /// \param algorithm The signature algorithm to set.
    void setAlgorithm(Algorithm algorithm);

    /// \brief Set the key id `kid` field with the given \p keyId.
    /// \param keyId The key id to set.
    void setKeyId(const std::string& keyId);

    /// \brief Algorithm field name.
    static const std::string ALG;

    /// \brief JSON Web Key field name.
    static const std::string JKU;

    /// \brief JSON Web Key.
    static const std::string JWK;

    /// \brief Key ID field name.
    static const std::string KID;

    /// \brief X.509 URL.
    static const std::string X5U;

    /// \brief X.509 URL.
    static const std::string X5T;

    /// \brief X.509 URL.
    static const std::string X5C;

    /// \brief Critical types.
    static const std::string CRIT;

protected:
    /// \brief Get the string represetnation of an algorithm.
    /// \algorithm The algorithm to convert.
    /// \returns the string representation of an algorithm.
    static std::string toString(Algorithm algorithm);

};


/// \brief A JSON Web Token (JWT) Payload.
class JSONWebTokenPayload: public JSONWebTokenData
{
public:
    /// \brief Create an empty JSONWebTokenPayload.
    JSONWebTokenPayload();

    /// \brief Destroy the JSONWebTokenPayload.
    virtual ~JSONWebTokenPayload();

    /// \brief Set the issuer `iss` field with the given \p issuer.
    /// \param issuer The issuer to set.
    void setIssuer(const std::string& issuer);

    /// \brief Set the audience `aud` field with the given \p audience.
    ///
    /// Sets the `aud` field the a space-delimited list of the audience
    /// that the application requests.
    ///
    /// \param audience The audience to set.
    void setAudience(const std::vector<std::string>& audience);

    /// \brief Set the audience `aud` field with the given \p audience.
    /// \param audience The audience to set.
    void setAudience(const std::string& audience);

    /// \brief Set the audience `jti` field with the given \p id.
    /// \param id The id to set.
    void setId(const std::string& id);

    /// \brief Set the issued at `iat` field with the given \p time;
    /// \param time The issued at epoch time in seconds.
    /// \sa https://en.wikipedia.org/wiki/Unix_time
    void setIssuedAtTime(uint64_t time);

    /// \brief Set the expiration time `exp` field with the given \p time;
    /// \param time The expiration epoch time in seconds.
    /// \sa https://en.wikipedia.org/wiki/Unix_time
    void setExpirationTime(uint64_t time);

    /// \brief Set the not before time `nbf` field with the given \p time;
    /// \param time The not before epoch time in seconds.
    /// \sa https://en.wikipedia.org/wiki/Unix_time
    void setNotBeforeTime(uint64_t time);

    /// \brief Set the claim type `typ` field with the given \p type.
    /// \param type the claim type to set.
    void setType(const std::string& type);

    /// \brief Set the subject `sub` field with the given \p subject.
    void setSubject(const std::string& subject);

    /// \brief Issuer claim that identifies the audience.
    static const std::string ISS;

    /// \brief Audience claim that identifies the audience.
    static const std::string AUD;

    /// \brief JWT ID.
    static const std::string JTI;

    /// \brief Issued at claim time (UNIX time).
    static const std::string IAT;

    /// \brief Expiration time (UNIX time).
    static const std::string EXP;

    /// \brief Not before time (UNIX time).
    static const std::string NBF;

    /// \brief JSON Web Token type.
    static const std::string TYP;

    /// \brief The claim identifying the subject.
    static const std::string SUB;

};


/// \brief A JSON Web Token (JWT) generator.
///
/// JSON Web Tokens are submitted to authentication servers in order to acquire
/// an authentication token for a given API. The authentication token is then
/// submitted with subsequent API requests.
///
///
/// JWTs have the following format:
///
/// aaaaaaaaaa.bbbbbbbbbbb.ccccccccccc
///
/// ^^^^^^^^^^ ^^^^^^^^^^^ ^^^^^^^^^^^
///   header     payload    signature
///
/// \sa http://self-issued.info/docs/draft-ietf-oauth-json-web-token.html
class JSONWebToken
{
public:
    /// \brief Generate a signed JSON Web Token.
    /// \param privateKey The private key.
    /// \param privateKeyPassphrase The private key pass phrase.
    /// \param header The JSON Web Signature header.
    /// \param payload The JSON Web Token payload.
    /// \returns A signed JWT or an empty string on error.
    static std::string generateToken(const std::string& privateKey,
                                     const std::string& privateKeyPassphrase,
                                     const JSONWebSignatureHeader& header,
                                     const JSONWebTokenPayload& payload);

};


} } // namespace ofx::HTTP
