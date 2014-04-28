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


#include "ofx/HTTP/Client/BaseClientProcessors.h"


namespace ofx {
namespace HTTP {


BaseClientProcessors::BaseClientProcessors()
{
}


BaseClientProcessors::~BaseClientProcessors()
{
}

    
void BaseClientProcessors::processRequest(Client::BaseRequest& request,
                                          Context& context)
{
    RequestProcessors::iterator iter = _requestProcessors.begin();

    while (iter != _requestProcessors.end())
    {
        (*iter)->processRequest(request, context);
        ++iter;
    }
}


void BaseClientProcessors::processResponse(Client::BaseRequest& request,
                                           Client::BaseResponse& response,
                                           Context& context)
{
    ResponseHandlers::iterator iter = _responseHandlers.begin();

    while (iter != _responseHandlers.end())
    {
        (*iter)->handleResponse(request, response, context);
        ++iter;
    }
}


void BaseClientProcessors::addRequestProcessor(AbstractRequestProcessor* processor)
{
    _requestProcessors.push_back(processor);
}


void BaseClientProcessors::addResponseHandler(AbstractResponseHandler* handler)
{
    _responseHandlers.push_back(handler);
}


void BaseClientProcessors::removeRequestProcessor(AbstractRequestProcessor* processor)
{
    RequestProcessors::iterator iter = std::find(_requestProcessors.begin(),
                                                 _requestProcessors.end(),
                                                 processor);

    if (iter != _requestProcessors.end())
    {
        _requestProcessors.erase(iter);
    }
}


void BaseClientProcessors::removeResponseHandler(AbstractResponseHandler* handler)
{
    ResponseHandlers::iterator iter = std::find(_responseHandlers.begin(),
                                                _responseHandlers.end(),
                                                handler);

    if (iter != _responseHandlers.end())
    {
        _responseHandlers.erase(iter);
    }
}

    
} } // namespace ofx::HTTP
