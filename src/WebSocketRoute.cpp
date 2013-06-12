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
WebSocketRoute::WebSocketRoute(const Settings& settings)
: _settings(settings)
{

}
    
//------------------------------------------------------------------------------
WebSocketRoute::~WebSocketRoute() {
    ofLogVerbose("WebSocketRoute::~WebSocketRoute") << "Closing down route: " << getRoute() << endl;
}
    
//------------------------------------------------------------------------------
bool WebSocketRoute::canHandleRequest(const HTTPServerRequest& request,
                                      bool bIsSecurePort)
{
    // require HTTP_GET
    if(request.getMethod() != Poco::Net::HTTPRequest::HTTP_GET) {
        return false;
    }
    
    if(Poco::icompare(request.get("Upgrade", ""), "websocket")  != 0) {
        return false;
    }

    // require websocket upgrade headers
    // this is all fixed in Poco 1.4.6 and 1.5.+
    std::string connectionHeader = Poco::toLower(request.get("Connection", ""));
    
    if(Poco::icompare(connectionHeader, "Upgrade") != 0 &&
       !ofIsStringInString(connectionHeader,"upgrade")) {
       // this request is coming from firefox, which is known to send things that look like:
       // Connection:keep-alive, Upgrade
       // thus failing the standard Poco upgrade test.
       // we can't do this here, but will do a similar hack in the handler
        return false;
    }
    
    // require a valid path
    Poco::URI uri;
    
    try {
        uri = Poco::URI(request.getURI());
    } catch(const Poco::SyntaxException& exc) {
        ofLogError("ServerWebSocketRoute::canHandleRequest") << exc.what();
        return false;
    }
    
    // just get the path
    std::string path = uri.getPath();

    // make paths absolute
    if(path.empty()) { path = "/"; }
    
    return Poco::RegularExpression(_settings.getRoute()).match(path);
}
                                 
//------------------------------------------------------------------------------
HTTPRequestHandler* WebSocketRoute::createRequestHandler(const HTTPServerRequest& request)
{
    return new WebSocketRouteHandler(*this,_settings);
}


} }
