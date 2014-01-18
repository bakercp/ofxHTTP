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


#pragma once


#include <set>
#include <string>
#include "Poco/Timespan.h"
#include "ofx/HTTP/Server/BaseRouteSettings.h"


namespace ofx {
namespace HTTP {


class WebSocketRouteSettings: public BaseRouteSettings
    /// \brief Settings for a WebSocketRoute.
{
public:
    typedef std::set<std::string> SubprotocolSet;
        ///< \breif A typedef for subprotocols.

    typedef std::set<std::string> OriginSet;
        ///< \brief A typedef for origins.

    WebSocketRouteSettings(const std::string& routePathPattern = DEFAULT_WEBSOCKET_ROUTE_PATH_PATTERN,
                           bool requireSecurePort = false);
        ///< \param routePathPattern The regex pattern that this route
        ///<        will handle.
        ///< \param requireSecurePorttrue True if this route requires
        ///<        communication on an SSL encrypted port.

    virtual ~WebSocketRouteSettings();
        ///< \brief Destroy the WebSocketRouteSettings.

    void setValidSubprotcols(const SubprotocolSet& subprotocols);
        ///< \brief Set the list of valid subprotocols.
        ///< \param subprotocols A set of valid subprotocols.
        ///< \note  An empty set means that any requested subprotocol will be
        ///<        accepted.  A non-empty set means that the requested
        ///<        subprotocol MUST be in the set.

    const SubprotocolSet& getValidSubprotocols() const;
        ///< \returns The set of valid subprotocols.
        ///< \note  If empty, all requested subprotocols will be accepted.

    void setValidOrigins(const OriginSet& origins);
        ///< \brief Set the list of valid origins.
        ///< \param origins A set of valid origins.
        ///< \note  An empty set means that any Origin header in the request
        ///<        will be accepted.  A non-empty set means that the request
        ///<        origin MUST be in the set.

    const OriginSet& getValidOrigins() const;
        ///< \returns The set of valid origins.
        ///< \note  If empty, all Origin headers will be accepted.

    void setAutoPingPongResponse(bool autoPingPongResponse);
        ///< \param autoPingPongResponse If set to true, the WebSocket
        ///<        connection will return all PINGs with a PONG

    bool getAutoPingPongResponse() const;

    void setKeepAlive(bool keepAlive);

    bool getKeepAlive() const;

    void setReceiveTimeout(const Poco::Timespan& receiveTimeout);

    Poco::Timespan getReceiveTimeout() const;

    void setSendTimeout(const Poco::Timespan& sendTimeout);

    Poco::Timespan getSendTimeout() const;

    void setPollTimeout(const Poco::Timespan& pollTimeout);

    Poco::Timespan getPollTimeout() const;

    void setBufferSize(std::size_t bufferSize);

    std::size_t getBufferSize() const;

    static const std::string DEFAULT_WEBSOCKET_ROUTE_PATH_PATTERN;
    static const Poco::Timespan DEFAULT_RECEIVE_TIMEOUT;
    static const Poco::Timespan DEFAULT_SEND_TIMEOUT;
    static const Poco::Timespan DEFAULT_POLL_TIMEOUT;

    enum
    {
        DEFAULT_BUFFER_SIZE = 1024
    };


private:
    SubprotocolSet _validSubprotocols;
    OriginSet _validOrigins;

    bool _autoPingPongResponse; // automatically return pong frames if true
    bool _keepAlive;

    Poco::Timespan _receiveTimeout;
    Poco::Timespan _sendTimeout;
    Poco::Timespan _pollTimeout;

    std::size_t _bufferSize;

};


} } // namespace ofx::HTTP
