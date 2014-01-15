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


#include "ofx/HTTP/Server/PostRouteEvents.h"
#include "ofx/HTTP/Server/PostRouteHandler.h"
#include "ofx/HTTP/Server/PostRouteInterface.h"
#include "ofx/HTTP/Server/PostRouteSettings.h"


namespace ofx {
namespace HTTP {


class PostRoute: public PostRouteInterface
{
public:
    typedef std::shared_ptr<PostRoute>    SharedPtr;
    typedef std::weak_ptr<PostRoute>      WeakPtr;
    typedef PostRouteSettings             Settings;

    PostRoute(const Settings& settings = Settings());
    virtual ~PostRoute();

    std::string getRoutePathPattern() const;

    bool canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                          bool isSecurePort) const;

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    virtual PostRouteEvents& getEventsRef();

    Settings getSettings() const;

    PostRouteEvents events;

    template<class ListenerClass>
    void registerFileUploadEvents(ListenerClass* listener);

    template<class ListenerClass>
    void unregisterFileUploadEvents(ListenerClass* listener);

    static SharedPtr makeShared(const Settings& settings = Settings())
    {
        return SharedPtr(new PostRoute(settings));
    }

    static const Poco::Net::MediaType POST_CONTENT_TYPE_MULTIPART;
    static const Poco::Net::MediaType POST_CONTENT_TYPE_TEXT_PLAIN;
    static const Poco::Net::MediaType POST_CONTENT_TYPE_URLENCODED;

protected:
    Settings _settings;

};


template<class ListenerClass>
void PostRoute::registerFileUploadEvents(ListenerClass* listener)
{
    ofAddListener(events.onPost, listener, &ListenerClass::onPost);
}

template<class ListenerClass>
void PostRoute::unregisterFileUploadEvents(ListenerClass* listener)
{
    ofRemoveListener(events.onPost, listener, &ListenerClass::onPost);
}


} } // namespace ofx::HTTP
