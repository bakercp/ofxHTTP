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


#include "WebSocketRoute.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
WebSocketRoute::WebSocketRoute(const Settings& settings):
    _settings(settings)
{
}
    
//------------------------------------------------------------------------------
WebSocketRoute::~WebSocketRoute()
{
}

std::string WebSocketRoute::getRoutePathPattern() const
{
    return _settings.getRoutePathPattern();
}

bool WebSocketRoute::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                      bool isSecurePort) const
{
    // require HTTP_GET
    if(request.getMethod() != Poco::Net::HTTPRequest::HTTP_GET)
    {
        return false;
    }

    // check route
    if(!BaseRoute::canHandleRequest(request, isSecurePort))
    {
        return false;
    }

    // check to see if this is an websocket upgrade request
    if(Poco::icompare(request.get("Upgrade", ""), "websocket")  != 0)
    {
        return false;
    }

    // TODO: firefox hack
    // require websocket upgrade headers
    // this is all fixed in Poco 1.4.6 and 1.5.+
    std::string connectionHeader = Poco::toLower(request.get("Connection", ""));

    if(Poco::icompare(connectionHeader, "Upgrade") != 0 &&
       !ofIsStringInString(connectionHeader,"upgrade"))
    {
        // this request is coming from firefox, which is known to send things that look like:
        // Connection:keep-alive, Upgrade
        // thus failing the standard Poco upgrade test.
        // we can't do this here, but will do a similar hack in the handler
        return false;
    }
    
    return true;

}

Poco::Net::HTTPRequestHandler* WebSocketRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new WebSocketRouteHandler(*this);
}


void WebSocketRoute::stop()
{
    close(); // TODO: get method signatures in sync ...
}


WebSocketRouteSettings WebSocketRoute::getSettings() const
{
    return _settings;
}

BaseWebSocketSessionManager& WebSocketRoute::getSessionManagerRef()
{
    return *this;
}




} } // namespace ofx::HTTP
