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
bool BaseWebSocketSessionManager::sendFrame(BaseWebSocketRouteHandler* handler,
                                            const WebSocketFrame& frame)
{
    if(handler != NULL) {
        return handler->sendFrame(frame);
    } else {
        ofLogError("ofxBaseWebSocketSessionManager::sendFrame") << "handler == NULL";
        return false;
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::disconnect(BaseWebSocketRouteHandler* handler)
{
    if(handler != NULL) {
        handler->disconnect();
    } else {
        ofLogError("ofxBaseWebSocketSessionManager::disconnect") << "handler == NULL";
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::disconnectAll()
{
    Poco::FastMutex::ScopedLock lock(mutex);

    HandlerSetIter iter = handlers.begin();

    while(iter != handlers.end()) {
        disconnect(*iter);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool BaseWebSocketSessionManager::sendBinary(BaseWebSocketRouteHandler* handler,
                                             ofBaseHasPixels& image)
{
    return sendBinary(handler,image.getPixelsRef());
}

//------------------------------------------------------------------------------
bool BaseWebSocketSessionManager::sendBinary(BaseWebSocketRouteHandler* handler,
                                             ofPixels& pixels)
{
    return sendBinary(handler,pixels.getPixels(),static_cast<unsigned int>(pixels.size()));
}

//------------------------------------------------------------------------------
bool BaseWebSocketSessionManager::sendBinary(BaseWebSocketRouteHandler* handler,
                                             unsigned char* data,
                                             size_t size)
{
    WebSocketFrame frame(data,size,Poco::Net::WebSocket::FRAME_BINARY);
    return sendFrame(handler,frame);
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::broadcast(ofPixelsRef pixels)
{
    Poco::FastMutex::ScopedLock lock(mutex);
    std::set<BaseWebSocketRouteHandler*>::iterator iter = handlers.begin();

    int numChannels = pixels.getNumChannels();
    int width       = pixels.getWidth();
    int height      = pixels.getHeight();

    while(iter != handlers.end()) {
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
    Poco::FastMutex::ScopedLock lock(mutex);

    std::set<BaseWebSocketRouteHandler*>::iterator iter = handlers.begin();

    while(iter != handlers.end()) {
        sendFrame(*iter,frame);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::registerRouteHandler(BaseWebSocketRouteHandler* handler)
{
    cout << "registering ..." << endl;

    Poco::FastMutex::ScopedLock lock(mutex);
    if(!handlers.insert(handler).second) {
        ofLogError("ofxWebSocketManager::registerRouteHandler") << "Element was already in set!";
    }
}

//------------------------------------------------------------------------------
void BaseWebSocketSessionManager::unregisterRouteHandler(BaseWebSocketRouteHandler* handler)
{
    cout << "unregistering ..." << endl;
    Poco::FastMutex::ScopedLock lock(mutex);
    size_t numErased = handlers.erase(handler);

    if(numErased != 1) {
        ofLogError("ofxWebSocketManager::unregisterRouteHandler") << "Num erased != 1: " << numErased;
    }
}

//------------------------------------------------------------------------------
size_t BaseWebSocketSessionManager::getNumClientsConnected()
{
    Poco::FastMutex::ScopedLock lock(mutex);
    return handlers.size();
}

//------------------------------------------------------------------------------
std::vector<std::string> BaseWebSocketSessionManager::getAvailableSubprotcols()
{
    Poco::FastMutex::ScopedLock lock(mutex);

    std::vector<std::string> availableSubprotocols;

    EventMapIter eventIter = eventMap.begin();

    while(eventIter != eventMap.end()) {
        availableSubprotocols.push_back((*eventIter).first);
        ++eventIter;
    }

    return availableSubprotocols;
}

//------------------------------------------------------------------------------
bool BaseWebSocketSessionManager::selectSubprotocol(const std::vector<std::string>& proposedSubprotocols,
                                                    std::string& selectedSubprotocol)
{

    selectedSubprotocol.clear();

    if(proposedSubprotocols.empty()) {
        return false;
    } else {
        Poco::FastMutex::ScopedLock lock(mutex);

        std::vector<std::string>::const_iterator iter = proposedSubprotocols.begin();

        while(iter != proposedSubprotocols.end()) {

            EventMapIter eventIter = eventMap.find(*iter);

            if(eventIter != eventMap.end()) {
                selectedSubprotocol = (*eventIter).first;
                return true;
            }
            ++iter;
        }
        return false;
    }
}

    
} } // namespace ofx::HTTP
