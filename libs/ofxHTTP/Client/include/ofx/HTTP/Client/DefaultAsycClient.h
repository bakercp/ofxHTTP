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


#include <deque>
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "ofx/HTTP/Types/ThreadSettings.h"
#include "ofx/HTTP/Client/DefaultClient.h"
#include "ofx/HTTP/Client/DefaultAsycClientTask.h"
#include "ofx/HTTP/Client/ClientEvents.h"
#include "ofx/HTTP/Client/BaseRequest.h"
#include "ofx/HTTP/Client/GetRequest.h"
#include "ofx/HTTP/Client/PostRequest.h"


namespace ofx {
namespace HTTP {
namespace Client {


class DefaultAsycClient
{
public:
    DefaultAsycClient(std::size_t maxConnections = DEFAULT_MAXIMUM_CONNECTIONS,
                      Poco::ThreadPool& threadPool = Poco::ThreadPool::defaultPool());

    virtual ~DefaultAsycClient();

    void update(ofEventArgs& args);
    void exit(ofEventArgs& args);

    Poco::UUID get(const std::string& uri,
                   const Poco::Net::NameValueCollection& formFields = Poco::Net::NameValueCollection(),
                   const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1,
                   const Poco::UUID& requestId = BaseRequest::generateUUID(),
                   ThreadSettings threadSettings = ThreadSettings());

    /// \brief Construct a PostRequest with a given uri and http version.
    /// \param uri the Post endpoint uri.
    /// \param formFields A collection of form fields.
    /// \param formParts A collection of form parts.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    /// \param requestId A unique UUID for this request.
    /// \returns a UUID associated with the BaseRequest.
    Poco::UUID post(const std::string& uri,
                    const Poco::Net::NameValueCollection formFields = Poco::Net::NameValueCollection(),
                    const PostRequest::FormParts formParts = PostRequest::FormParts(),
                    const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1,
                    const Poco::UUID& requestId = BaseRequest::generateUUID(),
                    ThreadSettings threadSettings = ThreadSettings());

    /// \brief Submit a request.
    ///
    /// The DefaultClientPool will take ownership of the pointer and destroy
    /// it after use.  Users must not attempt to modify the request after it
    /// has been submitted.
    ///
    /// \returns a UUID associated with the BaseRequest.
    Poco::UUID request(BaseRequest* pRequest,
                       ThreadSettings threadSettings = ThreadSettings());


    /// \brief Get the number of queued requests.
    /// \returns the number of queued requests.
    std::size_t getNumQueuedRequests() const;

    /// \brief Get the number of active requests.
    /// \returns the number of active requests.
    std::size_t getNumActiveRequests() const;

    /// \brief Cancel an existing task.
    /// \param uuid The UUID returned with the initial task.
    bool cancel(const Poco::UUID& uuid);

    /// \brief Users subscribe to these events.
    // ClientEvents events;

    enum
    {
        DEFAULT_MAXIMUM_CONNECTIONS = 4
    };

    virtual bool onHTTPClientResponseEvent(ClientResponseEventArgs& args);
    virtual bool onHTTPClientErrorEvent(ClientErrorEventArgs& args);

    virtual bool onHTTPClientRequestProgress(ClientRequestProgressArgs& args);
    virtual bool onHTTPClientResponseProgress(ClientResponseProgressArgs& args);

    virtual bool onHTTPClientRequestFilterEvent(MutableClientRequestArgs& args);
    virtual bool onHTTPClientResponseFilterEvent(MutableClientResponseArgs& args);

private:
    DefaultAsycClient(const DefaultAsycClient&);
    DefaultAsycClient& operator = (const DefaultAsycClient&);

    Context* createDefaultContext();
    BaseResponse* createDefaultResponse();

    /// \brief Subclasses can implement sorting, pruning etc.
    ///
    /// This is called before threads are executed.
    virtual void processTaskQueue();

    void enqueTask(DefaultAsycClientTask::SharedPtr task);

    typedef std::deque<DefaultAsycClientTask::SharedPtr> TaskQueue;

    TaskQueue _queuedRequests;
    TaskQueue _activeRequests;

    std::size_t _maxConnections;

    Poco::ThreadPool& _threadPool;

    mutable Poco::FastMutex _mutex;
};


} } } // namespace ofx::HTTP::Client
