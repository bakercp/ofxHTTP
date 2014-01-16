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


#include "ofx/HTTP/WebSocket/WebSocketConnection.h"
#include "ofx/HTTP/WebSocket/WebSocketRoute.h"


namespace ofx {
namespace HTTP {


WebSocketConnection::WebSocketConnection(WebSocketRoute& parent):
    _parent(parent),
    _isConnected(false)
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

    try
    {
        // TODO: copy the request?
        _requestHeaders = request;
        _clientAddress = request.clientAddress();

        applyFirefoxHack(request); // TODO: fix when poco is upgraded

        //Utils::dumpHeaders(request,response);

        handleOrigin(request,response);
//        handleSubprotocols(request,response);
        handleExtensions(request,response);

        //////////////////////////////////////////////////////

        Poco::Net::WebSocket ws(request, response);
        ws.setReceiveTimeout(_parent.getSettings().getReceiveTimeout());
        ws.setSendTimeout(_parent.getSettings().getSendTimeout());
        ws.setKeepAlive(_parent.getSettings().getKeepAlive());

        _mutex.lock();
        _isConnected = true;
        _mutex.unlock();

//        WebSocketEventArgs eventArgs(*this);
//        ofNotifyEvent(_manager.events.onOpenEvent, eventArgs, this);

        ofLogNotice("ServerWebSocketRouteHandler::handleRequest") << "WebSocket connection established.";


        Poco::Buffer<char> buffer(_parent.getSettings().getBufferSize());

        int flags = 0;
        int numBytesReceived = 0;
        //        int numBytesSent = 0;

        WebSocketEventArgs eventArgs(*this);
        ofNotifyEvent(_parent.events.onOpenEvent, eventArgs, this);

        do
        {
            if(ws.poll(_parent.getSettings().getPollTimeout(),Poco::Net::Socket::SELECT_READ))
            {
                numBytesReceived = ws.receiveFrame(buffer.begin(), buffer.size(), flags);

                if(numBytesReceived > 0)
                {
                    WebSocketFrame frame(buffer.begin(), numBytesReceived, flags);

                    if(_parent.getSettings().getAutoPingPongResponse())
                    {
                        if(frame.isPing())
                        {
                            WebSocketFrame pongFrame(buffer.begin(),
                                                     numBytesReceived,
                                                     Poco::Net::WebSocket::FRAME_FLAG_FIN | Poco::Net::WebSocket::FRAME_OP_PONG);
                            sendFrame(pongFrame);
                        }
                        else if(frame.isPong())
                        {
                            WebSocketFrame pingFrame(buffer.begin(),
                                                     numBytesReceived,
                                                     Poco::Net::WebSocket::FRAME_FLAG_FIN | Poco::Net::WebSocket::FRAME_OP_PING);
                            sendFrame(pingFrame);
                        }
                    }

                    frameReceived(frame);

                    WebSocketFrameEventArgs frameArgs(frame, *this);
                    ofNotifyEvent(_parent.events.onFrameReceivedEvent, frameArgs, this);

                }
                else
                {
                    // clean shutdown
                    close();
                }
            }

            // lock the queue while we work on it
            processFrameQueue(ws);

            if(ws.poll(_parent.getSettings().getPollTimeout(),Poco::Net::Socket::SELECT_ERROR))
            {
                close(); // locks!
                //cout << "GOT ERROR KILLING IT!" << endl;
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
        socketClosed();

        WebSocketEventArgs eventArgs(*this, (WebSocketError)exc.code());

        ofNotifyEvent(_parent.events.onErrorEvent, eventArgs, this);
    }
    catch (const Poco::TimeoutException& exc)
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "TimeoutException: " << exc.code() << " Desc: " << exc.what();
        socketClosed();
        WebSocketEventArgs eventArgs(*this, WS_ERR_TIMEOUT);
        ofNotifyEvent(_parent.events.onErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    }
    catch (const Poco::Net::NetException& exc)
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "NetException: " << exc.code() << " Desc: " << exc.what();
        socketClosed();
        WebSocketEventArgs eventArgs(*this, WS_ERR_NET_EXCEPTION);
        ofNotifyEvent(_parent.events.onErrorEvent, eventArgs, this);
        // response socket has already been closed (!?)
    }
    catch (const std::exception& exc)
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "exception: " << exc.what();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request, response);
        socketClosed();
        WebSocketEventArgs eventArgs(*this, WS_ERR_OTHER);
        ofNotifyEvent(_parent.events.onErrorEvent, eventArgs, this);
    }
    catch ( ... )
    {
        ofLogError("ServerWebSocketRouteHandler::handleRequest") << "... Unknown exception.";
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request,response);
        socketClosed();
        WebSocketEventArgs eventArgs(*this, WS_ERR_OTHER);
        ofNotifyEvent(_parent.events.onErrorEvent, eventArgs, this);
    }

    WebSocketEventArgs eventArgs(*this);
    ofNotifyEvent(_parent.events.onCloseEvent, eventArgs, this);
}


