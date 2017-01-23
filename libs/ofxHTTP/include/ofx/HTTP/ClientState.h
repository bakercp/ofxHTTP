//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>


namespace ofx {
namespace HTTP {


enum class ClientState
{
    /// \brief A default value that should not be encountered.
    NONE,

    /// \brief The system starts to detect a proxy.
    ///
    /// This step may not occur depending on the system configuration.
    DETECTING_PROXY,

    /// \brief The system is resolving the hostname for the HTTP connection.
    ///
    /// This step may not occur if the hostname doesn't need to be resolved.
    RESOLVING_NAME,

    /// \brief The HTTP connection to the server is negotiating SSL.
    ///
    /// If the SSL negotiation fails, then this will be the last step that
    /// occurs on the HTTP connection.
    CONNECTING_TO_SERVER,

    /// \brief HTTP headers are being sent to the server.
    NEGOTIATING_SSL,

    /// \brief HTTP headers are being sent to the server.
    SENDING_HEADERS,

    /// \brief HTTP content is being sent to the server.
    ///
    /// This step may not occur if there is no content to send.
    SENDING_CONTENTS,

    /// \brief The HTTP client is waiting for a response from the server.
    WAITING_FOR_RESPONSE,

    /// \brief The HTTP client is receiving headers from the server.
    RECEIVING_HEADERS,

    /// \brief The HTTP client is receiving content from the server.
    RECEIVING_CONTENTS,

    /// \brief The HTTP client is being redirected.
    REDIRECTING
};


std::string to_string(ClientState state);

bool from_string(const std::string text, ClientState& state);


} } // namespace ofx::HTTP
