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


#include "ofx/HTTP/Server/FileSystemRouteHandler.h"
#include "ofx/HTTP/Server/FileSystemRouteSettings.h"


namespace ofx {
namespace HTTP {


FileSystemRouteHandler::FileSystemRouteHandler(FileSystemRouteInterface& parent):
    BaseRouteHandler(parent),
    _parent(parent)
{
}

FileSystemRouteHandler::~FileSystemRouteHandler()
{
}

void FileSystemRouteHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                           Poco::Net::HTTPServerResponse& response)
{
    Poco::Path dataFolder(ofToDataPath("",true));
    Poco::Path documentRoot(ofToDataPath(_parent.getSettings().getDocumentRoot(),true));

    std::string dataFolderString = dataFolder.toString();
    std::string documentRootString = documentRoot.toString();

    // doc root validity check
    if(_parent.getSettings().getRequireDocumentRootInDataFolder() &&
       (documentRootString.length() < dataFolderString.length() ||
        documentRootString.substr(0,dataFolderString.length()) != dataFolderString))
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << "Document Root is not a sub directory of the data folder.";
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request,response);
        return;
    }

    // check path

    Poco::URI uri(request.getURI());
    std::string path = uri.getPath(); // just get the path

    // make paths absolute
    if(path.empty())
    {
        path = "/";
    }

    Poco::Path requestPath = documentRoot.append(path).makeAbsolute();

    // add the default index if no filename is requested
    if(requestPath.getFileName().empty())
    {
        requestPath.append(_parent.getSettings().getDefaultIndex()).makeAbsolute();
    }

    std::string requestPathString = requestPath.toString();

    // double check path safety (not needed?)
    if((requestPathString.length() < documentRootString.length() ||
        requestPathString.substr(0,documentRootString.length()) != documentRootString))
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << "Requested document not inside DocumentFolder.";
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        _parent.handleRequest(request,response);
        return;
    }

    ofFile file(requestPathString); // use it to parse file name parts

    try
    {
//        ofx::Media::MediaTypeMap mediaMap;
//        Poco::Net::MediaType mediaType = mediaMap.getMediaTypeForSuffix(file.getExtension());

        std::string mediaTypeString = "application/octet-stream";
        std::string ext = file.getExtension();


        if(ext == "json")
        {
            mediaTypeString = "application/json";
        }
        else if(ext == "html")
        {
            mediaTypeString = "text/html";
        }
        else if(ext == "jpg" || ext == "jpeg")
        {
            mediaTypeString = "image/jpeg";
        }
        else if(ext == "png")
        {
            mediaTypeString = "image/png";
        }
        else if(ext == "js")
        {
            mediaTypeString = "application/javascript";
        }
        else if(ext == "css")
        {
            mediaTypeString = "text/css";
        }
        else if(ext == "xml")
        {
            mediaTypeString = "application/xml";
        }
        else if(ext == "ico")
        {
            mediaTypeString = "image/x-icon";
        }

        response.sendFile(file.getAbsolutePath(), mediaTypeString); // will throw exceptions
        return;
    }
    catch (const Poco::FileNotFoundException& ex)
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << ex.displayText();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        _parent.handleRequest(request,response);
    }
    catch (const Poco::OpenFileException& ex)
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << ex.displayText();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request,response);
    }
    catch (const exception& ex)
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << "Unknown server error: " << ex.what();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request,response);
    }
}


} } // namespace ofx::HTTP
