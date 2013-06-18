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


#include "ServerRouteHandler.h"


namespace ofx {
namespace HTTP {
        

//------------------------------------------------------------------------------
BaseServerRouteHandler::BaseServerRouteHandler(const BaseServerRouteSettings& settings)
{
    //route = _settings.getRoute();

    // TODO: what is being done with _settings?
}

//------------------------------------------------------------------------------
BaseServerRouteHandler::~BaseServerRouteHandler()
{
}

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
    } else if(authStatus == Authentication::UNAUTHORIZED ||
              authStatus == Authentication::NO_CREDENTIALS) {
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
void BaseServerRouteHandler::handleExchange(ServerExchange& exchange)
{
    /*
    Poco::Path dataFolder(ofToDataPath("",true));
    Poco::Path documentRoot(ofToDataPath(_settings.documentRoot,true));

    std::string dataFolderString = dataFolder.toString();
    std::string documentRootString = documentRoot.toString();

    // doc root validity check
    if(_settings.bRequireDocumentRootInDataFolder &&
       (documentRootString.length() < dataFolderString.length() ||
        documentRootString.substr(0,dataFolderString.length()) != dataFolderString)) {
           ofLogError("ServerDefaultRouteHandler::handleRequest") << "Document Root is not a sub directory of the data folder.";
           exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
           sendErrorResponse(exchange.response);
           return;
       }

    // check path

    Poco::URI uri(exchange.request.getURI());
    std::string path = uri.getPath(); // just get the path

    // make paths absolute
    if(path.empty()) {
        path = "/";
    }

    Poco::Path requestPath = documentRoot.append(path).makeAbsolute();

    // add the default index if no filename is requested
    if(requestPath.getFileName().empty()) {
        requestPath.append(_settings.defaultIndex).makeAbsolute();
    }

    std::string requestPathString = requestPath.toString();

    // double check path safety (not needed?)
    if((requestPathString.length() < documentRootString.length() ||
        requestPathString.substr(0,documentRootString.length()) != documentRootString)) {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << "Requested document not inside DocumentFolder.";
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        sendErrorResponse(exchange.response);
        return;
    }

    ofFile file(requestPathString); // use it to parse file name parts

    try {
        ofx::Media::MediaTypeMap mediaMap;
        Poco::Net::MediaType mediaType = mediaMap.getMediaTypeForSuffix(file.getExtension());
        exchange.response.sendFile(file.getAbsolutePath(), mediaType.toString()); // will throw exceptions
    } catch(const Poco::FileNotFoundException& ex) {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << ex.displayText();
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        sendErrorResponse(exchange.response);
    } catch(const Poco::OpenFileException& ex) {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << ex.displayText();
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
    } catch(const exception& ex) {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << "Unknown server error: " << ex.what();
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
    }
*/
    exchange.response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    exchange.response.setReason("No Matching Routes Found.");
    sendErrorResponse(exchange.response);
}

//------------------------------------------------------------------------------
void BaseServerRouteHandler::sendErrorResponse(Poco::Net::HTTPServerResponse& response)
{
    /*
    // now check to see if the status was set something other than 200 by an exception

    Poco::Net::HTTPResponse::HTTPStatus responseStatus = response.getStatus();
    // see if we have an html file with that error code
    ofFile errorFile(settings.documentRoot + "/" + ofToString(responseStatus) + ".html");
    if(errorFile.exists()) {
        try {
            response.sendFile(errorFile.getAbsolutePath(),"text/html");
        } catch(const Poco::FileNotFoundException& exc) {
            BaseServerRouteHandler::sendErrorResponse(response);
        } catch(const Poco::OpenFileException& exc) {
            BaseServerRouteHandler::sendErrorResponse(response);
        } catch (const Poco::Exception& exc) {
            ofLogError("ServerRouteHandler::sendErrorResponse") << "Exception: " << exc.code() << " " << exc.displayText();
        } catch (const std::exception& exc) {
            ofLogError("ServerRouteHandler::sendErrorResponse") << "exception: " << exc.what();
        } catch ( ... ) {
            ofLogError("ServerRouteHandler::sendErrorResponse") << "... Unknown exception.";
        }
    } else {
        // we didn't have a corresponding error html in the DocumentRoot, so generate one
        BaseServerRouteHandler::sendErrorResponse(response);
    }

     */
    
    // we will assume that the sender has set the status and
    // reason appropriately before calling the sendErrorResponse()

    // watch out for situations when the response socket has been closed
    try {
        Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
        std::string reason = response.getReason();
        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");
        std::ostream& ostr = response.send(); // get output stream
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

//------------------------------------------------------------------------------
Authentication::Status BaseServerRouteHandler::authenticate(ServerExchange& exchange)
{
    return Authentication::OK;
}

//------------------------------------------------------------------------------
void BaseServerRouteHandler::updateSession(ServerExchange& exchange)
{
}


} } // namespace ofx::HTTP