void WebSocketConnection::frameReceived(const WebSocketFrame& frame)
{
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
        return false;
    }
}


void WebSocketConnection::frameSent(const WebSocketFrame& frame,
                                    std::size_t nBytesSent)
{
}


void WebSocketConnection::socketClosed()
{
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


//
//std::string WebSocketConnection::getSubprotocol() const
//{
//    ofScopedLock lock(_mutex);
//    return _settings.getSubprotocol();
//}


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
    Utils::dumpHeaders(request,OF_LOG_NOTICE);
    // Access-Control-Allow-Origin

    //    ofLogError("ServerRouteHandler::handleOrigin") << "TODO: handle/check origin";
}

//
//void WebSocketConnection::handleSubprotocols(Poco::Net::HTTPServerRequest& request,
//                                             Poco::Net::HTTPServerResponse& response)
//{
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
//}


void WebSocketConnection::handleExtensions(Poco::Net::HTTPServerRequest& request,
                                           Poco::Net::HTTPServerResponse& response)
{
    if(request.has("Sec-WebSocket-Extensions"))
    {
        // TODO: support these
        // http://tools.ietf.org/html/draft-tyoshino-hybi-websocket-perframe-deflate-05
        // http://tools.ietf.org/html/draft-ietf-hybi-websocket-multiplexing-09
    }
}


void WebSocketConnection::processFrameQueue(Poco::Net::WebSocket& ws)
{
    int numBytesSent = 0;
    
    ofScopedLock lock(_mutex);
    while(!_frameQueue.empty())
    {
        WebSocketFrame frame = _frameQueue.front();
        _frameQueue.pop();
        
        if(frame.size() > 0)
        {
            if(ws.poll(_parent.getSettings().getPollTimeout(), Poco::Net::Socket::SELECT_WRITE))
            {
//                cout << "frame " << frame.getBinaryBuffer() << " sz=" << frame.size() << " flags=" << frame.getFlags() << endl;


                numBytesSent = ws.sendFrame(frame.getBinaryBuffer(),
                                            frame.size(),
                                            frame.getFlags());
                
                WebSocketError error = WS_ERR_NONE;

                if(numBytesSent <= 0)
                {
                    ofLogWarning("ServerWebSocketRouteHandler::handleRequest") << "WebSocket numBytesSent <= 0";
                    error = WS_ERROR_ZERO_BYTE_FRAME_SENT;
                }
                else if(numBytesSent < frame.size())
                {
                    ofLogWarning("ServerWebSocketRouteHandler::handleRequest") << "WebSocket numBytesSent < frame.size()";
                    WebSocketEventArgs eventArgs(*this);
                    error = WS_ERROR_INCOMPLETE_FRAME_SENT;
                }

                WebSocketFrameEventArgs eventArgs(frame, *this, error);

                frameSent(frame, numBytesSent);
                
                ofNotifyEvent(_parent.events.onFrameSentEvent, eventArgs, this);
            }
            
        }
    }
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
