#include "ofxWebSocketRoute.h"

//------------------------------------------------------------------------------
ofxWebSocketRoute::ofxWebSocketRoute(const Settings& _settings) { }
    
//------------------------------------------------------------------------------
ofxWebSocketRoute::~ofxWebSocketRoute() {
    cout << "closing down route!" << endl;
}
    
//------------------------------------------------------------------------------
bool ofxWebSocketRoute::canHandleRequest(const HTTPServerRequest& request, bool bIsSecurePort) {
    // require HTTP_GET
    if(request.getMethod() != HTTPRequest::HTTP_GET) return false;
    
    if(icompare(request.get("Upgrade", ""), "websocket")  != 0) return false;

    // require websocket upgrade headers
    // this is all fixed in Poco 1.4.6 and 1.5.+
    string connectionHeader = toLower(request.get("Connection", ""));
    if(icompare(connectionHeader, "Upgrade") != 0 &&
       !ofIsStringInString(connectionHeader,"upgrade")) {
       // this request is coming from firefox, which is known to send things that look like:
       // Connection:keep-alive, Upgrade
       // thus failing the standard Poco upgrade test.
       // we can't do this here, but will do a similar hack in the handler
        return false;
    }
    
    // require a valid path
    URI uri;
    try {
        uri = URI(request.getURI());
    } catch(const SyntaxException& exc) {
        ofLogError("ofxHTTPServerWebSocketRoute::canHandleRequest") << exc.what();
        return false;
    }
    
    // just get the path
    string path = uri.getPath();
    // make paths absolute
    if(path.empty()) { path = "/"; }
    
    return RegularExpression(settings.getRoute()).match(path);
}
                                 
//------------------------------------------------------------------------------
HTTPRequestHandler* ofxWebSocketRoute::createRequestHandler(const HTTPServerRequest& request) {
    return new ofxWebSocketRouteHandler(*this,settings);
}
