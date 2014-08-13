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


#include "ofx/HTTP/WebSocketConnection.h"
#include "ofx/HTTP/WebSocketRoute.h"
#include "Poco/ByteOrder.h"


namespace ofx {
namespace HTTP {


WebSocketConnection::WebSocketConnection(WebSocketRoute& parent):
    _parent(parent),
    _isConnected(false),
    _totalBytesSent(0),
    _totalBytesReceived(0)
{
    _parent.registerWebSocketConnection(this);
}


WebSocketConnection::~WebSocketConnection()
{
    _parent.unregisterWebSocketConnection(this);
}


void WebSocketConnection::handleRequest(Poco::Net::HTTPServerRequest& request,
                                        Poco::Net::HTTPServerResponse& response)
{
    Poco::UUID sessionId = _parent.getSessionId(request, response);

    // get a copy of the settings
    WebSocketRouteSettings settings = _parent.getSettings();
    WebSocketEvents& events = _parent.events;

    try
    {
        _requestHeaders = request;
        _clientAddress = request.clientAddress();

        /// \todo fix when poco is upgraded
        applyFirefoxHack(request); //

        // check origin headers
        handleOrigin(request, response);

        // validate subprotocols
        handleSubprotocols(request, response);

        // respond to extensions
        handleExtensions(request, response);

        Poco::Net::WebSocket ws(request, response);

        ws.setReceiveTimeout(settings.getReceiveTimeout());
        ws.setSendTimeout(settings.getSendTimeout());
        ws.setKeepAlive(settings.getKeepAlive());

        _mutex.lock();
        _isConnected = true;
        _mutex.unlock();

        Poco::Buffer<char> buffer(settings.getBufferSize());

        int flags = 0;

        WebSocketOpenEventArgs eventArgs(sessionId, *this, request);
        ofNotifyEvent(events.onOpenEvent, eventArgs, this);

        do
        {
            flags = 0; // clear

            if (ws.poll(settings.getPollTimeout(), Poco::Net::Socket::SELECT_READ))
            {
                int numBytesReceived = ws.receiveFrame(buffer.begin(),
                                                       buffer.size(),
                                                       flags);

                _totalBytesReceived += numBytesReceived;

                if (numBytesReceived > 0)
                {
                    WebSocketFrame frame(buffer.begin(), numBytesReceived, flags);

                    if (frame.isPing() || frame.isPong())
                    {
                        if (settings.getAutoPingPongResponse())
                        {
                            int frameFlag = Poco::Net::WebSocket::FRAME_FLAG_FIN;

                            if (frame.isPing())
                            {
                                frameFlag |= Poco::Net::WebSocket::FRAME_OP_PONG;
                            }
                            else
                            {
                                frameFlag |= Poco::Net::WebSocket::FRAME_OP_PING;
                            }

                            WebSocketFrame pingPongFrame(buffer.begin(), numBytesReceived, frameFlag);

                            _mutex.lock();
                            _frameQueue.push(pingPongFrame);
                            _mutex.unlock();
                        }
                    }
                    else if (frame.isClose())
                    {
                        int code = -1;
                        std::string reason = "";

                        // TODO: it is possible, per the spec send
                        std::size_t n = frame.size();

                        const char* p = frame.getBinaryBuffer();

                        if (n >= 2)
                        {
                            // Get thec close code.
                            code = Poco::ByteOrder::fromNetwork((p[0] << 8) | p[1]);
                        }
                        else
                        {
                            ofLogWarning("WebSocketConnection::handleRequest") << "Non-conforming client, no close code sent.";
                        }

                        if (n > 2)
                        {
                            // Skip the first two bytes of the code.
                            reason = std::string(frame.getText().begin() + 2, frame.getText().end());
                        }
                        else
                        {
                            switch (code)
                            {
                                case 1000:
                                    reason = "Normal closure.";
                                    break;
                                case 1001:
                                    reason = "Going away.";
                                    break;
                                case 1002:
                                    reason = "Protocol error.";
                                    break;
                                case 1003:
                                    reason = "Received incompatible frame.";
                                    break;
                            }
                        }

                        WebSocketCloseEventArgs closeEventArgs(sessionId, *this, code, reason);
                        ofNotifyEvent(events.onCloseEvent, closeEventArgs, this);
                    }
                    else
                    {
                        WebSocketFrameEventArgs frameArgs(frame, sessionId, *this);
                        ofNotifyEvent(events.onFrameReceivedEvent, frameArgs, this);
                    }
                }
                else
                {
                    // clean shutdown if we read and no bytes were available.
                    close();
                }
            }

            // Send frames from _frameQueue.
            while (getSendQueueSize() > 0) // lock
            {
                _mutex.lock();
                WebSocketFrame frame = _frameQueue.front();
                _frameQueue.pop();
                _mutex.unlock();

                if(frame.size() > 0)
                {
                    if(ws.poll(settings.getPollTimeout(), Poco::Net::Socket::SELECT_WRITE))
                    {
                        int numBytesSent = 0;

                        numBytesSent = ws.sendFrame(frame.getBinaryBuffer(),
                                                    frame.size(),
                                                    frame.getFlags());

                        _totalBytesSent += numBytesSent;

                        // WebSocketError error = WS_ERR_NONE;

                        if(0 == numBytesSent)
                        {
                            ofLogWarning("ServerWebSocketRouteHandler::handleRequest") << "WebSocket numBytesSent == 0";
                            // error = WS_ERROR_ZERO_BYTE_FRAME_SENT;
                        }
                        // TODO ofBuffer::size() returns long ... sendFrame returns int ... :/
                        else if(numBytesSent < frame.size())
                        {
                            ofLogWarning("ServerWebSocketRouteHandler::handleRequest") << "WebSocket numBytesSent < frame.size()";
                            // error = WS_ERROR_INCOMPLETE_FRAME_SENT;
                        }
                        
                        WebSocketFrameEventArgs eventArgs(frame, sessionId, *this);
                        ofNotifyEvent(events.onFrameSentEvent, eventArgs, this);
                    }
                }
            }

            // Check for read error
            if (ws.poll(settings.getPollTimeout(), Poco::Net::Socket::SELECT_ERROR))
            {
                ofScopedLock lock(_mutex);
                _isConnected = false;
            }

        }
        while (_isConnected && (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);

        ofLogNotice("ServerWebSocketRouteHandler::handleRequest") << "WebSocket connection closed.";

    }
    catch (const Poco::Net::WebSocketException& exc)
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "WebSocketException: " << exc.code() << " Desc: " << exc.what();

        switch (exc.code())
        {
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.setReason("WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION");
                break;
            case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.setReason("WS_ERR_NO_HANDSHAKE");
                break;
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.setReason("WS_ERR_HANDSHAKE_NO_VERSION");
                break;
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.setReason("WS_ERR_HANDSHAKE_NO_KEY");
                break;
        }

        _parent.handleRequest(request,response);
        WebSocketErrorEventArgs eventArgs(sessionId, *this, (WebSocketError)exc.code());
        ofNotifyEvent(_parent.events.onErrorEvent, eventArgs, this);
    }
    catch (const Poco::TimeoutException& exc)
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "TimeoutException: " << exc.code() << " Desc: " << exc.what();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request, response);
        WebSocketErrorEventArgs eventArgs(sessionId, *this, WS_ERR_TIMEOUT);
        ofNotifyEvent(events.onErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    }
    catch (const Poco::Net::NetException& exc)
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "NetException: " << exc.code() << " Desc: " << exc.what();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request, response);
        WebSocketErrorEventArgs eventArgs(sessionId, *this, WS_ERR_NET_EXCEPTION);
        ofNotifyEvent(events.onErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    }
    catch (const std::exception& exc)
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "exception: " << exc.what();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request, response);
        WebSocketErrorEventArgs eventArgs(sessionId, *this, WS_ERR_OTHER);
        ofNotifyEvent(_parent.events.onErrorEvent, eventArgs, this);
    }
    catch ( ... )
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "... Unknown exception.";
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request,response);
        WebSocketErrorEventArgs eventArgs(sessionId, *this, WS_ERR_OTHER);
        ofNotifyEvent(_parent.events.onErrorEvent, eventArgs, this);
    }
}


