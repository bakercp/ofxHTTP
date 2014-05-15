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


#include "ofx/HTTP/Client/DefaultAsycClientTask.h"


namespace ofx {
namespace HTTP {
namespace Client {


DefaultAsycClientTask::DefaultAsycClientTask(BaseRequest* request,
                                             BaseResponse* response,
                                             Context* context):
    DefaultClient(),
    _request(request),
    _response(response),
    _context(context),
    _threadFinished(false)
{
}


DefaultAsycClientTask::~DefaultAsycClientTask()
{
    delete _request;
    delete _response;
    delete _context;
}


void DefaultAsycClientTask::run()
{
    submit(*_request, *_response, *_context);

    _threadFinished = true;
}

    
bool DefaultAsycClientTask::isThreadFinished() const
{
    return _threadFinished;
}


const Poco::UUID& DefaultAsycClientTask::getRequestId() const
{
    poco_assert(_request);
    return _request->getRequestId();
}


} } } // namespace ofx::HTTP::Client
