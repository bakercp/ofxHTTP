// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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


#include "Poco/TeeStream.h"
#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/ClientProgressStream.h"
//#include "ofx/HTTP/BaseRequest.h"
//#include "ofx/HTTP/BaseResponse.h"


namespace ofx {
namespace HTTP {


class BaseClient:
    public AbstractRequestStreamListener,
    public AbstractResponseStreamListener
{
public:
    typedef std::vector<AbstractRequestFilter*> RequestFilters;
    typedef std::vector<AbstractResponseFilter*> ResponseFilters;

    BaseClient();

    BaseClient(RequestFilters requestFilters,
               ResponseFilters responseFilters,
               std::streamsize bytesPerProgressUpdate = DEFAULT_BYTES_PER_PROGRESS_UPDATE);

    virtual ~BaseClient();

    // Can throw a lot of different exceptions.
    std::istream& execute(BaseRequest& request,
                          BaseResponse& response,
                          Context& context);

    // Submit requests / responses and interact via registering for ClientEvents.
    void submit(BaseRequest& request,
                BaseResponse& response,
                Context& context);

    void addRequestFilter(AbstractRequestFilter* filter);
    void addResponseFilter(AbstractResponseFilter* filter);

    void removeRequestFilter(AbstractRequestFilter* filter);
    void removeResponseFilter(AbstractResponseFilter* filter);

    void setRequestStreamFilter(AbstractRequestStreamFilter* filter);
    void setResponseStreamFilter(AbstractResponseStreamFilter* filter);

    void removeRequestStreamFilter();
    void removeResponseStreamFilter();

    void setBytesPerProgressUpdate(std::streamsize bytesPerProgressUpdate);
    std::streamsize getBytesPerProgressUpdate() const;

    template <class ListenerClass>
    void registerClientEvents(ListenerClass* listener,
                              int priority = OF_EVENT_ORDER_AFTER_APP);

    template <class ListenerClass>
    void unregisterClientEvents(ListenerClass* listener,
                                int priority = OF_EVENT_ORDER_AFTER_APP);

    template <class ListenerClass>
    void registerClientProgressEvents(ListenerClass* listener,
                                      int priority = OF_EVENT_ORDER_AFTER_APP);

    template <class ListenerClass>
    void unregisterClientProgressEvents(ListenerClass* listener,
                                        int priority = OF_EVENT_ORDER_AFTER_APP);

    template <class ListenerClass>
    void registerClientFilterEvents(ListenerClass* listener,
                                    int priority = OF_EVENT_ORDER_AFTER_APP);

    template <class ListenerClass>
    void unregisterClientFilterEvents(ListenerClass* listener,
                                      int priority = OF_EVENT_ORDER_AFTER_APP);

    /// \brief A collection of client events.
    ClientEvents events;

    enum
    {
        /// \brief The default number of bytes to wait between progress update events.
        DEFAULT_BYTES_PER_PROGRESS_UPDATE = 1024
    };
    

protected:
    /// \brief A list of request filters.
    ///
    /// These filters are automatically applied to the outgoing request.
    RequestFilters _requestFilters;

    /// \brief A list of response filters.
    ///
    /// These filters are automatically applied to the incoming response.
    ResponseFilters _responseFilters;

    /// \brief The number of bytes to wait between progress update events.
    std::streamsize _bytesPerProgressUpdate;

private:
    /// \brief A pointer to the client progress request output stream.
    std::unique_ptr<std::ostream> _pClientProgressRequestStream;

    /// \brief A pointer to the client progress request input stream.
    std::unique_ptr<std::istream> _pClientProgressResponseStream;

    AbstractRequestStreamFilter* _pRequestStreamFilter;
    AbstractResponseStreamFilter* _pResponseStreamFilter;

    void requestFilter(BaseRequest& request, Context& context);

    void responseFilter(BaseRequest& request,
                        BaseResponse& response,
                        Context& context);

    std::ostream& send(BaseRequest& request, Context& context);

    std::istream& receive(BaseRequest& request,
                          BaseResponse& response,
                          Context& context);

    void progress(const BaseRequest& request,
                  Context& context,
                  std::streamsize totalBytesTransferred) override;

    void progress(const BaseRequest& request,
                  const BaseResponse& response,
                  Context& context,
                  std::streamsize totalBytesTransferred) override;

};


template <class ListenerClass>
void BaseClient::registerClientEvents(ListenerClass* listener, int priority)
{
    ofAddListener(events.onHTTPClientErrorEvent, listener, &ListenerClass::onHTTPClientErrorEvent, priority);
    ofAddListener(events.onHTTPClientResponseEvent, listener, &ListenerClass::onHTTPClientResponseEvent, priority);
}


template <class ListenerClass>
void BaseClient::unregisterClientEvents(ListenerClass* listener, int priority)
{
    ofRemoveListener(events.onHTTPClientErrorEvent, listener, &ListenerClass::onHTTPClientErrorEvent, priority);
    ofRemoveListener(events.onHTTPClientResponseEvent, listener, &ListenerClass::onHTTPClientResponseEvent, priority);
}


template <class ListenerClass>
void BaseClient::registerClientProgressEvents(ListenerClass* listener, int priority)
{
    ofAddListener(events.onHTTPClientRequestProgress, listener, &ListenerClass::onHTTPClientRequestProgress, priority);
    ofAddListener(events.onHTTPClientResponseProgress, listener, &ListenerClass::onHTTPClientResponseProgress, priority);
}


template <class ListenerClass>
void BaseClient::unregisterClientProgressEvents(ListenerClass* listener, int priority)
{
    ofRemoveListener(events.onHTTPClientRequestProgress, listener, &ListenerClass::onHTTPClientRequestProgress, priority);
    ofRemoveListener(events.onHTTPClientResponseProgress, listener, &ListenerClass::onHTTPClientResponseProgress, priority);
}


template <class ListenerClass>
void BaseClient::registerClientFilterEvents(ListenerClass* listener, int priority)
{
    ofAddListener(events.onHTTPClientRequestFilterEvent, listener, &ListenerClass::onHTTPClientRequestFilterEvent, priority);
    ofAddListener(events.onHTTPClientResponseFilterEvent, listener, &ListenerClass::onHTTPClientResponseFilterEvent, priority);
}


template <class ListenerClass>
void BaseClient::unregisterClientFilterEvents(ListenerClass* listener, int priority)
{
    ofRemoveListener(events.onHTTPClientRequestFilterEvent, listener, &ListenerClass::onHTTPClientRequestFilterEvent, priority);
    ofRemoveListener(events.onHTTPClientResponseFilterEvent, listener, &ListenerClass::onHTTPClientResponseFilterEvent, priority);
}


} } // namespace ofx::HTTP
