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


#include "ofx/HTTP/DefaultClientTaskQueue.h"


namespace ofx {
namespace HTTP {


DefaultClientTaskQueue::DefaultClientTaskQueue(int maxTasks,
                                               Poco::ThreadPool& threadPool):
    TaskQueue_<ClientResponseEventArgs>(maxTasks, threadPool)
{
}

DefaultClientTaskQueue::~DefaultClientTaskQueue()
{
}


Poco::UUID DefaultClientTaskQueue::get(const std::string& uri,
                                       const Poco::Net::NameValueCollection& formFields,
                                       const std::string& httpVersion,
                                       const Poco::UUID& requestId,
                                       ThreadSettings threadSettings)
{

    GetRequest* req = new GetRequest(uri,
                                     formFields,
                                     httpVersion,
                                     requestId);

    return request(req,
                   threadSettings);
}


Poco::UUID DefaultClientTaskQueue::post(const std::string& uri,
                                        const Poco::Net::NameValueCollection formFields,
                                        const PostRequest::FormParts formParts,
                                        const std::string& httpVersion,
                                        const Poco::UUID& requestId,
                                        ThreadSettings threadSettings)
{
    return request(new PostRequest(uri,
                                   formFields,
                                   formParts,
                                   httpVersion,
                                   requestId),
                                   threadSettings);
}


Poco::UUID DefaultClientTaskQueue::request(BaseRequest* pRequest,
                                           ThreadSettings threadSettings)
{

    DefaultClientTask* task = new DefaultClientTask(pRequest,
                                                   createDefaultResponse(),
                                                   createDefaultContext());

    return start(task);
}


Context* DefaultClientTaskQueue::createDefaultContext()
{
    // TODO, attach context info here.
    return new Context();
}


BaseResponse* DefaultClientTaskQueue::createDefaultResponse()
{
    return new BaseResponse();
}


} } // namespace ofx::HTTP
