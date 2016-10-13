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


#include "ofx/HTTP/GetRequestTask.h"
#include "ofx/HTTP/GetRequest.h"
#include "ofx/MediaTypeMap.h"


namespace ofx {
namespace HTTP {


GetRequestTask::GetRequestTask(const std::string& uri, const std::string& cachePath):
    Poco::Task(uri),
    _uri(uri),
    _cachePath(cachePath)
{
}


GetRequestTask::~GetRequestTask()
{
}


void GetRequestTask::runTask()
{
    Client client;
    Context context;
    GetRequest request(_uri);
    auto l0 = context.events.onHTTPClientStateChange.newListener(this, &GetRequestTask::onHTTPClientStateChange);
    auto l1 = context.events.onHTTPClientRequestProgress.newListener(this, &GetRequestTask::onHTTPClientRequestProgress);
    auto l2 = context.events.onHTTPClientResponseProgress.newListener(this, &GetRequestTask::onHTTPClientResponseProgress);
    auto l3 = context.events.onHTTPClientResponseStream.newListener(this, &GetRequestTask::onHTTPClientResponseStream);
    auto l4 = context.events.onHTTPClientError.newListener(this, &GetRequestTask::onHTTPClientError);
    client.submit(context, request);
}


bool GetRequestTask::onHTTPClientStateChange(ClientStateChangeEventArgs& args)
{
    return true;
}


bool GetRequestTask::onHTTPClientRequestProgress(ClientRequestProgressEventArgs& args)
{
    float progress = args.progress().progress();

    if (progress == Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH)
    {
        setProgress(0);
    }
    else
    {
        setProgress(progress);
    }

    return true;
}


bool GetRequestTask::onHTTPClientResponseProgress(ClientResponseProgressEventArgs& args)
{
    float progress = args.progress().progress();

    if (progress == Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH)
    {
        setProgress(0);
    }
    else
    {
        setProgress(progress);
    }

    return true;
}


bool GetRequestTask::onHTTPClientResponseStream(ClientResponseStreamEventArgs& args)
{
    ofBuffer buffer(args.stream());

    if (!_cachePath.empty())
    {
        ofBufferToFile(_cachePath, buffer);
    }

    Poco::Net::MediaType mediaType(args.response().getContentType());

    if (mediaType.matches("image"))
    {
        ofPixels pixels;
        ofLoadImage(pixels, buffer);
        postNotification(new Poco::TaskCustomNotification<ofPixels>(this, pixels));
    }
    else if (mediaType.matches("application", "json"))
    {
        ofJson json = ofJson::parse(buffer);
        postNotification(new Poco::TaskCustomNotification<ofJson>(this, json));
    }
    else
    {
        std::cout << "Unknown type " << mediaType.toString() << " posting buffer.";
        postNotification(new Poco::TaskCustomNotification<ofBuffer>(this, buffer));
    }

    return true;
}


bool GetRequestTask::onHTTPClientError(ClientErrorEventArgs& args)
{
    // We throw an exception to be caught by the task manager.
    throw args.exception();

    // Unreachable.
    return true;
}


} } // namespace ofx::HTTP