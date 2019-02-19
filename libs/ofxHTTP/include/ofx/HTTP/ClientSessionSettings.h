//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include "Poco/Timespan.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofTypes.h"
#include "ofx/HTTP/ProxySettings.h"
#include "ofx/HTTP/HTTPUtils.h"


namespace ofx {
namespace HTTP {

    
/// \brief Settings for an Poco::Net::HTTPClientSession.
///
/// This is used by a Context to configure client / server interaction.
class ClientSessionSettings
{
public:
    /// \brief Create a default ClientSessionSettings.
    ClientSessionSettings();

    /// \brief Destroy the ClientSessionSettings.
    virtual ~ClientSessionSettings();

    /// \brief Set the user-agent for this client session.
    /// \param userAgent The user agent string.
    void setUserAgent(const std::string& userAgent);

    /// \returns the current user-agent.
    std::string getUserAgent() const;

    /// \brief Set the max number of redirects to automatically follow.
    /// \param maxRedirects The max number of redirects to automatically follow.
    void setMaxRedirects(std::size_t maxRedirects);

    /// \returns The max number of redirects to automatically follow.
    std::size_t getMaxRedirects() const;

    /// \brief Get the default headers to included with each request.
    /// \returns the default headers to included with each request.
    const Poco::Net::NameValueCollection& getDefaultHeaders() const;

    /// \brief Set the default headers to included with each request.
    /// \params headers The default headers to included with each request.
    void setDefaultHeaders(const Poco::Net::NameValueCollection& headers);

    /// \brief Add a default headers to included with each request.
    /// \params name The header name.
    /// \params value The header value.
    void addDefaultHeader(const std::string& name, const std::string& value);

    /// \brief Enable or disable the session keep-alive.
    /// \param keepAlive True if keep-alive should be enabled.
    void setkeepAlive(bool keepAlive);

    /// \returns True if keep-alive is enabled.
    bool getKeepAlive() const;

    /// \brief Set the keep-alive timeout.
    /// \param keepAliveTimeout The keep-alive timeout.
    void setKeepAliveTimeout(Poco::Timespan keepAliveTimeout);

    /// \returns The keep-alive timeout.
    Poco::Timespan getKeepAliveTimeout() const;

    /// \brief Set the client timeout.
    /// \param clientTimeout The client timeout to set.
    void setTimeout(Poco::Timespan timeout);

    /// \returns The client timeout.
    Poco::Timespan getTimeout() const;

    /// \brief Set the proxy settings for this client session.
    /// \param proxy The proxy settings to use.
    void setProxy(const ProxySettings& proxy);

    /// \returns the current proxy settings.
    const ProxySettings& getProxySettings() const;

    /// \brief Set the minimum number of bytes per progress update.
    ///
    /// This is the default number of bytes to wait between progress update
    /// events events. If this number is very low, then too many events may be
    /// sent. If this number is too high, the progress updates may not come
    /// frequently enough to be useful.
    ///
    /// A value of 0 will disable the progress updates.
    ///
    /// \param bytesPerProgressUpdate The number of bytes to set.
    void setMinimumBytesPerProgressUpdate(int64_t minimumBytesPerProgressUpdate);

    /// \returns The minimum number of bytes per progress update.
    int64_t getMinimumBytesPerProgressUpdate() const;

    /// \brief Set the maximum interval between progress updates.
    ///
    /// This is useful for slow connections to keep the update data rate
    /// up-to-date.
    ///
    /// A value of 0 will effectively disable the maximum interval.
    ///
    /// \param interval The interval to set.
    void setMaximumProgressUpdateInterval(Poco::Timespan interval);

    /// \returns the maximum interval between progress updates.
    Poco::Timespan getMaximumProgressUpdateInterval() const;

    /// \brief The default user agent sent with the client.
    ///
    /// Default: "Mozilla/5.0 (compatible; Client/1.0 +https://github.com/bakercp/ofxHTTP)".
    static const std::string DEFAULT_USER_AGENT;

    /// \brief The default max number of redirects to automatically follow.
    ///
    /// Default: 20.
    static const std::size_t DEFAULT_MAX_REDIRECTS;

    /// \brief The default keep alive timeout.
    ///
    /// Default: 8 Seconds.
    static const Poco::Timespan DEFAULT_KEEPALIVE_TIMEOUT;

    /// \brief The default client timeout
    ///
    /// Default: 60 Seconds.
    static const Poco::Timespan DEFAULT_TIMEOUT;

    /// \brief The default number of bytes to wait between progress update events.
    ///
    /// Default: 1024 Bytes.
    static const int64_t DEFAULT_MINIMUM_BYTES_PER_PROGRESS_UPDATE;

    /// \brief The default maximum interval between progress updates.
    ///
    /// Default: 1 Second.
    static const Poco::Timespan DEFAULT_MAXIMUM_PROGRESS_UPDATE_INTERVAL;

private:
    /// \brief The user-agent string.
    std::string _userAgent;

    /// \brief The maximum number of redirects to automatically follow.
    std::size_t _maxRedirects = DEFAULT_MAX_REDIRECTS;

    /// \brief True if the session keep-alive should be enabled.
    bool _keepAlive;

    /// \brief The keep-alive timeout.
    Poco::Timespan _keepAliveTimeout;

    /// \brief The client timeout.
    Poco::Timespan _timeout;

    /// \brief The proxy settings for the client session.
    ProxySettings _proxy;

    /// \brief The number of bytes to wait between progress update events.
    ///
    /// A value of 0 will disable the progress updates.
    int64_t _minimumBytesPerProgressUpdate;

    /// \brief The maximum interval between progress updates.
    Poco::Timespan _maximumProgressUpdateInterval;

    /// \brief The a collection of default headers to include with each request.
    Poco::Net::NameValueCollection _defaultHeaders;

};

    
} } // namespace ofx::HTTP
