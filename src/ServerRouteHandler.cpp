#include "ServerRouteHandler.h"


namespace ofx {
namespace HTTP {
        

//------------------------------------------------------------------------------
BaseServerRouteHandler::BaseServerRouteHandler(const BaseRouteSettings& _settings) {
    //route = _settings.getRoute();
}

//------------------------------------------------------------------------------
BaseServerRouteHandler::~BaseServerRouteHandler() { }

//------------------------------------------------------------------------------
void BaseServerRouteHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                       Poco::Net::HTTPServerResponse& response)
{
    ServerExchange exchange(request,response);
    Authentication::Status authStatus = authenticate(exchange);
    if(authStatus == Authentication::OK) {
        updateSession(exchange);
        handleExchange(exchange);
        return;
    } else if(authStatus == Authentication::UNAUTHORIZED || authStatus == Authentication::NO_CREDENTIALS) {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        sendErrorResponse(response);
        return;
    } else {
        ofLogError("ServerBaseRouteHandler::handleRequest") << "Unknown ofxHTTPAuthenticationStatus: " << authStatus;
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(response);
        return;
    }
}

////------------------------------------------------------------------------------
//string ServerRouteHandler::getRoute() const {
//    return route;
//}

////------------------------------------------------------------------------------
//bool ServerDefaultRouteHandler::isValidRequest(HTTPServerRequest& request,
//                                                   HTTPServerResponse& response,
//                                                   const Settings& settings) {
//    
//    string sessionId = "";
//    
//    // extract cookie from request
//    NameValueCollection cookies;
//    request.getCookies(cookies);
//    NameValueCollection::ConstIterator it = cookies.find(settings.sessionCookieName);
//    if (it != cookies.end()) {
//        sessionId = it->second;
//    } else {
//        sessionId = ServerSessionManager::generateSessionKey(request);
//        HTTPCookie cookie(settings.sessionCookieName,sessionId);
//        cookie.setPath("/");
//        // cookie.setDomain(""); // set the cookie's domain to the virtual host?
//        // set no age, so it expires @ end of session
//        response.addCookie(cookie);
//    }
//        
//    // TODO: update session manager
//    
//    URI uri(request.getURI());
//    
//    const string path = uri.getPath(); // just get the path
//
//    if(settings.requireAuthentication) {
//        if(request.hasCredentials()) {
//            HTTPBasicCredentials credentials(request);
//
//            if(settings.username == credentials.getUsername() &&
//               settings.password == credentials.getPassword()) {
//                // add an authentication cookie?
//                return true;
//            } else {
//                response.setStatusAndReason(HTTPResponse::HTTP_UNAUTHORIZED);
//                sendErrorResponse(response);
//                return false;
//            }
//        } else {
//            response.requireAuthentication(settings.realm);
//            response.setContentLength(0);
//            response.send();
//            return false;
//        }
//    } else {
//        return true;
//    }
//}

//------------------------------------------------------------------------------
void BaseServerRouteHandler::handleExchange(ServerExchange& exchange) {
    exchange.response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    exchange.response.setReason("No Matching Routes Found.");
    sendErrorResponse(exchange.response);
}

//------------------------------------------------------------------------------
void BaseServerRouteHandler::sendErrorResponse(Poco::Net::HTTPServerResponse& response) {
    // we will assume that the sender has set the status and
    // reason appropriately before calling the sendErrorResponse()

    // watch out for situations when the response socket has been closed
    try {
        Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
        string reason = response.getReason();
        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");
        ostream& ostr = response.send(); // get output stream
        ostr << "<html>";
        ostr << "<head><title>" << status << " - " << reason << "</title></head>";
        ostr << "<body>";
        ostr << "<h1>" << status << " - " << reason << "</h1>";
        ostr << "</body>";
        ostr << "<html>";
    } catch (const Poco::Exception& exc) {
        ofLogError("ServerRouteHandler::sendErrorResponse") << "Exception: " << exc.code() << " " << exc.displayText();
    } catch (const std::exception& exc) {
        ofLogError("ServerRouteHandler::sendErrorResponse") << "exception: " << exc.what();
    } catch ( ... ) {
        ofLogError("ServerRouteHandler::sendErrorResponse") << "... Unknown exception.";
    }
}


} }
