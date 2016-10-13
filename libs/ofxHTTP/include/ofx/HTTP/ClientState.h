// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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
