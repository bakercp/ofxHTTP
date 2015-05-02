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


#include "ofx/HTTP/FileSystemRoute.h"
#include "ofUtils.h"
#include "ofx/MediaTypeMap.h"


namespace ofx {
namespace HTTP {


const std::string FileSystemRouteSettings::DEFAULT_DOCUMENT_ROOT = "DocumentRoot/";
const std::string FileSystemRouteSettings::DEFAULT_INDEX         = "index.html";


FileSystemRouteSettings::FileSystemRouteSettings(const std::string& routePathPattern,
                                                 bool requireSecurePort):
    BaseRouteSettings(routePathPattern,
                      requireSecurePort,
                      BaseRouteSettings::DEFAULT_HTTP_METHODS),
    _defaultIndex(DEFAULT_INDEX),
    _documentRoot(DEFAULT_DOCUMENT_ROOT),
    _autoCreateDocumentRoot(false),
    _requireDocumentRootInDataFolder(true)
{
}


FileSystemRouteSettings::~FileSystemRouteSettings()
{
}


void FileSystemRouteSettings::setDefaultIndex(const std::string& defaultIndex)
{
    _defaultIndex = defaultIndex;
}


const std::string& FileSystemRouteSettings::getDefaultIndex() const
{
    return _defaultIndex;
}


void FileSystemRouteSettings::setDocumentRoot(const std::string& documentRoot)
{
    _documentRoot = documentRoot;
}


const std::string& FileSystemRouteSettings::getDocumentRoot() const
{
    return _documentRoot;
}


void FileSystemRouteSettings::setAutoCreateDocumentRoot(bool autoCreateDocumentRoot)
{
    _autoCreateDocumentRoot = autoCreateDocumentRoot;
}


bool FileSystemRouteSettings::getAutoCreateDocumentRoot() const
{
    return _autoCreateDocumentRoot;
}


void FileSystemRouteSettings::setRequireDocumentRootInDataFolder(bool requireDocumentRootInDataFolder)
{
    _requireDocumentRootInDataFolder = requireDocumentRootInDataFolder;
}


bool FileSystemRouteSettings::getRequireDocumentRootInDataFolder() const
{
    return _requireDocumentRootInDataFolder;
}


FileSystemRoute::FileSystemRoute(const Settings& settings):
    BaseRoute_<FileSystemRouteSettings>(settings)
{
}


FileSystemRoute::~FileSystemRoute()
{
}


void FileSystemRoute::handleRequest(Poco::Net::HTTPServerRequest& request,
                                    Poco::Net::HTTPServerResponse& response)
{
    BaseRoute_<FileSystemRouteSettings>::handleRequest(request, response);

    Poco::Path dataFolder(ofToDataPath("", true));
    Poco::Path documentRoot(ofToDataPath(getSettings().getDocumentRoot(), true));

    std::string dataFolderString = dataFolder.toString();
    std::string documentRootString = documentRoot.toString();

    // Document root validity check.
    if (_settings.getRequireDocumentRootInDataFolder() &&
       (documentRootString.length() < dataFolderString.length() ||
        documentRootString.substr(0, dataFolderString.length()) != dataFolderString))
    {
        ofLogError("FileSystemRoute::handleRequest") << "Document Root is not a sub directory of the data folder.";
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        handleErrorRequest(request, response);
        return;
    }

    // check path

    Poco::URI uri(request.getURI());
    std::string path = uri.getPath(); // just get the path

    // make paths absolute
    if (path.empty())
    {
        path = "/";
    }

    Poco::Path requestPath = documentRoot.append(path).makeAbsolute();

    // add the default index if no filename is requested
    if (requestPath.getFileName().empty())
    {
        requestPath.append(getSettings().getDefaultIndex()).makeAbsolute();
    }

    std::string requestPathString = requestPath.toString();

    // double check path safety (not needed?)
    if ((requestPathString.length() < documentRootString.length() ||
         requestPathString.substr(0, documentRootString.length()) != documentRootString))
    {
        ofLogError("FileSystemRoute::handleRequest") << "Requested document not inside DocumentFolder.";
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        handleErrorRequest(request, response);
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
        ofLogError("FileSystemRoute::handleRequest") << exc.displayText();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        handleErrorRequest(request, response);
        return;
    }
    catch (const Poco::OpenFileException& exc)
    {
        ofLogError("FileSystemRoute::handleRequest") << exc.displayText();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        handleErrorRequest(request, response);
        return;
    }
    catch (const std::exception& exc)
    {
        ofLogError("FileSystemRoute::handleRequest") << "Unknown server error: " << exc.what();
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        handleErrorRequest(request, response);
        return;
    }
}


void FileSystemRoute::handleErrorRequest(Poco::Net::HTTPServerRequest& request,
                                         Poco::Net::HTTPServerResponse& response)
{
    // See if we have an html file with that error code.
    ofFile errorFile(_settings.getDocumentRoot() + "/" + ofToString(response.getStatus()) + ".html");

    if (errorFile.exists())
    {
        try
        {
            response.sendFile(errorFile.getAbsolutePath(), "text/html");
            return;
        }
        catch (const Poco::FileNotFoundException& exc)
        {
            ofLogVerbose("FileSystemRoute::sendErrorResponse") << "Poco::FileNotFoundException: " << exc.code() << " " << exc.displayText();
        }
        catch (const Poco::OpenFileException& exc)
        {
            ofLogVerbose("FileSystemRoute::sendErrorResponse") << "Poco::OpenFileException: " << exc.code() << " " << exc.displayText();
        }
        catch (const Poco::Exception& exc)
        {
            ofLogVerbose("FileSystemRoute::sendErrorResponse") << "Exception: " << exc.code() << " " << exc.displayText();
        }
        catch (const std::exception& exc)
        {
            ofLogVerbose("FileSystemRoute::sendErrorResponse") << "exception: " << exc.what();
        }
        catch ( ... )
        {
            ofLogVerbose("FileSystemRoute::sendErrorResponse") << "... Unknown exception.";
        }
    }
    
    // If nothing is returned, then base route will get it
    BaseRoute_<FileSystemRouteSettings>::handleErrorRequest(request, response);
}


Poco::Net::HTTPRequestHandler* FileSystemRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new RequestHandlerAdapter(*this);
}


} } // namespace ofx::HTTP
