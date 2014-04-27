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


#include "ofEvents.h"
#include "ofFileUtils.h"
#include "Poco/Net/HTTPClientSession.h"
#include "ofx/HTTP/Client/BaseRequest.h"


namespace ofx {
namespace HTTP {
namespace Client {


class BaseClientResponseArgs: public ofEventArgs
{
public:
    BaseClientResponseArgs(const BaseRequest& request,
                           const Poco::Net::HTTPResponse& response):
        _request(request),
        _response(response)
    {
    }

    
    virtual ~BaseClientResponseArgs()
    {
    }


    const BaseRequest& getRequest() const
    {
        return _request;
    }


    const Poco::Net::HTTPResponse& getResponse() const
    {
        return _response;
    }


private:
    const BaseRequest& _request;
    const Poco::Net::HTTPResponse& _response;

};


class ClientResponseEventArgs: public BaseClientResponseArgs
{
public:
    ClientResponseEventArgs(const BaseRequest& request,
                            const Poco::Net::HTTPResponse& response,
                            std::istream& responseStream):
        BaseClientResponseArgs(request, response),
        _responseStream(responseStream)
    {
    }


    virtual ~ClientResponseEventArgs()
    {
    }

    bool saveFile(const std::string& filename) const
    {
        ofBuffer buffer(_responseStream);
        return ofBufferToFile(filename, buffer);
    }

    ofBuffer getBuffer() const
    {
        return ofBuffer(_responseStream);
    }

    std::istream& getResponseStream()
    {
        return _responseStream;
    }

protected:
    std::istream& _responseStream;
    
};


class ClientErrorEventArgs: public BaseClientResponseArgs
{
public:
    ClientErrorEventArgs(const BaseRequest& request,
                         const Poco::Net::HTTPResponse& response,
                         const Poco::Exception& exception):
        BaseClientResponseArgs(request, response),
        _exception(exception)
    {
    }


    virtual ~ClientErrorEventArgs()
    {
    }


    const Poco::Exception& getException() const
    {
        return _exception;
    }

protected:
    const Poco::Exception& _exception;

};


class ClientEvents
{
public:
    ofEvent<ClientErrorEventArgs> onHTTPClientErrorEvent;
    ofEvent<ClientResponseEventArgs> onHTTPClientResponseEvent;


    //    ofEvent<PostEventArgs> onHTTPPostEvent;
    //    ofEvent<PostFormEventArgs> onHTTPFormEvent;
    //    ofEvent<PostUploadEventArgs> onHTTPUploadEvent;
    
};


} } } // namespace ofx::HTTP::Client
