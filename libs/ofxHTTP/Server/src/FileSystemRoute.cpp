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


#include "ofx/HTTP/Server/FileSystemRoute.h"


namespace ofx {
namespace HTTP {


FileSystemRoute::FileSystemRoute(const Settings& settings):
    _settings(settings)
{
}

FileSystemRoute::~FileSystemRoute()
{
}

bool FileSystemRoute::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                       bool isSecurePort) const
{
    // require an HTTP_GET call
    return request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        BaseRoute::canHandleRequest(request, isSecurePort);
}

void FileSystemRoute::handleRequest(Poco::Net::HTTPServerRequest& request,
                                    Poco::Net::HTTPServerResponse& response)
{
    // see if we have an html file with that error code
    ofFile errorFile(_settings.getDocumentRoot() + "/" + ofToString(response.getStatus()) + ".html");

    if(errorFile.exists())
    {
        try
        {
            response.sendFile(errorFile.getAbsolutePath(),"text/html");
            return;
        }
        catch (const Poco::FileNotFoundException& exc)
        {
            ofLogVerbose("ServerRouteHandler::sendErrorResponse") << "Poco::FileNotFoundException: " << exc.code() << " " << exc.displayText();
        }
        catch (const Poco::OpenFileException& exc)
        {
            ofLogVerbose("ServerRouteHandler::sendErrorResponse") << "Poco::OpenFileException: " << exc.code() << " " << exc.displayText();
        }
        catch (const Poco::Exception& exc)
        {
            ofLogVerbose("ServerRouteHandler::sendErrorResponse") << "Exception: " << exc.code() << " " << exc.displayText();
        }
        catch (const std::exception& exc)
        {
            ofLogVerbose("ServerRouteHandler::sendErrorResponse") << "exception: " << exc.what();
        }
        catch ( ... )
        {
            ofLogVerbose("ServerRouteHandler::sendErrorResponse") << "... Unknown exception.";
        }
    }
    // if nothing is returned, then base route will get it

    BaseRoute::handleRequest(request,response);
}

Poco::Net::HTTPRequestHandler* FileSystemRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
  return new FileSystemRouteHandler(*this);
}


FileSystemRouteSettings FileSystemRoute::getSettings() const
{
    return _settings;
}


} } // namespace ofx::HTTP
