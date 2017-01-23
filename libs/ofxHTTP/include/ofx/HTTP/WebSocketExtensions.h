//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <queue>
#if defined(POCO_UNBUNDLED)
#include <zlib.h>
#else
#include "Poco/zlib.h"
#endif
#include "Poco/Buffer.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/WebSocketFrame.h"


namespace ofx {
namespace HTTP {


/// \brief A filter factory for per-message WebSocket compression.
/// \note This is experimental and may not be fully RFC7692-compliant.
/// \sa https://tools.ietf.org/html/rfc7692
class WebSocketPerMessageCompressionFactory: public AbstractWebSocketFilterFactory
{
public:
    /// \brief Create a WebSocketPerMessageCompressionFactory extension.
    WebSocketPerMessageCompressionFactory();

    /// \brief Destroy a WebSocketPerMessageCompressionFactory.
    virtual ~WebSocketPerMessageCompressionFactory();

    std::unique_ptr<AbstractWebSocketFilter> makeFilterForRequest(ServerEventArgs& evt) const override;

};


/// \brief A WebSocketFilter for per-message compression.
class WebSocketPerMessageCompressionFilter: public AbstractWebSocketFilter
{
public:
    WebSocketPerMessageCompressionFilter();
    virtual ~WebSocketPerMessageCompressionFilter();

    void receiveFilter(WebSocketFrame& frame) override;
    void sendFilter(WebSocketFrame& frame) override;

private:
    enum
    {
        BUFFER_SIZE = 32768,
        MIN_WINDOW_BITS = 8,
        MAX_WINDOW_BITS = 15
    };

    int _deflateWindowBits = 15;
    int _deflateMemLevel = 8;

    int _inflateWindowBits = 15;

    //bool _noContextTakeover = false;

    z_stream _deflateState;
    z_stream _inflateState;

    uint8_t* _buffer;

};


} } // namespace ofx::HTTP