bool WebSocketConnection::sendFrame(const WebSocketFrame& frame) const
{
    ofScopedLock lock(_mutex);

    if(_isConnected)
    {
        _frameQueue.push(frame);
        return true;
    }
    else
    {
        ofLogError("WebSocketConnection::sendFrame") << "Not connected, frame not sent.";
        return false;
    }
}


std::size_t WebSocketConnection::getSendQueueSize() const
{
    ofScopedLock lock(_mutex);
    return _frameQueue.size();
}


void WebSocketConnection::clearSendQueue()
{
    ofScopedLock lock(_mutex);
    std::queue<WebSocketFrame> empty; // a way to clear queues.
    std::swap(_frameQueue, empty);
}


void WebSocketConnection::close()
{
    ofScopedLock lock(_mutex);
    _isConnected = false;
}


Poco::Net::NameValueCollection WebSocketConnection::getRequestHeaders() const
{
    ofScopedLock lock(_mutex);
    return _requestHeaders;
}


Poco::Net::SocketAddress WebSocketConnection::getClientAddress() const
{
    ofScopedLock lock(_mutex);
    return _clientAddress;
}


bool WebSocketConnection::isConnected() const
{
    ofScopedLock lock(_mutex);
    return _isConnected;
}

std::size_t WebSocketConnection::getTotalBytesSent() const
{
    ofScopedLock lock(_mutex);
    return _totalBytesSent;
}


