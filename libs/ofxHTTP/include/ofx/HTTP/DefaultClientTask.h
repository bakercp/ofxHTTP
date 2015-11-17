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


#pragma once


#include "Poco/Task.h"
#include "Poco/TaskNotification.h"
#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/DefaultClient.h"
#include "ofx/HTTP/ThreadSettings.h"


namespace ofx {
namespace HTTP {


class DefaultClientTask: public DefaultClient, public Poco::Task
{
public:
    DefaultClientTask(BaseRequest* request,
                      BaseResponse* response,
                      Context* context);

    virtual ~DefaultClientTask();

    void runTask();

    bool onHTTPClientResponseEvent(ClientResponseEventArgs& args);
    bool onHTTPClientErrorEvent(ClientErrorEventArgs& args);

    bool onHTTPClientRequestProgress(ClientRequestProgressArgs& args);
    bool onHTTPClientResponseProgress(ClientResponseProgressArgs& args);

    bool onHTTPClientRequestFilterEvent(MutableClientRequestArgs& args);
    bool onHTTPClientResponseFilterEvent(MutableClientResponseArgs& args);

protected:
    virtual void handleBufferEvent(const ClientResponseBufferEventArgs& buffer);

private:
    BaseRequest* _request;
    BaseResponse* _response;
    Context* _context;

};


} } // namespace ofx::HTTP
