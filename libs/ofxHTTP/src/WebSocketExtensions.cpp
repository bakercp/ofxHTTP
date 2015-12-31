// =============================================================================
//
// Copyright (c) 2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/WebSocketExtensions.h"
#include "ofx/HTTP/HTTPUtils.h"
#include "ofx/HTTP/ServerEvents.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofx/IO/ByteBufferStream.h"
#include "ofx/IO/Compression.h"
#include "Poco/DeflatingStream.h"
#include "Poco/InflatingStream.h"


namespace ofx {
namespace HTTP {


WebSocketPerMessageCompressionFactory::WebSocketPerMessageCompressionFactory()
{
}


WebSocketPerMessageCompressionFactory::~WebSocketPerMessageCompressionFactory()
{
}



std::unique_ptr<AbstractWebSocketFilter> WebSocketPerMessageCompressionFactory::makeFilterForRequest(ServerEventArgs& evt) const
{
    try
    {
        const std::string& extensionValues = evt.getRequest().get("Sec-WebSocket-Extensions");

        std::vector<std::string> elements;

        Poco::Net::MessageHeader::splitElements(extensionValues, elements, true);

        for (auto& element : elements)
        {
            std::string value;
            Poco::Net::NameValueCollection parameters;
            Poco::Net::MessageHeader::splitParameters(element, value, parameters);

            // Assume permessage-deflate and permessage-deflate are equivalent.

            if (0 == Poco::icompare(value, "permessage-deflate") ||
                0 == Poco::icompare(value, "x-webkit-deflate-frame"))
            {
                std::stringstream ss;

                // Set the value.
                ss << value;

                for (const auto& parameter : parameters)
                {
                    const std::string& key = parameter.first;
                    const std::string& value = parameter.second;

                    if (0 == Poco::icompare(key, "server_no_context_takeover"))
                    {
                        // https://tools.ietf.org/html/rfc7692#section-7.1.1.1
                        // - has no value
                        // If the peer server doesn't use context takeover, the
                        // client doesn't need to reserve memory to retain the
                        // LZ77 sliding window between messages.

                        // A server MAY include the "server_no_context_takeover"
                        // extension parameter in an extension negotiation
                        // response even if the extension negotiation offer
                        // being accepted by the extension negotiation response
                        // didn't include the "server_no_context_takeover"
                        // extension parameter.
                        //
                        // Currently unsupported.
                    }
                    else if (0 == Poco::icompare(key, "client_no_context_takeover"))
                    {
                        // https://tools.ietf.org/html/rfc7692#section-7.1.1.2
                        //
                        // Currently unsupported.
                    }
                    else if (0 == Poco::icompare(key, "server_max_window_bits"))
                    {
                        // https://tools.ietf.org/html/rfc7692#section-7.1.2.1
                        // This parameter has a decimal integer value without
                        // leading zeroes between 8 to 15, inclusive, indicating
                        // the base-2 logarithm of the LZ77 sliding window size,
                        // and MUST conform to the ABNF below.
                        //
                        // server-max-window-bits = 1*DIGIT
                        //
                        // If the peer server uses a small LZ77 sliding window
                        // to compress messages, the client can reduce the
                        // memory needed for the LZ77 sliding window.

                        // A server MAY include the "server_max_window_bits"
                        // extension parameter in an extension negotiation
                        // response even if the extension negotiation offer
                        // being accepted by the response didn't include the
                        // "server_max_window_bits" extension parameter.
                        //
                        // Currently unsupported.
                    }
                    else if (0 == Poco::icompare(key, "client_max_window_bits"))
                    {
                        // https://tools.ietf.org/html/rfc7692#section-7.1.2.2
                        // Currently unsupported.
                    }
                    else
                    {
                        ofLogWarning("WebSocketPerMessageCompressionFactory::makeFilterForRequest") << "Unknown permessage-deflate element: " << element;
                    }
                }

                evt.getResponse().set("Sec-WebSocket-Extensions", ss.str());

                return std::make_unique<WebSocketPerMessageCompressionFilter>();
            }
        }

        return nullptr;

    }
    catch (const Poco::NotFoundException& exc)
    {
        return nullptr;
    }
}


uint8_t DEFLATE_BYTE_BLOCK[] = { 0x00, 0x00, 0xff, 0xff };
std::size_t DEFLATE_BYTE_BLOCK_SIZE = sizeof(DEFLATE_BYTE_BLOCK);


WebSocketPerMessageCompressionFilter::WebSocketPerMessageCompressionFilter():
    _deflateWindowBits(15),
    _deflateMemLevel(8),
    _inflateWindowBits(15),
    _noContextTakeover(false)
{
    _deflateState.zalloc    = Z_NULL;
    _deflateState.zfree     = Z_NULL;
    _deflateState.opaque    = Z_NULL;
    _deflateState.next_in   = 0;
    _deflateState.avail_in  = 0;
    _deflateState.next_out  = 0;
    _deflateState.avail_out = 0;


    _inflateState.zalloc    = Z_NULL;
    _inflateState.zfree     = Z_NULL;
    _inflateState.opaque    = Z_NULL;
    _inflateState.next_in   = 0;
    _inflateState.avail_in  = 0;
    _inflateState.next_out  = 0;
    _inflateState.avail_out = 0;

    int rc = Z_OK;

    rc = deflateInit2(&_deflateState,
                      _deflateMemLevel, 
                      Z_DEFLATED,
                      -1 * _deflateWindowBits,
                      8,
                      Z_DEFAULT_STRATEGY);

    if (rc != Z_OK)
    {
        throw Poco::IOException(zError(rc));
    }

    rc = inflateInit2(&_inflateState,
                      -1 * _inflateWindowBits);

    if (rc != Z_OK)
    {
        throw Poco::IOException(zError(rc));
    }

    _buffer = new uint8_t[BUFFER_SIZE];
}


WebSocketPerMessageCompressionFilter::~WebSocketPerMessageCompressionFilter()
{
    delete [] _buffer;
    deflateEnd(&_deflateState);
    inflateEnd(&_inflateState);
}


void WebSocketPerMessageCompressionFilter::receiveFilter(WebSocketFrame& frame)
{
    // https://tools.ietf.org/html/rfc7692#section-7.2.2

    frame.writeBytes(DEFLATE_BYTE_BLOCK, DEFLATE_BYTE_BLOCK_SIZE);

    IO::ByteBuffer uncompressed;

    int rc = Z_OK;

    _inflateState.avail_in = frame.size();
    _inflateState.next_in = frame.getPtr();

    do
    {
        _inflateState.avail_out = BUFFER_SIZE;
        _inflateState.next_out = _buffer;

        rc = inflate(&_inflateState, Z_SYNC_FLUSH);

        if (rc != Z_OK)
        {
            throw Poco::IOException("Inflate: " + std::string(zError(rc)));
        }

        uncompressed.writeBytes(_buffer, BUFFER_SIZE - _inflateState.avail_out);

    }
    while (_inflateState.avail_out == 0);

    frame.clear();
    frame.writeBytes(uncompressed);
    frame.setRSV1(false);
}


void WebSocketPerMessageCompressionFilter::sendFilter(WebSocketFrame& frame)
{
    // https://tools.ietf.org/html/rfc7692#section-7.2.1

    IO::ByteBuffer compressed;

    _deflateState.avail_in = frame.size();
    _deflateState.next_in = frame.getPtr();

    do
    {
        _deflateState.avail_out = BUFFER_SIZE;
        _deflateState.next_out = _buffer;

        int rc = deflate(&_deflateState, Z_SYNC_FLUSH);

        if (rc != Z_OK)
        {
            throw Poco::IOException("Deflate: " + std::string(zError(rc)));
        }

        compressed.writeBytes(_buffer, BUFFER_SIZE - _deflateState.avail_out);

    }
    while (_deflateState.avail_out == 0);

    frame.clear();
    frame.writeBytes(compressed);
    frame.setRSV1(true);

    // Remove trailing 0x00 0x00 0xff 0xff for final frames.
    if (frame.isFinal())
    {
        // Chop off the 0x00, 0x00, 0xff, 0xff sequence.
        frame.resize(frame.size() - 4);
    }
}


} } // namespace ofx::HTTP
