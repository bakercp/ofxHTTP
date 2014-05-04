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


#include "Poco/AtomicCounter.h"
#include "Poco/Runnable.h"
#include "ofx/HTTP/Client/ClientEvents.h"
#include "ofx/HTTP/Client/DefaultClient.h"
#include "ofx/HTTP/Types/ThreadSettings.h"


namespace ofx {
namespace HTTP {
namespace Client {


class DefaultAsycClientTask:
    public DefaultClient,
    public Poco::Runnable
{
public:
    typedef std::shared_ptr<DefaultAsycClientTask> SharedPtr;

    DefaultAsycClientTask(BaseRequest* request,
                          BaseResponse* response,
                          Context* context);

    virtual ~DefaultAsycClientTask();

    virtual void run();

    const Poco::UUID& getRequestId() const;

    static SharedPtr makeShared(BaseRequest* request,
                                BaseResponse* response,
                                Context* context)
    {
        return SharedPtr(new DefaultAsycClientTask(request, response, context));
    }


private:
    BaseRequest* _request;
    BaseResponse* _response;
    Context* _context;

};


} } } // namespace ofx::HTTP::Client
