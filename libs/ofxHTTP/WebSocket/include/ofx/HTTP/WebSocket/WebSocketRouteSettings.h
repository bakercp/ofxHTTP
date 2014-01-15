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


#include <string>
#include "Poco/Timespan.h"
#include "ofx/HTTP/Server/BaseRouteSettings.h"


namespace ofx {
namespace HTTP {


class WebSocketRouteSettings: public BaseRouteSettings
    /// \brief Settings for a WebSocketRoute.
{
public:
    WebSocketRouteSettings(const std::string& routePathPattern = DEFAULT_WEBSOCKET_ROUTE_PATH_PATTERN);
        ///< \brief Create the WebSocketRouteSettings with the given route path.
        ///< \param routePathPattern The regex pattern that this route
        ///<        will handle.

    virtual ~WebSocketRouteSettings();
        ///< \brief Destroy the WebSocketRouteSettings.

    void setSubprotocol(const std::string& subprotocol);
        ///< \brief Set the WebSocketRoute's subprotocol.
        ///< \param subprotocol The WebSocket subprotocol to support.

    std::string getSubprotocol() const;
        ///< \returns The subprotocol supported by the WebSocketRoute.

    void setAllowEmptySubprotocol(bool allowEmptySubprotocol);
        ///< \brief Allow WebSocket connections that do not define a protocol.
        ///< \param allowEmptySubprotocol true if this WebSocketRouteSettings
        ///<        supports WebSocket connections with empty protocols.

    bool getAllowEmptySubprotocol() const;
        ///< \returns true if these settings allow connections with empty
        ///<        subprotocols.

    void setAllowCrossOriginConnections(bool allowCrossOriginConnections);
        ///< \brief Allow WebSocket connections to occur across origins.
        ///< \details Cross origin connections are those in which the
        ///<        the calling page is in a different location than the targer.
        ///< \param allowCrossOriginConnections true if this
        ///<        WebSocketRouteSettingssupports supports
        ///<        with cross origin connections.

    bool getAllowCrossOriginConnections() const;
        ///< \returns true if these settings allow cross origin connections.

//    void setIsBinary(bool isBinary);
//        ///< \brief Allow WebSocket connections that do not define a protocol.
//        ///< \param allowEmptySubprotocol true if this WebSocketRouteSettings
//        ///<        supports WebSocket connections with empty protocols.
//
//    bool getIsBinary() const;
//        ///< \returns truee

    void setAutoPingPongResponse(bool autoPingPongResponse);

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

private:
    std::string _subprotocol;

    bool _allowEmptySubprotocol;
    bool _allowCrossOriginConnections;

//    bool _isBinary;

    bool _autoPingPongResponse; // automatically return pong frames if true
    bool _keepAlive;

    Poco::Timespan _receiveTimeout;
    Poco::Timespan _sendTimeout;
    Poco::Timespan _pollTimeout;

    std::size_t _bufferSize;

};


} } // namespace ofx::HTTP
