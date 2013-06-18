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


#include "ServerRouteManager.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
ServerRouteManager::ServerRouteManager(RouteFactories& factories,
                                       bool bIsSecurePort) :
    _factories(factories),
    _bIsSecurePort(bIsSecurePort)
{
}
    
//------------------------------------------------------------------------------
ServerRouteManager::~ServerRouteManager()
{
}

//------------------------------------------------------------------------------
HTTPRequestHandler* ServerRouteManager::createRequestHandler(const HTTPServerRequest& request)
{

    // We start with the last factory that was added.
    // Thus, factories with overlapping routes should be
    // carefully ordered.
    
    RouteFactories::reverse_iterator iter = _factories.rbegin();
    while(iter != _factories.rend()) {
        if((*iter)->canHandleRequest(request,_bIsSecurePort)) {
            return (*iter)->createRequestHandler(request);
        }
        ++iter;
    }

    // if we get to this point, we didn't find a matching route
    return new BaseServerRouteHandler(BaseServerRouteSettings());
}


} } // namespace ofx::HTTP
