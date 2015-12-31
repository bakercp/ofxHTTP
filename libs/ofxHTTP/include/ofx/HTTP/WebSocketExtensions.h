// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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

    int _deflateWindowBits;
    int _deflateMemLevel;

    int _inflateWindowBits;

    bool _noContextTakeover;

    z_stream _deflateState;
    z_stream _inflateState;

    uint8_t* _buffer;

};


} } // namespace ofx::HTTP
