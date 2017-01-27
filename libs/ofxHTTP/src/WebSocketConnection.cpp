//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/WebSocketConnection.h"
#include "ofx/HTTP/WebSocketRoute.h"
#include "Poco/ByteOrder.h"


namespace ofx {
namespace HTTP {


WebSocketConnection::WebSocketConnection(WebSocketRoute& _route):
    BaseRouteHandler_<WebSocketRoute>(_route)
{
    route().registerConnection(this);
}


WebSocketConnection::~WebSocketConnection()
{
    route().unregisterConnection(this);
}


void WebSocketConnection::handleRequest(ServerEventArgs& evt)
{
    try
    {
        _requestHeaders = evt.request();
        _clientAddress = evt.request().clientAddress();

        /// \todo Fix when poco is upgraded.
        applyFirefoxHack(evt);

        // Check origin headers.
        handleOrigin(evt);

        // Validate subprotocols.
        handleSubprotocols(evt);

        // Respond to extensions.
        handleExtensions(evt);

        Poco::Net::WebSocket ws(evt.request(), evt.response());

        ws.setReceiveTimeout(route().settings().getReceiveTimeout());
        ws.setSendTimeout(route().settings().getSendTimeout());
        ws.setKeepAlive(route().settings().getKeepAlive());

        _mutex.lock();
        _isConnected = true;
        _mutex.unlock();

        Poco::Buffer<char> buffer(route().settings().getBufferSize());

        int flags = 0;

        WebSocketOpenEventArgs eventArgs(evt, *this);
        ofNotifyEvent(route().events.onWebSocketOpenEvent, eventArgs, this);

        do
        {
            flags = 0; // clear

            if (ws.poll(route().settings().getPollTimeout(), Poco::Net::Socket::SELECT_READ))
            {
                int numBytesReceived = ws.receiveFrame(buffer.begin(),
                                                       buffer.size(),
                                                       flags);

                _totalBytesReceived += numBytesReceived;

                if (numBytesReceived > 0)
                {
                    WebSocketFrame frame(buffer.begin(), numBytesReceived, flags);

                    // Send a return frame.
                    if (frame.isPing() || frame.isPong())
                    {
                        if (route().settings().getAutoPingPongResponse())
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
                        uint16_t code = 0;
                        
                        std::string reason = "";

                        // TODO: it is possible, per the spec send
                        std::size_t n = frame.size();

                        const char* pData = frame.getCharPtr();

                        if (n >= 2)
                        {
                            // Get the close code.
                            code = static_cast<uint16_t>(Poco::ByteOrder::fromNetwork((pData[0] << 8) | pData[1]));
                        }
                        else
                        {
                            ofLogWarning("WebSocketConnection::handleRequest") << "Non-conforming client, no close code sent.";
                        }

                        if (n > 2)
                        {
                            // Skip the first two bytes of the code.
                            reason = frame.toString().substr(2);
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

                        WebSocketCloseEventArgs closeEventArgs(evt,
                                                               *this,
                                                               code,
                                                               reason);

                        ofNotifyEvent(route().events.onWebSocketCloseEvent,
                                      closeEventArgs,
                                      this);

                        ofLogVerbose("WebSocketConnection::handleRequest") << "WebSocket connection closed: code=" << code << " reason: " << reason;

                    }
                    else
                    {
                        // Apply receive filters to received frame.
                        for (auto& filter: _filters)
                        {
                            filter->receiveFilter(frame);
                        }

                        WebSocketFrameEventArgs frameArgs(evt, *this, frame);

                        ofNotifyEvent(route().events.onWebSocketFrameReceivedEvent,
                                      frameArgs,
                                      this);
                    }
                }
                else
                {
                    // Clean shutdown if we read and no bytes were available.
                    stop();
                }
            }

            // Send frames from _frameQueue.
            while (sendQueueSize() > 0) // lock
            {
                _mutex.lock();
                WebSocketFrame frame = _frameQueue.front();
                _frameQueue.pop();
                _mutex.unlock();

                if (frame.size() > 0)
                {
                    if (ws.poll(route().settings().getPollTimeout(),
                                Poco::Net::Socket::SELECT_WRITE))
                    {
                        // Apply send filters to queued frame.
                        for (auto& filter: _filters)
                        {
                            filter->sendFilter(frame);
                        }

                        const char* pData = frame.getCharPtr();

                        int numBytesSent = ws.sendFrame(pData,
                                                        frame.size(),
                                                        frame.flags());

                        // WebSocketError error = WS_ERR_NONE;

                        if (numBytesSent < 0)
                        {
                            ofLogWarning("WebSocketConnection::handleRequest") << "WebSocket numBytesSent < 0";
                            // error = WS_ERROR_ZERO_BYTE_FRAME_SENT;
                        }
                        else
                        {
                            if (numBytesSent < static_cast<int>(frame.size()))
                            {
                                ofLogWarning("WebSocketConnection::handleRequest") << "WebSocket numBytesSent < frame.size()";
                                // error = WS_ERROR_INCOMPLETE_FRAME_SENT;
                            }

                            _totalBytesSent += numBytesSent;
                        }

                        WebSocketFrameEventArgs eventArgs(evt, *this, frame);
                        
                        ofNotifyEvent(route().events.onWebSocketFrameSentEvent,
                                      eventArgs,
                                      this);
                    }
                }
            }

            // Check for read error
            if (ws.poll(route().settings().getPollTimeout(),
                        Poco::Net::Socket::SELECT_ERROR))
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _isConnected = false;
            }

        }
        while (_isConnected && (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);

        ofLogNotice("WebSocketConnection::handleRequest") << "WebSocket connection closed.";

    }
    catch (const Poco::Net::WebSocketException& exc)
    {
        ofLogError("WebSocketConnection::handleRequest") << "WebSocketException: " << exc.code() << " : " << exc.displayText();

        switch (exc.code())
        {
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                evt.response().set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
                evt.response().setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                evt.response().setReason("WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION");
                break;
            case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
                evt.response().setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                evt.response().setReason("WS_ERR_NO_HANDSHAKE");
                break;
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
                evt.response().setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                evt.response().setReason("WS_ERR_HANDSHAKE_NO_VERSION");
                break;
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                evt.response().setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                evt.response().setReason("WS_ERR_HANDSHAKE_NO_KEY");
                break;
        }

        route().handleRequest(evt);
        WebSocketErrorEventArgs eventArgs(evt, *this, (WebSocketError)exc.code());
        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
    }
    catch (const Poco::TimeoutException& exc)
    {
        ofLogError("WebSocketConnection::handleRequest") << "TimeoutException: " << exc.code() << " Desc: " << exc.what();
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_TIMEOUT);
        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    }
    catch (const Poco::Net::NetException& exc)
    {
        ofLogError("WebSocketConnection::handleRequest") << "NetException: " << exc.code() << " Desc: " << exc.what();
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_NET_EXCEPTION);
        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("WebSocketConnection::handleRequest") << "Exception: " << exc.displayText();
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_OTHER);
        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
    }
    catch (const std::exception& exc)
    {
        ofLogError("WebSocketConnection::handleRequest") << "exception: " << exc.what();
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_OTHER);
        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
    }
    catch ( ... )
    {
        ofLogError("WebSocketConnection::handleRequest") << "... Unknown exception.";
        evt.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        route().handleRequest(evt);
        WebSocketErrorEventArgs eventArgs(evt, *this, WS_ERR_OTHER);
        ofNotifyEvent(route().events.onWebSocketErrorEvent, eventArgs, this);
    }
}


