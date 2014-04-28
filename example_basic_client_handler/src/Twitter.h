#pragma once


#include "Poco/Net/HTMLForm.h"
#include "ofx/HTTP/Client/BaseRequestResponseProcessor.h"


class Twitter: public ofx::HTTP::BaseRequestResponseProcessor
{
public:
	Twitter();
	~Twitter();

	void login(const std::string& consumerKey,
               const std::string& consumerSecret,
               const std::string& token,
               const std::string& tokenSecret);

	static const std::string TWITTER_URI;

    void processRequest(ofx::HTTP::Client::BaseRequest& request,
                        ofx::HTTP::Context& context);

    bool handleResponse(ofx::HTTP::Client::BaseRequest& request,
                        ofx::HTTP::Client::BaseResponse& response,
                        ofx::HTTP::Context& context);

    /// Signs the given HTTP request according to OAuth 1.0a as used by the Twitter API.
    ///
    /// See <https://dev.twitter.com/docs/auth/authorizing-request> and
    /// <https://dev.twitter.com/docs/auth/creating-signature> for more information.
	void sign(Poco::Net::HTTPRequest& request,
              const Poco::Net::HTMLForm& params,
              const std::string& uri) const;

    /// Creates a nonce, which is basically a Base64-encoded 32 character random
    /// string, with non-alphanumeric characters removed.
	std::string createNonce() const;

    /// Creates a OAuth signature for the given request and its parameters, according
    /// to <https://dev.twitter.com/docs/auth/creating-signature>.
	std::string createSignature(Poco::Net::HTTPRequest& request,
                                const Poco::Net::HTMLForm& params,
                                const std::string& uri,
                                const std::string& nonce,
                                const std::string& timestamp) const;

    /// \brief Percent-encodes the given string according to Twitter API's rules,
    /// given in <https://dev.twitter.com/docs/auth/percent-encoding-parameters>.
	static std::string percentEncode(const std::string& str);

private:
	std::string _consumerKey;
	std::string _consumerSecret;
	std::string _token;
	std::string _tokenSecret;
};
