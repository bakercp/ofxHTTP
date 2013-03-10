#include "ofxWebSocketRouteHandler.h"

//------------------------------------------------------------------------------
ofxWebSocketRouteHandler::Settings::Settings() {
    route = "/";
    bAutoPingPongResponse = true;
    bKeepAlive = true;
    
    bAllowEmptySubprotocol = false;
    bAllowCrossOriginConnections = false;
    
    bIsBinary = false;
    
    receiveTimeout = Timespan(60 * Timespan::SECONDS); // 60 second default
    sendTimeout    = Timespan(60 * Timespan::SECONDS); // 60 second default
    pollTimeout    = Timespan(10 * Timespan::MILLISECONDS); // brief pause to check the socket
    
    bufferSize = 1024;
};

//------------------------------------------------------------------------------
ofxWebSocketRouteHandler::ofxWebSocketRouteHandler(ofxBaseWebSocketSessionManager& _manager, const Settings& _settings)
:
settings(_settings),
manager(_manager),
subprotocol(""),
bIsConnected(false)
{
    manager.registerRouteHandler(this);
}

//------------------------------------------------------------------------------
ofxWebSocketRouteHandler::~ofxWebSocketRouteHandler() {
    manager.unregisterRouteHandler(this);
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::handleExchange(ofxHTTPServerExchange& exchange) {

    try {
        applyFirefoxHack(exchange.request); // TODO: fix when poco is upgraded

        ofDumpRequestHeaders(exchange);
        
        handleOrigin(exchange);
        handleSubprotocols(exchange);
        handleExtensions(exchange);
        
        //////////////////////////////////////////////////////
        
        WebSocket ws(exchange.request, exchange.response);
        ws.setReceiveTimeout(settings.receiveTimeout);
        ws.setSendTimeout(settings.sendTimeout);
        ws.setKeepAlive(settings.bKeepAlive);

        setIsConnected(true);

        ofxWebSocketEventArgs eventArgs(*this);
        ofNotifyEvent(manager.events.onOpenEvent, eventArgs, this);
        
        ofLogNotice("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "WebSocket connection established.";

        char buffer[settings.bufferSize];
        memset(buffer,0,sizeof(buffer)); // initialize to 0

        int flags = 0;
        int numBytesReceived = 0;
//        int numBytesSent = 0;
        
        do {
            if(ws.poll(settings.pollTimeout, Socket::SELECT_READ)) {
                numBytesReceived = ws.receiveFrame(buffer, sizeof(buffer), flags);
                
                if(numBytesReceived > 0) {
                    
                    ofxWebSocketFrame frame(buffer,numBytesReceived,flags);
                    
                    if(settings.bAutoPingPongResponse) {
                        if(frame.isPing()) {
                            ofxWebSocketFrame pongFrame(buffer,numBytesReceived,WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
                            sendFrame(pongFrame);
                        } else if(frame.isPong()) {
                            ofxWebSocketFrame pingFrame(buffer,numBytesReceived,WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PING);
                            sendFrame(pingFrame);
                        }
                    }
                    
                    frameReceived(frame);
                    
                    ofxWebSocketFrameEventArgs frameArgs(*this,frame);
                    ofNotifyEvent(manager.events.onFrameReceivedEvent,frameArgs,this);
                } else {
                    // clean shutdown
                    disconnect();
                }
            }

            // lock the queue while we work on it
            processFrameQueue(ws);
            
            if(ws.poll(settings.pollTimeout, Socket::SELECT_ERROR)) {
                disconnect(); // locks!
                cout << "GOT ERROR KILLING IT!" << endl;
            }
            
        } while (bIsConnected && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
        
        ofLogNotice("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "WebSocket connection closed.";
    
    } catch (const WebSocketException& exc) {
        ofLogError("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "WebSocketException: " << exc.code() << " Desc: " << exc.what();
        ofxWebSocketEventArgs eventArgs(*this);
        switch (exc.code()) {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                eventArgs.setError(WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION);
                exchange.response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
                exchange.response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                exchange.response.setReason("WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION");
                break;
            case WebSocket::WS_ERR_NO_HANDSHAKE:
                eventArgs.setError(WS_ERR_NO_HANDSHAKE);
                exchange.response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                exchange.response.setReason("WS_ERR_NO_HANDSHAKE");
                break;
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
                eventArgs.setError(WS_ERR_HANDSHAKE_NO_VERSION);
                exchange.response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                exchange.response.setReason("WS_ERR_HANDSHAKE_NO_VERSION");
                break;
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                eventArgs.setError(WS_ERR_HANDSHAKE_NO_KEY);
                exchange.response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                exchange.response.setReason("WS_ERR_HANDSHAKE_NO_KEY");
                break;
        }
        
        sendErrorResponse(exchange.response);
        socketClosed();
        ofNotifyEvent(manager.events.onErrorEvent, eventArgs, this);
    } catch (const TimeoutException& exc) {
        ofLogError("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "TimeoutException: " << exc.code() << " Desc: " << exc.what();
        socketClosed();
        ofxWebSocketEventArgs eventArgs(*this,WS_ERR_TIMEOUT);
        ofNotifyEvent(manager.events.onErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    } catch (const NetException& exc) {
        ofLogError("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "NetException: " << exc.code() << " Desc: " << exc.what();
        socketClosed();
        ofxWebSocketEventArgs eventArgs(*this,WS_ERR_NET_EXCEPTION);
        ofNotifyEvent(manager.events.onErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    } catch (const exception& exc) {
        ofLogError("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "exception: " << exc.what();
        exchange.response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
        socketClosed();
        ofxWebSocketEventArgs eventArgs(*this,WS_ERR_OTHER);
        ofNotifyEvent(manager.events.onErrorEvent, eventArgs, this);
    } catch ( ... ) {
        ofLogError("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "... Unknown exception.";
        exchange.response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
        socketClosed();
        ofxWebSocketEventArgs eventArgs(*this,WS_ERR_OTHER);
        ofNotifyEvent(manager.events.onErrorEvent, eventArgs, this);
    }
    ofxWebSocketEventArgs eventArgs(*this);
    ofNotifyEvent(manager.events.onCloseEvent,eventArgs,this);
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::frameReceived(const ofxWebSocketFrame& _frame) {
    ofxWebSocketFrame frame(_frame);
    sendFrame(_frame); // echo
}

//------------------------------------------------------------------------------
bool ofxWebSocketRouteHandler::sendFrame(const ofxWebSocketFrame& _frame) {
    ofScopedLock lock(mutex);
    if(bIsConnected) {
        frameQueue.push(_frame);
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::frameSent(const ofxWebSocketFrame& _frame, int _nBytesSent) {
    ofLogVerbose("ofxHTTPServerWebSocketRouteHandler::frameSent") << _frame.toString() << " nBytesSent=" << _nBytesSent;
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::socketClosed() {
    ofLogVerbose("ofxHTTPServerWebSocketRouteHandler::frameSent") << "Socket closed.";
}

//------------------------------------------------------------------------------
size_t ofxWebSocketRouteHandler::getSendQueueSize() {
    ofScopedLock lock(mutex);
    return frameQueue.size();
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::clearSendQueue() {
    ofScopedLock lock(mutex);
    queue<ofxWebSocketFrame> empty; // a way to clear queues.
    swap( frameQueue, empty );
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::disconnect() {
    setIsConnected(false);
}

//------------------------------------------------------------------------------
bool ofxWebSocketRouteHandler::isConnected() {
    ofScopedLock lock(mutex);
    return bIsConnected;
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::setIsConnected(bool _bIsConnected) {
    ofScopedLock lock(mutex);
    bIsConnected = _bIsConnected;
}


//------------------------------------------------------------------------------
string ofxWebSocketRouteHandler::getSubprotocol() {
    ofScopedLock lock(mutex);
    return subprotocol;
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::handleOrigin(ofxHTTPServerExchange &exchange) {
    ofLogError("ofxWebSocketRouteHandler::handleOrigin") << "TODO: handle/check origin";
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::handleSubprotocols(ofxHTTPServerExchange& exchange) {
    
    bool validProtocol = manager.selectSubprotocol(ofSplitString(exchange.request.get("Sec-WebSocket-Protocol",""),",",true,true),subprotocol);
    
    // if we don't support the protocol, we don't send a Sec-WebSocket-Protocol header with the response.
    // in doing so, we leave the decision up to the client.  most clients should terminate the connection.
    // http://stackoverflow.com/questions/13545453/http-response-code-when-requested-websocket-subprotocol-isnt-supported-recogniz
    if(validProtocol && !subprotocol.empty()) {
        exchange.response.set("Sec-WebSocket-Protocol", subprotocol);
    } else {
        // we don't send anything, which is equiv to null according to the spec
        // exchange.response.set("Sec-WebSocket-Protocol", "null");
    }
}

//------------------------------------------------------------------------------
void ofxWebSocketRouteHandler::handleExtensions(ofxHTTPServerExchange& exchange) {
    if(exchange.request.has("Sec-WebSocket-Extensions")) {
        // TODO: support these
        // http://tools.ietf.org/html/draft-tyoshino-hybi-websocket-perframe-deflate-05
        // http://tools.ietf.org/html/draft-ietf-hybi-websocket-multiplexing-09
    }
}

void ofxWebSocketRouteHandler::processFrameQueue(WebSocket& ws) {
    int numBytesSent = 0;
    
    ofScopedLock lock(mutex);
    while(!frameQueue.empty()) {
        ofxWebSocketFrame frame = frameQueue.front();
        frameQueue.pop();
        
        if(frame.size() > 0) {
            if(ws.poll(settings.pollTimeout, Socket::SELECT_WRITE)) {
                numBytesSent = ws.sendFrame(frame.getBinaryBuffer(),
                                            frame.size(),
                                            frame.getFlags());
                
                ofxWebSocketFrameEventArgs eventArgs(*this,frame);
                
                if(numBytesSent <= 0) {
                    ofLogWarning("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "WebSocket numBytesSent <= 0";
                    eventArgs.setError(WS_ERROR_ZERO_BYTE_FRAME_SENT);
                } else if(numBytesSent < frame.size()) {
                    ofLogWarning("ofxHTTPServerWebSocketRouteHandler::handleRequest") << "WebSocket numBytesSent < frame.size()";
                    ofxWebSocketEventArgs eventArgs(*this);
                    eventArgs.setError(WS_ERROR_INCOMPLETE_FRAME_SENT);
                }
                
                frameSent(frame,numBytesSent);
                
                ofNotifyEvent(manager.events.onFrameSentEvent, eventArgs, this);
            }
            
        }
    }
}



