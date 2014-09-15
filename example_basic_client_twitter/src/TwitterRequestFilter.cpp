#include "TwitterRequestFilter.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/URI.h"
#include "Poco/SHA1Engine.h"
#include "Poco/HMACEngine.h"
#include "Poco/Base64Encoder.h"
#include "Poco/RandomStream.h"
#include "Poco/Timestamp.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"
#include "Poco/StreamCopier.h"
#include <sstream>
#include <map>
#include "ofx/HTTP/BaseRequest.h"
#include "ofx/HTTP/Context.h"


namespace ofx {


TwitterRequestFilter::TwitterRequestFilter()
{
}


TwitterRequestFilter::~TwitterRequestFilter()
{
}


void TwitterRequestFilter::setup(const std::string& consumerKey,
                    const std::string& consumerSecret,
                    const std::string& token,
                    const std::string& tokenSecret)
{
	_consumerKey    = consumerKey;
	_consumerSecret = consumerSecret;
	_token          = token;
	_tokenSecret    = tokenSecret;
}


void TwitterRequestFilter::requestFilter(HTTP::BaseRequest& request,
                                         HTTP::Context& context)
{
    sign(request, request.getForm(), request.getURI());
}


void TwitterRequestFilter::sign(Poco::Net::HTTPRequest& request,
                                const Poco::Net::HTMLForm& params,
                                const std::string& uri) const
{
	std::string nonce(createNonce());
	std::string timestamp(Poco::NumberFormatter::format(Poco::Timestamp().epochTime()));
	std::string signature(createSignature(request, params, uri, nonce, timestamp));
	std::string authorization(
                              Poco::format(
                                           "OAuth"
                                           " oauth_consumer_key=\"%s\","
                                           " oauth_nonce=\"%s\","
                                           " oauth_signature=\"%s\","
                                           " oauth_signature_method=\"HMAC-SHA1\","
                                           " oauth_timestamp=\"%s\","
                                           " oauth_token=\"%s\","
                                           " oauth_version=\"1.0\"",
                                           percentEncode(_consumerKey),
                                           percentEncode(nonce),
                                           percentEncode(signature),
                                           timestamp,
                                           percentEncode(_token)
                                           )
                              );
	request.set("Authorization", authorization);
}


std::string TwitterRequestFilter::createNonce() const
{
	std::ostringstream base64Nonce;
	Poco::Base64Encoder base64Encoder(base64Nonce);
	Poco::RandomInputStream randomStream;
	for (int i = 0; i < 32; ++i)
	{
		base64Encoder.put(randomStream.get());
	}
	base64Encoder.close();
	std::string nonce = base64Nonce.str();
	return Poco::translate(nonce, "+/=", "");
}


std::string TwitterRequestFilter::createSignature(Poco::Net::HTTPRequest& request,
                                                  const Poco::Net::HTMLForm& params,
                                                  const std::string& uri,
                                                  const std::string& nonce,
                                                  const std::string& timestamp) const
{
	std::map<std::string, std::string> paramsMap;
	paramsMap["oauth_consumer_key"]     = percentEncode(_consumerKey);
	paramsMap["oauth_nonce"]            = percentEncode(nonce);
	paramsMap["oauth_signature_method"] = "HMAC-SHA1";
	paramsMap["oauth_timestamp"]        = timestamp;
	paramsMap["oauth_token"]            = percentEncode(_token);
	paramsMap["oauth_version"]          = "1.0";

    if (Poco::Net::HTMLForm::ENCODING_URL == params.getEncoding())
    {
        for (Poco::Net::HTMLForm::ConstIterator it = params.begin(); it != params.end(); ++it)
        {
            paramsMap[percentEncode(it->first)] = percentEncode(it->second);
        }
    }

	std::string paramsString;
	for (std::map<std::string, std::string>::const_iterator it = paramsMap.begin();
         it != paramsMap.end();
         ++it)
	{
		if (it != paramsMap.begin()) paramsString += '&';
		paramsString += it->first;
		paramsString += "=";
		paramsString += it->second;
	}

	std::string signatureBase = request.getMethod();
	signatureBase += '&';
	signatureBase += percentEncode(uri);
	signatureBase += '&';
	signatureBase += percentEncode(paramsString);

	std::string signingKey;
	signingKey += percentEncode(_consumerSecret);
	signingKey += '&';
	signingKey += percentEncode(_tokenSecret);
	
	Poco::HMACEngine<Poco::SHA1Engine> hmacEngine(signingKey);
	hmacEngine.update(signatureBase);
	Poco::DigestEngine::Digest digest = hmacEngine.digest();
	std::ostringstream digestBase64;
	Poco::Base64Encoder base64Encoder(digestBase64);
	base64Encoder.write(reinterpret_cast<char*>(&digest[0]), digest.size());
	base64Encoder.close();
	return digestBase64.str();
}


std::string TwitterRequestFilter::percentEncode(const std::string& str)
{
	std::string encoded;
	Poco::URI::encode(str, "!?#/'\",;:$&()[]*+=@", encoded);
	return encoded;
}


} // namespace ofx
