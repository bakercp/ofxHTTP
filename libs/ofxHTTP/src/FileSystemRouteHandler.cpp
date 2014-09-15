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


#include "ofx/HTTP/FileSystemRouteHandler.h"
#include "ofx/HTTP/FileSystemRoute.h"


namespace ofx {
namespace HTTP {


FileSystemRouteHandler::FileSystemRouteHandler(FileSystemRoute& parent):
    _parent(parent)
{
}


FileSystemRouteHandler::~FileSystemRouteHandler()
{
}


void FileSystemRouteHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                           Poco::Net::HTTPServerResponse& response)
{
    Poco::Path dataFolder(ofToDataPath("", true));
    Poco::Path documentRoot(ofToDataPath(_parent.getSettings().getDocumentRoot(), true));

    std::string dataFolderString = dataFolder.toString();
    std::string documentRootString = documentRoot.toString();

    // doc root validity check
    if(_parent.getSettings().getRequireDocumentRootInDataFolder() &&
       (documentRootString.length() < dataFolderString.length() ||
        documentRootString.substr(0, dataFolderString.length()) != dataFolderString))
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
        requestPathString.substr(0, documentRootString.length()) != documentRootString))
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << "Requested document not inside DocumentFolder.";
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        _parent.handleRequest(request,response);
        return;
    }

    ofFile file(requestPathString); // use it to parse file name parts
    std::string mediaTypeString = MediaTypeMap::getDefault()->getMediaTypeForPath(file.path()).toString();

    try
    {
        // TODO: this is where we would begin to work honoring
        /// Accept-Encoding:gzip, deflate, sdch
        response.sendFile(file.getAbsolutePath(), mediaTypeString);
        return;
    }
    catch (const Poco::FileNotFoundException& exc)
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << exc.displayText();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        _parent.handleRequest(request,response);
    }
    catch (const Poco::OpenFileException& exc)
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << exc.displayText();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request,response);
    }
    catch (const std::exception& exc)
    {
        ofLogError("ServerDefaultRouteHandler::handleRequest") << "Unknown server error: " << exc.what();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        _parent.handleRequest(request,response);
    }
}


} } // namespace ofx::HTTP
