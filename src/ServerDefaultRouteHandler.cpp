#include "ServerDefaultRouteHandler.h"


namespace ofx {
namespace HTTP {
        

//------------------------------------------------------------------------------
ServerDefaultRouteHandler::ServerDefaultRouteHandler(const ServerDefaultRouteSettings& _settings) :
ServerRouteHandler(_settings),
settings(_settings)
{ }

//------------------------------------------------------------------------------
ServerDefaultRouteHandler::~ServerDefaultRouteHandler() { }

//------------------------------------------------------------------------------
void ServerDefaultRouteHandler::handleExchange(ServerExchange& exchange) {

    Poco::Path dataFolder(ofToDataPath("",true));
    Poco::Path documentRoot(ofToDataPath(settings.documentRoot,true));
    
    std::string dataFolderString = dataFolder.toString();
    std::string documentRootString = documentRoot.toString();
    
    // doc root validity check
    if(settings.bRequireDocumentRootInDataFolder &&
       (documentRootString.length() < dataFolderString.length() ||
        documentRootString.substr(0,dataFolderString.length()) != dataFolderString)) {
           ofLogError("ServerDefaultRouteHandler::handleRequest") << "Document Root is not a sub directory of the data folder.";
           exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
           sendErrorResponse(exchange.response);
           return;
    }

    // check path
    
    Poco::URI uri(exchange.request.getURI());
    string path = uri.getPath(); // just get the path

    // make paths absolute
    if(path.empty()) { path = "/"; }
    
    Poco::Path requestPath = documentRoot.append(path).makeAbsolute();
    
    // add the default index if no filename is requested
    if(requestPath.getFileName().empty()) {
        requestPath.append(settings.defaultIndex).makeAbsolute();
    }
    
    string requestPathString = requestPath.toString();
    
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
    } catch (const Poco::FileNotFoundException& ex) {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << ex.displayText();
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        sendErrorResponse(exchange.response);
    } catch (const Poco::OpenFileException& ex) {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << ex.displayText();
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
    } catch (const exception& ex) {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << "Unknown server error: " << ex.what();
        exchange.response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
    }
}

//------------------------------------------------------------------------------
void ServerDefaultRouteHandler::sendErrorResponse(Poco::Net::HTTPServerResponse& response) {
    // now check to see if the status was set something other than 200 by an exception
    
    Poco::Net::HTTPResponse::HTTPStatus responseStatus = response.getStatus();
    // see if we have an html file with that error code
    ofFile errorFile(settings.documentRoot + "/" + ofToString(responseStatus) + ".html");
    if(errorFile.exists()) {
        try {
            response.sendFile(errorFile.getAbsolutePath(),"text/html");
        } catch(const Poco::FileNotFoundException& exc) {
            ServerRouteHandler::sendErrorResponse(response);
        } catch(const Poco::OpenFileException& exc) {
            ServerRouteHandler::sendErrorResponse(response);
        } catch (const Poco::Exception& exc) {
            ofLogError("ServerRouteHandler::sendErrorResponse") << "Exception: " << exc.code() << " " << exc.displayText();
        } catch (const std::exception& exc) {
            ofLogError("ServerRouteHandler::sendErrorResponse") << "exception: " << exc.what();
        } catch ( ... ) {
            ofLogError("ServerRouteHandler::sendErrorResponse") << "... Unknown exception.";
        }
    } else {
        // we didn't have a corresponding error html in the DocumentRoot, so generate one
        ServerRouteHandler::sendErrorResponse(response);
    }
}

} }
