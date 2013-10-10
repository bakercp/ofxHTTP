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


#include "BaseWebSocketSessionManager.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
BaseWebSocketSessionManager::BaseWebSocketSessionManager()
{
}

//------------------------------------------------------------------------------
BaseWebSocketSessionManager::~BaseWebSocketSessionManager()
{
}

//------------------------------------------------------------------------------
bool BaseWebSocketSessionManager::sendFrame(AbstractWebSocketConnection* connection,
                                            const WebSocketFrame& frame)
{
    if(0 != connection)
    {
        return connection->sendFrame(frame);
    }
    else
    {
        ofLogError("BaseWebSocketSessionManager::sendFrame") << "handler == NULL";
        return false;
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::close(AbstractWebSocketConnection* connection)
{
    if(0 != connection)
    {
        connection->stop();
    }
    else
    {
        ofLogError("BaseWebSocketSessionManager::disconnect") << "handler == NULL";
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::close()
{
    ofScopedLock lock(_mutex);
    WebSocketConnectionsIter iter = _connections.begin();
    while(iter != _connections.end())
    {
        close(*iter);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool BaseWebSocketSessionManager::sendBinary(AbstractWebSocketConnection* connection,
                                             ofBaseHasPixels& image)
{
    return sendBinary(connection,image.getPixelsRef());
}

//------------------------------------------------------------------------------
bool BaseWebSocketSessionManager::sendBinary(AbstractWebSocketConnection* connection,
                                             ofPixels& pixels)
{
    return sendBinary(connection,pixels.getPixels(),
                      static_cast<unsigned int>(pixels.size()));
}

//------------------------------------------------------------------------------
bool BaseWebSocketSessionManager::sendBinary(AbstractWebSocketConnection* connection,
                                             unsigned char* data,
                                             std::size_t size)
{
    WebSocketFrame frame(data,size,Poco::Net::WebSocket::FRAME_BINARY);
    return sendFrame(connection,frame);
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::broadcast(ofPixels& pixels)
{
    ofScopedLock lock(_mutex);
    WebSocketConnectionsIter iter = _connections.begin();

    int numChannels = pixels.getNumChannels();
    int width       = pixels.getWidth();
    int height      = pixels.getHeight();

    while(iter != _connections.end())
    {
        ofPixels pixels;
        //sendFrame(*iter,frame);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::broadcast(const std::string& text)
{
    broadcast(WebSocketFrame(text));
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::broadcast(const WebSocketFrame& frame)
{
    ofScopedLock lock(_mutex);
    WebSocketConnectionsIter iter = _connections.begin();
    while(iter != _connections.end())
    {
        sendFrame(*iter,frame);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::registerWebSocketConnection(AbstractWebSocketConnection* connection)
{
    ofScopedLock lock(_mutex);
    if(!_connections.insert(connection).second)
    {
        ofLogError("BaseWebSocketSessionManager::registerRouteHandler") << "Element was already in set!";
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::unregisterWebSocketConnection(AbstractWebSocketConnection* connection)
{
    ofScopedLock lock(_mutex);
    // TODO, this will never return more than 1
    std::size_t numErased = _connections.erase(connection);

    if(numErased != 1)
    {
        ofLogError("BaseWebSocketSessionManager::unregisterRouteHandler") << "Num erased != 1: " << numErased;
    }
}

//------------------------------------------------------------------------------
std::size_t BaseWebSocketSessionManager::getNumWebSocketConnections()
{
    ofScopedLock lock(_mutex);
    return _connections.size();
}

////------------------------------------------------------------------------------
//BaseWebSocketSessionManager::SubprotocolList BaseWebSocketSessionManager::getAvailableSubprotcols()
//{
//    ofScopedLock lock(_mutex);
//
//    SubprotocolList availableSubprotocols;
//
//    EventMapIter eventIter = _eventMap.begin();
//
//    while(eventIter != _eventMap.end())
//    {
//        availableSubprotocols.push_back((*eventIter).first);
//        ++eventIter;
//    }
//
//    return availableSubprotocols;
//}
//
////------------------------------------------------------------------------------
//bool BaseWebSocketSessionManager::selectSubprotocol(const SubprotocolList& proposedSubprotocols,
//                                                    std::string& selectedSubprotocol)
//{
//    selectedSubprotocol.clear();
//
//    if(proposedSubprotocols.empty())
//    {
//        return false;
//    }
//    else
//    {
//        ofScopedLock lock(_mutex);
//        // TODO: use find
//        SubprotocolList::const_iterator iter = proposedSubprotocols.begin();
//        while(iter != proposedSubprotocols.end())
//        {
//            EventMapIter eventIter = _eventMap.find(*iter);
//            if(eventIter != _eventMap.end())
//            {
//                selectedSubprotocol = (*eventIter).first;
//                return true;
//            }
//            ++iter;
//        }
//        return false;
//    }
//}

    
} } // namespace ofx::HTTP
