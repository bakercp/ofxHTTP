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


#include "WebSocketRouteHandler.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
WebSocketRouteHandler::WebSocketRouteHandler(BaseWebSocketSessionManager& manager,
                                             const Settings& settings) :
    BaseWebSocketRouteHandler(settings),
    _manager(manager),
    _bIsConnected(false)
{
    manager.registerRouteHandler(this);
}

//------------------------------------------------------------------------------
WebSocketRouteHandler::~WebSocketRouteHandler()
{
    _manager.unregisterRouteHandler(this);
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::handleExchange(ServerExchange& exchange)
{

    try {
        applyFirefoxHack(exchange.request); // TODO: fix when poco is upgraded

        Utils::dumpRequestHeaders(exchange);
        
        handleOrigin(exchange);
        handleSubprotocols(exchange);
        handleExtensions(exchange);
        
        //////////////////////////////////////////////////////
        
        Poco::Net::WebSocket ws(exchange.request, exchange.response);
        ws.setReceiveTimeout(settings.receiveTimeout);
        ws.setSendTimeout(settings.sendTimeout);
        ws.setKeepAlive(settings.bKeepAlive);

        setIsConnected(true);

        WebSocketEventArgs eventArgs(*this);
        ofNotifyEvent(_manager.events.onOpenEvent, eventArgs, this);
        
        ofLogNotice("ServerWebSocketRouteHandler::handleRequest") << "WebSocket connection established.";

        char buffer[settings.bufferSize];
        memset(buffer,0,sizeof(buffer)); // initialize to 0

        int flags = 0;
        int numBytesReceived = 0;
//        int numBytesSent = 0;
        
        do {
            if(ws.poll(settings.pollTimeout, Poco::Net::Socket::SELECT_READ)) {
                numBytesReceived = ws.receiveFrame(buffer, sizeof(buffer), flags);
                
                if(numBytesReceived > 0) {
                    
                    WebSocketFrame frame(buffer,numBytesReceived,flags);
                    
                    if(settings.bAutoPingPongResponse) {
                        if(frame.isPing()) {
                            WebSocketFrame pongFrame(buffer,
                                                        numBytesReceived,
                                                        Poco::Net::WebSocket::FRAME_FLAG_FIN | Poco::Net::WebSocket::FRAME_OP_PONG);
                            sendFrame(pongFrame);
                        } else if(frame.isPong()) {
                            WebSocketFrame pingFrame(buffer,
                                                        numBytesReceived,
                                                        Poco::Net::WebSocket::FRAME_FLAG_FIN | Poco::Net::WebSocket::FRAME_OP_PING);
                            sendFrame(pingFrame);
                        }
                    }
                    
                    frameReceived(frame);
                    
                    WebSocketFrameEventArgs frameArgs(*this,frame);
                    ofNotifyEvent(_manager.events.onFrameReceivedEvent,frameArgs,this);
                    
                } else {
                    // clean shutdown
                    disconnect();
                }
            }

            // lock the queue while we work on it
            processFrameQueue(ws);
            
            if(ws.poll(settings.pollTimeout, Poco::Net::Socket::SELECT_ERROR)) {
                disconnect(); // locks!
                cout << "GOT ERROR KILLING IT!" << endl;
            }
            
        } while (_bIsConnected && (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
        
        ofLogNotice("ServerWebSocketRouteHandler::handleRequest") << "WebSocket connection closed.";
    
    } catch (const Poco::Net::WebSocketException& exc) {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "WebSocketException: " << exc.code() << " Desc: " << exc.what();
        WebSocketEventArgs eventArgs(*this);
        switch (exc.code()) {
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                eventArgs.setError(WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION);
                exchange.response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
                exchange.response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                exchange.response.setReason("WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION");
                break;
            case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
                eventArgs.setError(WS_ERR_NO_HANDSHAKE);
                exchange.response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                exchange.response.setReason("WS_ERR_NO_HANDSHAKE");
                break;
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
                eventArgs.setError(WS_ERR_HANDSHAKE_NO_VERSION);
                exchange.response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                exchange.response.setReason("WS_ERR_HANDSHAKE_NO_VERSION");
                break;
            case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                eventArgs.setError(WS_ERR_HANDSHAKE_NO_KEY);
                exchange.response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                exchange.response.setReason("WS_ERR_HANDSHAKE_NO_KEY");
                break;
        }
        
        sendErrorResponse(exchange.response);
        socketClosed();
        ofNotifyEvent(_manager.events.onErrorEvent, eventArgs, this);
    } catch (const Poco::TimeoutException& exc) {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "TimeoutException: " << exc.code() << " Desc: " << exc.what();
        socketClosed();
        WebSocketEventArgs eventArgs(*this,WS_ERR_TIMEOUT);
        ofNotifyEvent(_manager.events.onErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    } catch (const Poco::Net::NetException& exc) {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "NetException: " << exc.code() << " Desc: " << exc.what();
        socketClosed();
        WebSocketEventArgs eventArgs(*this,WS_ERR_NET_EXCEPTION);
        ofNotifyEvent(_manager.events.onErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    } catch (const exception& exc) {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "exception: " << exc.what();
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
        socketClosed();
        WebSocketEventArgs eventArgs(*this,WS_ERR_OTHER);
        ofNotifyEvent(_manager.events.onErrorEvent, eventArgs, this);
    } catch ( ... ) {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "... Unknown exception.";
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
        socketClosed();
        WebSocketEventArgs eventArgs(*this,WS_ERR_OTHER);
        ofNotifyEvent(_manager.events.onErrorEvent, eventArgs, this);
    }
    WebSocketEventArgs eventArgs(*this);
    ofNotifyEvent(_manager.events.onCloseEvent,eventArgs,this);
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::frameReceived(const WebSocketFrame& frame)
{
    WebSocketFrame _frame(frame);
    sendFrame(_frame); // echo
}

//------------------------------------------------------------------------------
bool WebSocketRouteHandler::sendFrame(const WebSocketFrame& frame)
{
    ofScopedLock lock(_mutex);
    if(_bIsConnected) {
        _frameQueue.push(frame);
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::frameSent(const WebSocketFrame& frame,
                                      int nBytesSent)
{
    ofLogVerbose("ServerWebSocketRouteHandler::frameSent") << frame.toString() << " nBytesSent=" << nBytesSent;
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::socketClosed()
{
    ofLogVerbose("ServerWebSocketRouteHandler::frameSent") << "Socket closed.";
}

//------------------------------------------------------------------------------
size_t WebSocketRouteHandler::getSendQueueSize() const
{
    ofScopedLock lock(_mutex);
    return _frameQueue.size();
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::clearSendQueue()
{
    ofScopedLock lock(_mutex);
    queue<WebSocketFrame> empty; // a way to clear queues.
    swap( _frameQueue, empty );
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::disconnect()
{
    setIsConnected(false);
}

//------------------------------------------------------------------------------
bool WebSocketRouteHandler::isConnected() const
{
    ofScopedLock lock(_mutex);
    return _bIsConnected;
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::setIsConnected(bool bIsConnected)
{
    ofScopedLock lock(_mutex);
    _bIsConnected = bIsConnected;
}


//------------------------------------------------------------------------------
string WebSocketRouteHandler::getSubprotocol() const
{
    ofScopedLock lock(_mutex);
    return settings.subprotocol;
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::handleOrigin(ServerExchange &exchange)
{
    ofLogError("ofxWebSocketRouteHandler::handleOrigin") << "TODO: handle/check origin";
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::handleSubprotocols(ServerExchange& exchange)
{   
    std::vector<std::string> tokens = ofSplitString(exchange.request.get("Sec-WebSocket-Protocol",""),
                                                    ",",
                                                    true,
                                                    true);

    bool validProtocol = _manager.selectSubprotocol(tokens,settings.subprotocol);
    
    // if we don't support the protocol, we don't send a Sec-WebSocket-Protocol header with the response.
    // in doing so, we leave the decision up to the client.  most clients should terminate the connection.
    // http://stackoverflow.com/questions/13545453/http-response-code-when-requested-websocket-subprotocol-isnt-supported-recogniz
    if(validProtocol && !settings.subprotocol.empty()) {
        exchange.response.set("Sec-WebSocket-Protocol", settings.subprotocol);
    } else {
        // we don't send anything, which is equiv to null according to the spec
        // exchange.response.set("Sec-WebSocket-Protocol", "null");
    }
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::handleExtensions(ServerExchange& exchange)
{
    if(exchange.request.has("Sec-WebSocket-Extensions")) {
        // TODO: support these
        // http://tools.ietf.org/html/draft-tyoshino-hybi-websocket-perframe-deflate-05
        // http://tools.ietf.org/html/draft-ietf-hybi-websocket-multiplexing-09
    }
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::processFrameQueue(Poco::Net::WebSocket& ws)
{
    int numBytesSent = 0;
    
    ofScopedLock lock(_mutex);
    while(!_frameQueue.empty()) {
        WebSocketFrame frame = _frameQueue.front();
        _frameQueue.pop();
        
        if(frame.size() > 0) {
            if(ws.poll(settings.pollTimeout, Poco::Net::Socket::SELECT_WRITE)) {
                numBytesSent = ws.sendFrame(frame.getBinaryBuffer(),
                                            frame.size(),
                                            frame.getFlags());
                
                WebSocketFrameEventArgs eventArgs(*this,frame);
                
                if(numBytesSent <= 0) {
                    ofLogWarning("ServerWebSocketRouteHandler::handleRequest") << "WebSocket numBytesSent <= 0";
                    eventArgs.setError(WS_ERROR_ZERO_BYTE_FRAME_SENT);
                } else if(numBytesSent < frame.size()) {
                    ofLogWarning("ServerWebSocketRouteHandler::handleRequest") << "WebSocket numBytesSent < frame.size()";
                    WebSocketEventArgs eventArgs(*this);
                    eventArgs.setError(WS_ERROR_INCOMPLETE_FRAME_SENT);
                }
                
                frameSent(frame,numBytesSent);
                
                ofNotifyEvent(_manager.events.onFrameSentEvent, eventArgs, this);
            }
            
        }
    }
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::applyFirefoxHack(Poco::Net::HTTPServerRequest& request)
{
    // HACK FOR FIREFOX
    // require websocket upgrade headers
    std::string connectionHeader = Poco::toLower(request.get("Connection", ""));
    if(Poco::icompare(connectionHeader, "Upgrade") != 0) {
        std::string userAgent = Poco::toLower(request.get("User-Agent",""));
        if(!userAgent.empty() &&
           !connectionHeader.empty() &&
           ofIsStringInString(userAgent,"firefox") &&
           ofIsStringInString(connectionHeader,"upgrade")) {
            // this request is coming from firefox, which is known to send things that look like:
            // Connection:keep-alive, Upgrade
            // thus failing the standard Poco upgrade test.
            // we can't do this here, but will do a similar hack in the handler
            request.set("Connection","Upgrade");
        }
    }
}


} } // namespace ofx::HTTP
