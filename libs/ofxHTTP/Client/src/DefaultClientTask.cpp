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


#include "ofx/HTTP/Client/DefaultClientTask.h"


namespace ofx {
namespace HTTP {
namespace Client {


DefaultClientTask::DefaultClientTask(BaseRequest* request,
                                     BaseResponse* response,
                                     Context* context,
                                     ClientEvents& events,
                                     ThreadSettings threadSettings):
    _request(request),
    _response(response),
    _context(context),
    _events(events),
    _threadSettings(threadSettings)
{
}


DefaultClientTask::~DefaultClientTask()
{
    delete _request;
    delete _response;
    delete _context;
}


void DefaultClientTask::run()
{
    ClientRequestProgressArgs progress(*_request, *_context, 0);
    ofNotifyEvent(_events.onHTTPClientRequestProgress, progress);

    DefaultClient client;
    Context context;

    try
    {
        std::istream& responseStream = client.execute(*_request,
                                                      *_response,
                                                      *_context);

        ClientResponseEventArgs evt(responseStream,
                                    *_request,
                                    *_response,
                                    *_context);

        ofNotifyEvent(_events.onHTTPClientResponseEvent, evt);

        //std::cout << "STATUS: " << response.getStatus() << " : " << response.getReasonForStatus(response.getStatus()) << std::endl;

        //Poco::StreamCopier::copyStream(responseStream, std::cout);
    }
    catch(const Poco::SyntaxException& exc)
    {
        ClientErrorEventArgs evt(*_request, *_response, *_context, exc);
        ofNotifyEvent(_events.onHTTPClientErrorEvent, evt);
    }
    catch(const Poco::Net::HostNotFoundException& exc)
    {
        ClientErrorEventArgs evt(*_request, *_response, *_context, exc);
        ofNotifyEvent(_events.onHTTPClientErrorEvent, evt);
    }
    catch(const Poco::Net::HTTPException& exc)
    {
        ClientErrorEventArgs evt(*_request, *_response, *_context, exc);
        ofNotifyEvent(_events.onHTTPClientErrorEvent, evt);
    }
    catch(const Poco::Exception& exc)
    {
        ClientErrorEventArgs evt(*_request, *_response, *_context, exc);
        ofNotifyEvent(_events.onHTTPClientErrorEvent, evt);
    }
    catch(...)
    {
        Poco::Exception exc("Unknown exception.");
        ClientErrorEventArgs evt(*_request, *_response, *_context, exc);
        ofNotifyEvent(_events.onHTTPClientErrorEvent, evt);
    }



//    ClientRequestProgressArgs progress(*_request, *_context, 0);
//    ofNotifyEvent(_events.onHTTPClientRequestProgress, progress);
}


const Poco::UUID& DefaultClientTask::getRequestId() const
{
    poco_assert(_request);
    
    return _request->getRequestId();
}


const ThreadSettings& DefaultClientTask::getThreadSettings() const
{
    return _threadSettings;
}


} } } // namespace ofx::HTTP::Client
