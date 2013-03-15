#include "ofxHTTPServerUploadRouteHandler.h"

//------------------------------------------------------------------------------
ofxHTTPServerUploadRouteHandler::ofxHTTPServerUploadRouteHandler(const Settings& _settings) :
ofxHTTPServerRouteHandler(_settings),
settings(_settings)
{ }

//------------------------------------------------------------------------------
ofxHTTPServerUploadRouteHandler::~ofxHTTPServerUploadRouteHandler() { }

//------------------------------------------------------------------------------
void ofxHTTPServerUploadRouteHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
//    if(isValidRequest(request,response,settings.route)) {
//        
//        Path dataFolder(ofToDataPath("",true));
//        Path uploadFolder(ofToDataPath(settings.uploadFolder,true));
//        
//        string dataFolderString = dataFolder.toString();
//        string uploadFolderString = uploadFolder.toString();
//        
//        // upload folder validity check
//        if(settings.bRequireUploadFolderInDataFolder &&
//           (uploadFolderString.length() < dataFolderString.length() ||
//            uploadFolderString.substr(0,dataFolderString.length()) != dataFolderString)) {
//               ofLogError("ofxHTTPServerUploadRouteHandler::handleRequest") << "Upload folder is not a sub directory of the data folder.";
//               response.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
//               sendErrorResponse(response);
//               return;
//        }
//
//        HTMLForm form(request, request.stream(), *this);
//        
//        if(!settings.uploadRedirect.empty()) {
//            response.redirect(settings.uploadRedirect);
//        } else {
//            response.setStatusAndReason(HTTPResponse::HTTP_OK);
//            response.setContentLength(0);
//            response.send();
//        }
//    } else {
//        return; // isValidRequest took care of the response
//    }
}

//------------------------------------------------------------------------------
void ofxHTTPServerUploadRouteHandler::handlePart(const MessageHeader& header, std::istream& stream) {
    //NameValueCollection::ConstIterator iter = header.begin();
    
    //        while(iter != header.end()) {
    //            cout << (*iter).first << "=" << (*iter).second << endl;
    //            ++iter;
    //        }
    //
    
    if(header.has("Content-Type")) {
        string contentType = header["Content-Type"];
        if(!isContentTypeValid(contentType)) {
            ofLogError("ofxHTTPServerUploadRouteHandler::handlePart") << "Invalid content type: " << contentType;
            return; // reject
        }
    } else {
        ofLogError("ofxHTTPServerUploadRouteHandler::handlePart") << "No Content-Type header.";
        return;
    }
    
    // is this an uploaded file?
    if(header.has("Content-Disposition")) {// && header.has("form-data")) {
        string contentDisposition = header["Content-Disposition"];
        NameValueCollection parameters;
        MessageHeader::splitParameters(contentDisposition.begin(),contentDisposition.end(),parameters);
        
        if(parameters.has("filename")) {
            try {
            
                ofFile file(settings.uploadFolder + "/" + parameters["filename"], ofFile::WriteOnly);

                cout << file.getAbsolutePath() << endl;
                
                streamsize sz = StreamCopier::copyStream(stream,file,settings.writeBufferSize);
                
                cout << sz << endl;
                
                // The section below is from StreamCopier::copyStream,
                // and can be used for upload progress feedback
                //                int bufferSize = 8192;
                //                Buffer<char> buffer(bufferSize);
                //                streamsize len = 0;
                //                stream.read(buffer.begin(), bufferSize);
                //                streamsize n = stream.gcount();
                //                while (n > 0) {
                //                    len += n;
                //                    file.write(buffer.begin(), n);
                //                    if (stream && file) {
                //                        stream.read(buffer.begin(), bufferSize);
                //                        n = stream.gcount();
                //                        cout << n << endl;
                //                    }
                //                    else n = 0;
                //                }
                
                file.close();
                
            } catch(const Exception& exc) {
                ofLogError("ofxHTTPServerUploadRouteHandler::handlePart") << exc.displayText();
            } catch(const exception& exc) {
                ofLogError("ofxHTTPServerUploadRouteHandler::handlePart") << exc.what();
            } catch(...) {
                ofLogError("ofxHTTPServerUploadRouteHandler::handlePart") << "Uncaught thread exception: Unknown exception.";
            }
            
        } else {
            // error
        }
    }
    
}

//------------------------------------------------------------------------------
bool ofxHTTPServerUploadRouteHandler::isContentTypeValid(const string& contentType) const {
    // default true
    return true;
}