std::size_t WebSocketConnection::getTotalBytesReceived() const
{
    ofScopedLock lock(_mutex);
    return _totalBytesReceived;
}


//void WebSocketConnection::handleErrorResponse(Poco::Net::HTTPServerResponse& response)
//{
//    // TODO: come up with a better solution for shared / default error handling
//    // Respond immediately?
//
//    // We gave the handlers every opportunity to send a response.
//    // Now we must conclude that there is a server error.
//    try
//    {
//        response.setChunkedTransferEncoding(true);
//        response.setContentType("text/html");
//
//        std::ostream& ostr = response.send(); // get output stream
//        ostr << "<html>";
//        ostr << "<head><title>" << response.getStatus() << " - " << response.getReason() << "</title></head>";
//        ostr << "<body>";
//        ostr << "<h1>" << response.getStatus() << " - " << response.getReason() << "</h1>";
//        ostr << "</body>";
//        ostr << "<html>";
//    }
//    catch (const Poco::Exception& exc)
//    {
//        ofLogError("WebSocketConnection::handleErrorResponse") << "Exception: " << exc.code() << " " << exc.displayText();
//    }
//    catch (const std::exception& exc)
//    {
//        ofLogError("WebSocketConnection::handleErrorResponse") << "exception: " << exc.what();
//    }
//    catch ( ... )
//    {
//        ofLogError("WebSocketConnection::handleErrorResponse") << "... Unknown exception.";
//    }
//
//}


void WebSocketConnection::handleOrigin(Poco::Net::HTTPServerRequest& request,
                                       Poco::Net::HTTPServerResponse& response)
{
    // http://en.wikipedia.org/wiki/Same_origin_policy
    //    _settings.getAllowCrossOriginConnections();
    //    Utils::dumpHeaders(request,OF_LOG_NOTICE);
    // Access-Control-Allow-Origin
    //    ofLogError("ServerRouteHandler::handleOrigin") << "TODO: handle/check origin";
}


void WebSocketConnection::handleSubprotocols(Poco::Net::HTTPServerRequest& request,
                                             Poco::Net::HTTPServerResponse& response)
{
//    SubprotocolSet& getValidSubprotocols()
//    std::vector<std::string> proposedProtocols = ofSplitString(request.get("Sec-WebSocket-Protocol",""),
//                                                               ",",
//                                                               true,
//                                                               true);
//
//    std::string selectedProtocol;
//
//    bool isValidProtocol = _manager.selectSubprotocol(proposedProtocols,selectedProtocol);
//
//    // if we don't support the protocol, we don't send a Sec-WebSocket-Protocol header with the response.
//    // in doing so, we leave the decision up to the client.  most clients should terminate the connection.
//    // http://stackoverflow.com/questions/13545453/http-response-code-when-requested-websocket-subprotocol-isnt-supported-recogniz
//    if(isValidProtocol && ! selectedProtocol.empty())
//    {
//        response.set("Sec-WebSocket-Protocol",selectedProtocol);
//    }
//    else
//    {
//        // we don't send anything, which is equiv to null according to the spec
//        // exchange.response.set("Sec-WebSocket-Protocol", "null");
//    }
}


void WebSocketConnection::handleExtensions(Poco::Net::HTTPServerRequest& request,
                                           Poco::Net::HTTPServerResponse& response)
{
//    if(request.has("Sec-WebSocket-Extensions"))
//    {
//        // TODO: support these
//        // http://tools.ietf.org/html/draft-tyoshino-hybi-websocket-perframe-deflate-05
//        // http://tools.ietf.org/html/draft-ietf-hybi-websocket-multiplexing-09
//    }
}

void WebSocketConnection::applyFirefoxHack(Poco::Net::HTTPServerRequest& request)
{
    // HACK FOR FIREFOX
    // require websocket upgrade headers
    std::string connectionHeader = Poco::toLower(request.get("Connection", ""));
    
    if(Poco::icompare(connectionHeader, "Upgrade") != 0)
    {
        std::string userAgent = Poco::toLower(request.get("User-Agent",""));
        if(!userAgent.empty() &&
           !connectionHeader.empty() &&
           ofIsStringInString(userAgent,"firefox") &&
           ofIsStringInString(connectionHeader,"upgrade"))
        {
            // this request is coming from firefox, which is known to send things that look like:
            // Connection:keep-alive, Upgrade
            // thus failing the standard Poco upgrade test.
            // we can't do this here, but will do a similar hack in the handler
            request.set("Connection","Upgrade");
        }
    }
}


} } // namespace ofx::HTTP
