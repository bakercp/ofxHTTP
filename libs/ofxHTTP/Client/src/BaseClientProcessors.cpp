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

    
void BaseClientProcessors::processRequest(Poco::Net::HTTPRequest& request,
                                          Context& context)
{
    RequestProcessors::iterator iter = _requestProcessors.begin();

    while (iter != _requestProcessors.end())
    {
        (*iter)->processRequest(request, context);
        ++iter;
    }
}


void BaseClientProcessors::processResponse(Poco::Net::HTTPRequest& request,
                                           Poco::Net::HTTPResponse& response,
                                           Context& context)
{
    ResponseProcessor::iterator iter = _responseProcessors.begin();

    while (iter != _responseProcessors.end())
    {
        (*iter)->processResponse(request, response, context);
        ++iter;
    }
}


void BaseClientProcessors::addRequestProcessor(AbstractRequestProcessor* processor)
{
    _requestProcessors.push_back(processor);
}


void BaseClientProcessors::addResponseProcessor(AbstractResponseProcessor* processor)
{
    _responseProcessors.push_back(processor);
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


void BaseClientProcessors::removeResponseProcessor(AbstractResponseProcessor* processor)
{
    ResponseProcessor::iterator iter = std::find(_responseProcessors.begin(),
                                                 _responseProcessors.end(),
                                                 processor);

    if (iter != _responseProcessors.end())
    {
        _responseProcessors.erase(iter);
    }
}

    
} } // namespace ofx::HTTP
