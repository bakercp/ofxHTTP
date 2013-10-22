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


#include "ofx/HTTP/Server/FileUploadRouteEvents.h"
#include "ofx/HTTP/Server/FileUploadRouteHandler.h"
#include "ofx/HTTP/Server/FileUploadRouteInterface.h"
#include "ofx/HTTP/Server/FileUploadRouteSettings.h"


namespace ofx {
namespace HTTP {


class FileUploadRoute: public FileUploadRouteInterface
{
public:
    typedef std::shared_ptr<FileUploadRoute>    SharedPtr;
    typedef std::weak_ptr<FileUploadRoute>      WeakPtr;
    typedef FileUploadRouteSettings             Settings;

    FileUploadRoute(const Settings& settings = Settings());
    virtual ~FileUploadRoute();

    std::string getRoutePathPattern() const;

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

    virtual FileUploadRouteEvents& getEventsRef();

    Settings getSettings() const;

    FileUploadRouteEvents events;

    template<class ListenerClass>
    void registerFileUploadEvents(ListenerClass* listener);

    template<class ListenerClass>
    void unregisterFileUploadEvents(ListenerClass* listener);

    static SharedPtr makeShared(const Settings& settings = Settings())
    {
        return SharedPtr(new FileUploadRoute(settings));
    }

protected:
    Settings _settings;

};


template<class ListenerClass>
void FileUploadRoute::registerFileUploadEvents(ListenerClass* listener)
{
    ofAddListener(events.onUploadStarted, listener, &ListenerClass::onFileUploadStarted);
    ofAddListener(events.onUploadProgress,listener, &ListenerClass::onFileUploadProgress);
    ofAddListener(events.onUploadFinished, listener, &ListenerClass::onFileUploadFinished);
}

template<class ListenerClass>
void FileUploadRoute::unregisterFileUploadEvents(ListenerClass* listener)
{
    ofRemoveListener(events.onUploadStarted, listener, &ListenerClass::onFileUploadStarted);
    ofRemoveListener(events.onUploadProgress, listener, &ListenerClass::onFileUploadProgress);
    ofRemoveListener(events.onUploadFinished, listener, &ListenerClass::onFileUploadFinished);
}


} } // namespace ofx::HTTP
