

#include "WebSocketRouteHandler.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
WebSocketRouteHandler::WebSocketRouteHandler(BaseWebSocketSessionManager& manager,
                                             const Settings& settings)
: BaseWebSocketRouteHandler(settings)
, _manager(manager)
, _bIsConnected(false)
{
    manager.registerRouteHandler(this);
}

//------------------------------------------------------------------------------
WebSocketRouteHandler::~WebSocketRouteHandler()
{
    _manager.unregisterRouteHandler(this);
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::handleExchange(ServerExchange& exchange) {

    try {
        applyFirefoxHack(exchange.request); // TODO: fix when poco is upgraded

        ofDumpRequestHeaders(exchange);
        
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
void WebSocketRouteHandler::frameReceived(const WebSocketFrame& _frame) {
    WebSocketFrame frame(_frame);
    sendFrame(_frame); // echo
}

//------------------------------------------------------------------------------
bool WebSocketRouteHandler::sendFrame(const WebSocketFrame& _frame) {
    ofScopedLock lock(mutex);
    if(_bIsConnected) {
        frameQueue.push(_frame);
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::frameSent(const WebSocketFrame& _frame, int _nBytesSent) {
    ofLogVerbose("ServerWebSocketRouteHandler::frameSent") << _frame.toString() << " nBytesSent=" << _nBytesSent;
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::socketClosed() {
    ofLogVerbose("ServerWebSocketRouteHandler::frameSent") << "Socket closed.";
}

//------------------------------------------------------------------------------
size_t WebSocketRouteHandler::getSendQueueSize() const
{
    ofScopedLock lock(mutex);
    return frameQueue.size();
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::clearSendQueue() {
    ofScopedLock lock(mutex);
    queue<WebSocketFrame> empty; // a way to clear queues.
    swap( frameQueue, empty );
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::disconnect() {
    setIsConnected(false);
}

//------------------------------------------------------------------------------
bool WebSocketRouteHandler::isConnected() const
{
    ofScopedLock lock(mutex);
    return _bIsConnected;
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::setIsConnected(bool bIsConnected) {
    ofScopedLock lock(mutex);
    _bIsConnected = bIsConnected;
}


//------------------------------------------------------------------------------
string WebSocketRouteHandler::getSubprotocol() const {
    ofScopedLock lock(mutex);
    return settings.subprotocol;
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::handleOrigin(ServerExchange &exchange) {
    ofLogError("ofxWebSocketRouteHandler::handleOrigin") << "TODO: handle/check origin";
}

//------------------------------------------------------------------------------
void WebSocketRouteHandler::handleSubprotocols(ServerExchange& exchange) {
    
    bool validProtocol = _manager.selectSubprotocol(ofSplitString(exchange.request.get("Sec-WebSocket-Protocol",""),",",true,true),settings.subprotocol);
    
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
void WebSocketRouteHandler::handleExtensions(ServerExchange& exchange) {

    if(exchange.request.has("Sec-WebSocket-Extensions")) {
        // TODO: support these
        // http://tools.ietf.org/html/draft-tyoshino-hybi-websocket-perframe-deflate-05
        // http://tools.ietf.org/html/draft-ietf-hybi-websocket-multiplexing-09
    }
}

void WebSocketRouteHandler::processFrameQueue(Poco::Net::WebSocket& ws) {
    int numBytesSent = 0;
    
    ofScopedLock lock(mutex);
    while(!frameQueue.empty()) {
        WebSocketFrame frame = frameQueue.front();
        frameQueue.pop();
        
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


} } 
