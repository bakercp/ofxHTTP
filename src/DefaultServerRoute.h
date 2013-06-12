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


#pragma once


#include "BaseTypes.h"
#include "DefaultServerRouteHandler.h"


namespace ofx {
namespace HTTP {
        

class DefaultServerRoute : public BaseServerRoute
{
public:
    typedef DefaultServerRouteSettings Settings;
    typedef ofPtr<DefaultServerRoute> Ptr;

    DefaultServerRoute(const Settings& settings = Settings()) :
        _settings(settings)
    {
        
        ofDirectory documentRootDirectory(_settings.documentRoot);
        
        if(_settings.bAutoCreateDocumentRoot &&
           !documentRootDirectory.exists()) {
            documentRootDirectory.create();
        }
    }
    
    virtual ~DefaultServerRoute()
    {
    }

    bool canHandleRequest(const HTTPServerRequest& request,
                          bool bIsSecurePort)
    {

        // require HTTP_GET
        if(request.getMethod() != Poco::Net::HTTPRequest::HTTP_GET) {
            return false;
        }

        // require a valid path
        Poco::URI uri;

        try {
            uri = Poco::URI(request.getURI());
        } catch(const Poco::SyntaxException& exc) {
            ofLogError("ServerWebSocketRoute::canHandleRequest") << exc.what();
            return false;
        }
        
        // just get the path
        string path = uri.getPath();
        // make paths absolute

        if(path.empty()) {
            path = "/";
        }
        
        return Poco::RegularExpression(_settings.getRoute()).match(path);
    }

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
    {
        return new DefaultServerRouteHandler(_settings);
    }

    static Ptr Instance(const Settings& settings = Settings()) {
        return Ptr(new DefaultServerRoute(settings));
    }
    
protected:
    Settings _settings;
    
};


} }