bool WebSocketConnection::sendFrame(const WebSocketFrame& frame) const
{
    std::unique_lock<std::mutex> lock(_mutex);

    if (_isConnected)
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


std::size_t WebSocketConnection::sendQueueSize() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _frameQueue.size();
}


void WebSocketConnection::clearSendQueue()
{
    std::unique_lock<std::mutex> lock(_mutex);
    std::queue<WebSocketFrame> empty; // a way to clear queues.
    std::swap(_frameQueue, empty);
}


void WebSocketConnection::stop()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _isConnected = false;
}


Poco::Net::NameValueCollection WebSocketConnection::requestHeaders() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _requestHeaders;
}


Poco::Net::SocketAddress WebSocketConnection::clientAddress() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _clientAddress;
}


bool WebSocketConnection::isConnected() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _isConnected;
}

std::size_t WebSocketConnection::totalBytesSent() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _totalBytesSent;
}


std::size_t WebSocketConnection::totalBytesReceived() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _totalBytesReceived;
}


void WebSocketConnection::handleOrigin(ServerEventArgs&)
{
    // http://en.wikipedia.org/wiki/Same_origin_policy
    //    _settings.getAllowCrossOriginConnections();
    //    Utils::dumpHeaders(request,OF_LOG_NOTICE);
    // Access-Control-Allow-Origin
    //    ofLogError("ServerRouteHandler::handleOrigin") << "TODO: handle/check origin";
}


void WebSocketConnection::handleSubprotocols(ServerEventArgs&)
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


void WebSocketConnection::handleExtensions(ServerEventArgs& evt)
{
    // Combine multiple headers, if needed.

    std::stringstream extensionValues;

    bool alreadyFoundOne = false;

    auto iter = evt.request().begin();

    while (iter != evt.request().end())
    {
        const auto& key = (*iter).first;
        const auto& value = (*iter).second;

        if (0 == Poco::icompare(key, "Sec-WebSocket-Extensions"))
        {
            extensionValues << value;

            if (alreadyFoundOne)
            {
                extensionValues << ", ";
            }
            else
            {
                alreadyFoundOne = true;
            }
        }
        
        ++iter;
    }

    // Remove all Sec-WebSocket-Extensions headers.
    evt.request().erase("Sec-WebSocket-Extensions");

    // Add the sanitized, combined header.
    evt.request().add("Sec-WebSocket-Extensions", extensionValues.str());

    const auto& filterFactories = route().filterFactories();

    for (const auto& factory : filterFactories)
    {
        auto filter = factory->makeFilterForRequest(evt);

        if (filter != nullptr)
        {
            _filters.push_back(std::move(filter));
        }
    }
}


void WebSocketConnection::applyFirefoxHack(ServerEventArgs& evt)
{
    // HACK FOR FIREFOX
    // require websocket upgrade headers
    std::string connectionHeader = Poco::toLower(evt.request().get("Connection", ""));
    
    if (0 != Poco::icompare(connectionHeader, "Upgrade"))
    {
        std::string userAgent = Poco::toLower(evt.request().get("User-Agent",""));

        if (!userAgent.empty() &&
            !connectionHeader.empty() &&
            ofIsStringInString(userAgent, "firefox") &&
            ofIsStringInString(connectionHeader, "upgrade"))
        {
            // this request is coming from firefox, which is known to send things that look like:
            // Connection:keep-alive, Upgrade
            // thus failing the standard Poco upgrade test.
            // we can't do this here, but will do a similar hack in the handler
            evt.request().set("Connection", "Upgrade");
        }
    }
}


} } // namespace ofx::HTTP
