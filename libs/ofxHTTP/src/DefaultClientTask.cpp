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


#include "ofx/HTTP/DefaultClientTask.h"
#include "Poco/TaskManager.h"
#include "Poco/Buffer.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofx/IO/ByteBufferUtils.h"


namespace ofx {
namespace HTTP {


DefaultClientTask::DefaultClientTask(BaseRequest* request,
                                     BaseResponse* response,
                                     Context* context):
    DefaultClient(),
    Poco::Task(request->getURI()),
    _request(request),
    _response(response),
    _context(context)
{
}


DefaultClientTask::~DefaultClientTask()
{
    // ?
    delete _request;
    delete _response;
    delete _context;
}


void DefaultClientTask::runTask()
{
    registerClientEvents(this);
    registerClientProgressEvents(this);
    registerClientFilterEvents(this);

    // TODO: add progress callback.
    submit(*_request, *_response, *_context);

    unregisterClientFilterEvents(this);
    unregisterClientProgressEvents(this);
    unregisterClientEvents(this);
}


bool DefaultClientTask::onHTTPClientResponseEvent(HTTP::ClientResponseEventArgs& args)
{
    const std::size_t bufferSize = IO::ByteBufferUtils::DEFAULT_BUFFER_SIZE;

    IO::ByteBuffer byteBuffer;

    std::istream& istr = args.getResponseStream();

    Poco::Buffer<char> buffer(bufferSize);
    std::streamsize len = 0;
	istr.read(buffer.begin(), bufferSize);
    std::streamsize n = istr.gcount();
    while (n > 0)
	{
		len += n;
        byteBuffer.writeBytes(reinterpret_cast<uint8_t*>(buffer.begin()), n);
        if (istr && !isCancelled())
		{
			istr.read(buffer.begin(), bufferSize);
            n = istr.gcount();
		}
        else
        {
            n = 0;
        }
	}

    ClientResponseBufferEventArgs bufferEvent(byteBuffer,
                                              args.getRequest(),
                                              args.getResponse(),
                                              args.getContextRef());

    postNotification(new Poco::TaskCustomNotification<HTTP::ClientResponseBufferEventArgs>(this, bufferEvent));
    return true;
}


bool DefaultClientTask::onHTTPClientErrorEvent(HTTP::ClientErrorEventArgs& args)
{
    Poco::TaskManager* pOwner = getOwner();
    throw args.getException();
    return true;
}


bool DefaultClientTask::onHTTPClientRequestProgress(HTTP::ClientRequestProgressArgs& args)
{
    setProgress(args.getProgress());
    return true;
}


bool DefaultClientTask::onHTTPClientResponseProgress(HTTP::ClientResponseProgressArgs& args)
{
    setProgress(args.getProgress());
    return true;
}


bool DefaultClientTask::onHTTPClientRequestFilterEvent(HTTP::MutableClientRequestArgs& args)
{
    return true;
}


bool DefaultClientTask::onHTTPClientResponseFilterEvent(HTTP::MutableClientResponseArgs& args)
{
    return true;
}


} } // namespace ofx::HTTP
