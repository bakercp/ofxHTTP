// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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
    TaskQueue_<std::string>(maxTasks, threadPool)
{
}

DefaultClientTaskQueue::~DefaultClientTaskQueue()
{
}


std::string DefaultClientTaskQueue::get(const std::string& uri,
                                        const Poco::Net::NameValueCollection& formFields,
                                        const std::string& httpVersion,
                                        const std::string& requestId,
                                        ThreadSettings threadSettings)
{

    GetRequest* req = new GetRequest(uri, httpVersion);

    req->addFormFields(formFields);
    req->setRequestId(requestId);

    return request(req, threadSettings);
}


std::string DefaultClientTaskQueue::post(const std::string& uri,
                                         const Poco::Net::NameValueCollection formFields,
                                         const PostRequest::FormParts formParts,
                                         const std::string& httpVersion,
                                         const std::string& requestId,
                                         ThreadSettings threadSettings)
{
    PostRequest* req = new PostRequest(uri, httpVersion);

    req->addFormFields(formFields);
    req->addFormParts(formParts);
    req->setRequestId(requestId);

    return request(req, threadSettings);
}


std::string DefaultClientTaskQueue::request(BaseRequest* pRequest,
                                            ThreadSettings threadSettings)
{

    DefaultClientTask* task = new DefaultClientTask(pRequest,
                                                    createDefaultResponse(),
                                                    createDefaultContext());



    return start(Poco::UUIDGenerator::defaultGenerator().createOne().toString(), task);
}


void DefaultClientTaskQueue::handleTaskCustomNotification(const std::string& taskId,
                                                          Poco::AutoPtr<Poco::TaskNotification> pNotification)
{
    Poco::AutoPtr<Poco::TaskCustomNotification<ClientResponseBufferEventArgs>> taskCustomNotification = 0;

    if (!(taskCustomNotification = pNotification.cast<Poco::TaskCustomNotification<ClientResponseBufferEventArgs>>()).isNull())
    {
        TaskDataEventArgs_<std::string, ClientResponseBufferEventArgs> args(taskId,
                                                                            pNotification->task()->name(),
                                                                            pNotification->task()->state(),
                                                                            pNotification->task()->progress(),
                                                                            taskCustomNotification->custom());

        ofNotifyEvent(onClientBuffer, args, this);

    }
    else
    {
        TaskCustomNotificationEventArgs_<std::string> args(taskId,
                                                           pNotification->task()->name(),
                                                           pNotification->task()->state(),
                                                           pNotification->task()->progress(),
                                                           pNotification);

        ofNotifyEvent(TaskQueue_<std::string>::onTaskCustomNotification, args, this);

    }
}



Context* DefaultClientTaskQueue::createDefaultContext()
{
    // TODO, attach context info here.

    Context* c = new Context();

//    ofx::HTTP::SessionSettings sessionSettings;
//    sessionSettings.setProxy(ofx::HTTP::ProxySettings("127.0.0.1", 8888));
//
//    c->setSessionSettings(sessionSettings);

    return c;
}


BaseResponse* DefaultClientTaskQueue::createDefaultResponse()
{
    return new BaseResponse();
}


} } // namespace ofx::HTTP
