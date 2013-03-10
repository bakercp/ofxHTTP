#include "ofxHTTPServerDefaultRouteHandler.h"

//------------------------------------------------------------------------------
ofxHTTPServerDefaultRouteHandler::Settings::Settings() {
    route        = "/.*";
    
    defaultIndex = "index.html";
    documentRoot = "DocumentRoot";
 
    // TODO:
    // e.g. http://httpd.apache.org/docs/2.2/mod/mod_deflate.html
    // or the reverse? compression type as key?
//    ofxHTTPCompressorEntry html(MediaType("text/html"));
//    html.addCompressionType(GZIP);
//    html.addCompressionType(DEFLATE);
//
//    ofxHTTPCompressorEntry plain(MediaType("text/plain"));
//    html.addCompressionType(GZIP);
//    html.addCompressionType(DEFLATE);

    bRequireDocumentRootInDataFolder = true;
    bAutoCreateDocumentRoot = false;
}

//------------------------------------------------------------------------------
ofxHTTPServerDefaultRouteHandler::ofxHTTPServerDefaultRouteHandler(const Settings& _settings) :
settings(_settings)
{ }

//------------------------------------------------------------------------------
ofxHTTPServerDefaultRouteHandler::~ofxHTTPServerDefaultRouteHandler() { }

//------------------------------------------------------------------------------
void ofxHTTPServerDefaultRouteHandler::handleExchange(ofxHTTPServerExchange& exchange) {

    Path dataFolder(ofToDataPath("",true));
    Path documentRoot(ofToDataPath(settings.documentRoot,true));
    
    string dataFolderString = dataFolder.toString();
    string documentRootString = documentRoot.toString();
    
    // doc root validity check
    if(settings.bRequireDocumentRootInDataFolder &&
       (documentRootString.length() < dataFolderString.length() ||
        documentRootString.substr(0,dataFolderString.length()) != dataFolderString)) {
           ofLogError("ofxHTTPServerDefaultRouteHandler::handleRequest") << "Document Root is not a sub directory of the data folder.";
           exchange.response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
           sendErrorResponse(exchange.response);
           return;
    }

    // check path
    
    URI uri(exchange.request.getURI());
    string path = uri.getPath(); // just get the path

    // make paths absolute
    if(path.empty()) { path = "/"; }
    
    Path requestPath = documentRoot.append(path).makeAbsolute();
    
    // add the default index if no filename is requested
    if(requestPath.getFileName().empty()) {
        requestPath.append(settings.defaultIndex).makeAbsolute();
    }
    
    string requestPathString = requestPath.toString();
    
    // double check path safety (not needed?)
    if((requestPathString.length() < documentRootString.length() ||
        requestPathString.substr(0,documentRootString.length()) != documentRootString)) {
           ofLogError("ofxHTTPServerDefaultRouteHandler::handleRequest") << "Requested document not inside DocumentFolder.";
           exchange.response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND);
           sendErrorResponse(exchange.response);
           return;
    }

    ofFile file(requestPathString); // use it to parse file name parts
    
    try {
        MediaType mediaType = ofxHTTPGetMimeType(file.getExtension());
        exchange.response.sendFile(file.getAbsolutePath(), mediaType.toString()); // will throw exceptions
    } catch (const FileNotFoundException& ex) {
        ofLogError("ofxHTTPServerDefaultRouteHandler::handleRequest") << ex.displayText();
        exchange.response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND);
        sendErrorResponse(exchange.response);
    } catch (const OpenFileException& ex) {
        ofLogError("ofxHTTPServerDefaultRouteHandler::handleRequest") << ex.displayText();
        exchange.response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
    } catch (const exception& ex) {
        ofLogError("ofxHTTPServerDefaultRouteHandler::handleRequest") << "Unknown server error: " << ex.what();
        exchange.response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        sendErrorResponse(exchange.response);
    }
}

//------------------------------------------------------------------------------
void ofxHTTPServerDefaultRouteHandler::sendErrorResponse(HTTPServerResponse& response) {
    // now check to see if the status was set something other than 200 by an exception
    
    HTTPResponse::HTTPStatus responseStatus = response.getStatus();
    // see if we have an html file with that error code
    ofFile errorFile(settings.documentRoot + "/" + ofToString(responseStatus) + ".html");
    if(errorFile.exists()) {
        try {
            response.sendFile(errorFile.getAbsolutePath(),"text/html");
        } catch(const FileNotFoundException& exc) {
            ofxHTTPServerRouteHandler::sendErrorResponse(response);
        } catch(const OpenFileException& exc) {
            ofxHTTPServerRouteHandler::sendErrorResponse(response);
        } catch (const Exception& exc) {
            ofLogError("ofxHTTPServerRouteHandler::sendErrorResponse") << "Exception: " << exc.code() << " " << exc.displayText();
        } catch (const exception& exc) {
            ofLogError("ofxHTTPServerRouteHandler::sendErrorResponse") << "exception: " << exc.what();
        } catch ( ... ) {
            ofLogError("ofxHTTPServerRouteHandler::sendErrorResponse") << "... Unknown exception.";
        }
    } else {
        // we didn't have a corresponding error html in the DocumentRoot, so generate one
        ofxHTTPServerRouteHandler::sendErrorResponse(response);
    }
}