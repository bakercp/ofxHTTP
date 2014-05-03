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


#include "ofx/HTTP/Client/DefaultClientPool.h"


namespace ofx {
namespace HTTP {
namespace Client {


DefaultClientPool::DefaultClientPool(std::size_t maxConnections,
                                     Poco::ThreadPool& threadPool):
    _maxConnections(maxConnections),
    _threadPool(threadPool)
{
	ofAddListener(ofEvents().update, this, &DefaultClientPool::update);
}


DefaultClientPool::~DefaultClientPool()
{
	ofRemoveListener(ofEvents().update, this, &DefaultClientPool::update);
}


void DefaultClientPool::update(ofEventArgs& args)
{
    Poco::FastMutex::ScopedLock lock(_mutex);

    if (!_queuedRequests.empty() &&
        _activeRequests.size() < _maxConnections &&
        _threadPool.available() > 0)
    {
        try
        {
            _threadPool.startWithPriority( _queuedRequests[0]->getThreadSettings().getPriority(),
                                          *_queuedRequests[0],
                                           _queuedRequests[0]->getThreadSettings().getName());

            _activeRequests.push_back(_queuedRequests[0]);
            _queuedRequests.pop_front();
        }
        catch (const Poco::NoThreadAvailableException& exc)
        {
            // NOOP
        }
    }
}


void DefaultClientPool::exit(ofEventArgs& args)
{

}



Poco::UUID DefaultClientPool::get(const std::string& uri,
                                  const Poco::Net::NameValueCollection& formFields,
                                  const std::string& httpVersion,
                                  const Poco::UUID& requestId,
                                  ThreadSettings threadSettings)
{
    return request(new GetRequest(uri,
                                  formFields,
                                  httpVersion,
                                  requestId),
                   threadSettings);
}


Poco::UUID DefaultClientPool::post(const std::string& uri,
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


Poco::UUID DefaultClientPool::request(BaseRequest* pRequest,
                                      ThreadSettings threadSettings)
{
    DefaultClientTask* task = new DefaultClientTask(pRequest,
                                                    createDefaultResponse(),
                                                    createDefaultContext(),
                                                    events,
                                                    threadSettings);

    // add, signal
    enqueTask(task);

    return pRequest->getRequestId();
}


std::size_t DefaultClientPool::getNumQueuedRequests() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _queuedRequests.size();
}


std::size_t DefaultClientPool::getNumActiveRequests() const
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    return _activeRequests.size();
}


bool DefaultClientPool::cancel(const Poco::UUID& uuid)
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


Context* DefaultClientPool::createDefaultContext()
{
    // TODO, attach context info here.
    return new Context();
}


BaseResponse* DefaultClientPool::createDefaultResponse()
{
    return new BaseResponse();
}


void DefaultClientPool::processTaskQueue()
{
    // NOOP
}


void DefaultClientPool::enqueTask(DefaultClientTask* task)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    _queuedRequests.push_back(task);
}


} } } // namespace ofx::HTTP::Client
