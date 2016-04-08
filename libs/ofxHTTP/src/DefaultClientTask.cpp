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
    HTTPClient(),
    Poco::Task(request->getURI()),
    _request(request),
    _response(response),
    _context(context)
{
}


DefaultClientTask::~DefaultClientTask()
{
    delete _request;
    delete _response;
    delete _context;
}


void DefaultClientTask::runTask()
{
    std::vector<ofEventListener> listeners;
    listeners.emplace_back(events.onHTTPClientErrorEvent.newListener(this, &DefaultClientTask::onHTTPClientErrorEvent));
    listeners.emplace_back(events.onHTTPClientResponseEvent.newListener(this, &DefaultClientTask::onHTTPClientResponseEvent));
    listeners.emplace_back(events.onHTTPClientRequestProgress.newListener(this, &DefaultClientTask::onHTTPClientRequestProgress));
    listeners.emplace_back(events.onHTTPClientResponseProgress.newListener(this, &DefaultClientTask::onHTTPClientResponseProgress));
    listeners.emplace_back(events.onHTTPClientRequestFilterEvent.newListener(this, &DefaultClientTask::onHTTPClientRequestFilterEvent));
    listeners.emplace_back(events.onHTTPClientResponseFilterEvent.newListener(this, &DefaultClientTask::onHTTPClientResponseFilterEvent));

    submit(*_request, *_response, *_context);

    // Listeners are removed when the go out of scope.
}


bool DefaultClientTask::onHTTPClientResponseEvent(HTTP::ClientResponseEventArgs& args)
{
    const std::size_t bufferSize = IO::ByteBufferUtils::DEFAULT_BUFFER_SIZE;

    std::istream& istr = args.responseStream();

    std::streamsize contentLength = args.response().getContentLength();

    IO::ByteBuffer _byteBuffer;

    if (contentLength > 0)
    {
        _byteBuffer.reserve(contentLength);
    }

    Poco::Buffer<char> buffer(bufferSize);
    std::streamsize len = 0;
	istr.read(buffer.begin(), bufferSize);
    std::streamsize n = istr.gcount();
    
    while (n > 0)
	{
		len += n;
        _byteBuffer.writeBytes(reinterpret_cast<uint8_t*>(buffer.begin()), n);

        // Check for task cancellation.
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

    // Don't return cancelled data.
    if (!isCancelled())
    {
        ClientResponseBufferEventArgs bufferEvent(_byteBuffer,
                                                  args.request(),
                                                  args.response(),
                                                  args.context());
        handleBufferEvent(bufferEvent);
    }

    return true;
}


void DefaultClientTask::handleBufferEvent(const ClientResponseBufferEventArgs& bufferEvent)
{
    postNotification(new Poco::TaskCustomNotification<HTTP::ClientResponseBufferEventArgs>(this, bufferEvent));
}


bool DefaultClientTask::onHTTPClientErrorEvent(HTTP::ClientErrorEventArgs& args)
{
    // We throw an exception to be caught by the task manager.
    throw args.exception();
    return true;
}


bool DefaultClientTask::onHTTPClientRequestProgress(HTTP::ClientRequestProgressArgs& args)
{
    if (args.progress() == HTTP::ClientRequestProgressArgs::UNKNOWN_CONTENT_LENGTH)
    {
        setProgress(0);
    }
    else
    {
        setProgress(args.progress());
    }

    return true;
}


bool DefaultClientTask::onHTTPClientResponseProgress(HTTP::ClientResponseProgressArgs& args)
{
    if (args.progress() == HTTP::ClientRequestProgressArgs::UNKNOWN_CONTENT_LENGTH)
    {
        setProgress(0);
    }
    else
    {
        setProgress(args.progress());
    }

    return true;
}


bool DefaultClientTask::onHTTPClientRequestFilterEvent(HTTP::MutableClientRequestArgs&)
{
    return true;
}


bool DefaultClientTask::onHTTPClientResponseFilterEvent(HTTP::MutableClientResponseArgs&)
{
    return true;
}


} } // namespace ofx::HTTP
