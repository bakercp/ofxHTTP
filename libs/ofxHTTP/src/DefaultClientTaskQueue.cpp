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


#include "ofx/HTTP/DefaultAsycClient.h"


namespace ofx {
namespace HTTP {


DefaultAsycClient::DefaultAsycClient(std::size_t maxConnections,
                                     Poco::ThreadPool& threadPool):
    _maxConnections(maxConnections),
    _threadPool(threadPool)
{
	ofAddListener(ofEvents().update, this, &DefaultAsycClient::update);
}


DefaultAsycClient::~DefaultAsycClient()
{
	ofRemoveListener(ofEvents().update, this, &DefaultAsycClient::update);
}


void DefaultAsycClient::update(ofEventArgs& args)
{
    Poco::FastMutex::ScopedLock lock(_mutex);

    while (!_queuedRequests.empty() &&
           _activeRequests.size() < _maxConnections &&
           _threadPool.available() > 0)
    {
        try
        {
            _threadPool.start(*_queuedRequests.front());
            _activeRequests.push_back(_queuedRequests.front());
            _queuedRequests.pop_front();
        }
        catch (const Poco::NoThreadAvailableException& exc)
        {
            // No threads left.
            break;
        }
    }

    TaskQueue::iterator iter = _activeRequests.begin();

    while (iter != _activeRequests.end())
    {
        if ((*iter)->isThreadFinished())
        {
            iter = _activeRequests.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}


void DefaultAsycClient::exit(ofEventArgs& args)
{
}



Poco::UUID DefaultAsycClient::get(const std::string& uri,
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


Poco::UUID DefaultAsycClient::post(const std::string& uri,
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


Poco::UUID DefaultAsycClient::request(BaseRequest* pRequest,
                                      ThreadSettings threadSettings)
{


    DefaultAsycClientTask::SharedPtr task = DefaultAsycClientTask::makeShared(pRequest,
                                                                              createDefaultResponse(),
                                                                              createDefaultContext());

    task->registerClientEvents(this);
    task->registerClientFilterEvents(this);
    task->registerClientProgressEvents(this);

    
    // add, signal
    enqueTask(task);

    return pRequest->getRequestId();
}


std::size_t DefaultAsycClient::getNumQueuedRequests() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _queuedRequests.size();
}


std::size_t DefaultAsycClient::getNumActiveRequests() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _activeRequests.size();
}


bool DefaultAsycClient::cancel(const Poco::UUID& uuid)
{
    Poco::FastMutex::ScopedLock lock(_mutex);

    TaskQueue::iterator iter = _queuedRequests.begin();

    while (iter != _queuedRequests.end())
    {
        if (uuid == (*iter)->getRequestId())
        {
            _queuedRequests.erase(iter);
            return true;
        }

        ++iter;
    }

    return false;
}


Context* DefaultAsycClient::createDefaultContext()
{
    // TODO, attach context info here.
    return new Context();
}


BaseResponse* DefaultAsycClient::createDefaultResponse()
{
    return new BaseResponse();
}


void DefaultAsycClient::processTaskQueue()
{
    // NOOP
}


void DefaultAsycClient::enqueTask(DefaultAsycClientTask::SharedPtr task)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _queuedRequests.push_back(task);
}


bool DefaultAsycClient::onHTTPClientResponseEvent(ClientResponseEventArgs& args)
{
//    std::cout << "ofApp::onHTTPClientResponseEvent" << std::endl;
//    Poco::StreamCopier::copyStream(args.getResponseStream(), std::cout);
//    std::cout << std::endl;

    return true;
}


bool DefaultAsycClient::onHTTPClientErrorEvent(ClientErrorEventArgs& args)
{
    

//    std::cout << "ofApp::onHTTPClientErrorEvent:" << std::endl;
//    std::cout << args.getRequest().getRequestId().toString() << endl;
//    std::cout << args.getException().displayText() << std::endl;
    return true;
}


bool DefaultAsycClient::onHTTPClientRequestProgress(ClientRequestProgressArgs& args)
{
//    std::cout << "DefaultAsycClient::onHTTPClientRequestProgress:" << std::endl;
    return true;
}


bool DefaultAsycClient::onHTTPClientResponseProgress(ClientResponseProgressArgs& args)
{
//    std::cout << "DefaultAsycClient::onHTTPClientResponseProgress:" << std::endl;
    return true;
}


bool DefaultAsycClient::onHTTPClientRequestFilterEvent(MutableClientRequestArgs& args)
{
//    std::cout << "DefaultAsycClient::onHTTPClientRequestFilterEvent:" << std::endl;
    return true;
}


bool DefaultAsycClient::onHTTPClientResponseFilterEvent(MutableClientResponseArgs& args)
{
//    std::cout << "DefaultAsycClient::onHTTPClientResponseFilterEvent:" << std::endl;
    return true;
}


} } // namespace ofx::HTTP
