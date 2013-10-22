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


#include "ofx/HTTP/Server/BaseRoute.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
BaseRoute::BaseRoute()
{
}

//------------------------------------------------------------------------------
BaseRoute::~BaseRoute()
{
}

//------------------------------------------------------------------------------
std::string BaseRoute::getRoutePathPattern() const
{
    return BaseRouteSettings::DEFAULT_ROUTE_PATH_PATTERN;
}

//------------------------------------------------------------------------------
bool BaseRoute::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                 bool isSecurePort) const
{
    // require a valid path
    std::string path = "/";

    try
    {
        path = Poco::URI(request.getURI()).getPath();
    }
    catch (const Poco::SyntaxException& exc)
    {
        ofLogError("BaseRoute::canHandleRequest") << exc.what();
        return false;
    }

    if(path.empty())
    {
        path = "/";
    }

    return Poco::RegularExpression(getRoutePathPattern()).match(path);
}

//------------------------------------------------------------------------------
Poco::Net::HTTPRequestHandler* BaseRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new BaseRouteHandler(*this);
}

//------------------------------------------------------------------------------
void BaseRoute::handleRequest(Poco::Net::HTTPServerRequest& request,
                              Poco::Net::HTTPServerResponse& response)
{
    // We gave the handlers every opportunity to send a response.
    // Now we must conclude that there is a server error.
    try
    {
        // if we got this far and our status is still marked as 200, that constitutes a server error.
        if(response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "No handlers for route.");
        }

        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");

        std::ostream& ostr = response.send(); // get output stream
        ostr << "<html>";
        ostr << "<head><title>" << response.getStatus() << " - " << response.getReason() << "</title></head>";
        ostr << "<body>";
        ostr << "<h1>" << response.getStatus() << " - " << response.getReason() << "</h1>";
        ostr << "</body>";
        ostr << "<html>";
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("BaseRoute::handleRequest") << "Exception: " << exc.code() << " " << exc.displayText();
    }
    catch (const std::exception& exc)
    {
        ofLogError("BaseRoute::handleRequest") << "exception: " << exc.what();
    }
    catch ( ... )
    {
        ofLogError("BaseRoute::handleRequest") << "... Unknown exception.";
    }
}

//------------------------------------------------------------------------------
void BaseRoute::stop()
{
    // empty
}


} } // namespace ofx::HTTP
