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


#include "Poco/Net/MediaType.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofUtils.h"
#include "ofx/HTTP/Server/ServerEvents.h"


namespace ofx {
namespace HTTP {


//class MultipartFormDataEventArgs: public BaseServerEvent
//{
//public:
//    FormDataEventArgs(const Headers& requestHeaders):
//        BaseServerEvent(requestHeaders)
//    {
//    }
//
//
//};
//
//
//class MultiPartDataEventArgs: public BaseServerEvent
//{
//public:
//    FormDataEventArgs(const Headers& requestHeaders):
//        BaseServerEvent(requestHeaders)
//    {
//    }
//
//    std::string getName() const
//    {
//        return _name;
//    }
//
//private:
//    Poco::Net::MediaType _mediaType;
//    std::string _name;
//
//};
//
//
//class FileFormDataEventArgs: public FormDataEventArgs
//{
//public:
//    FormDataEventArgs(Poco::Net::MediaType mediaType,
//                      std::string name):
//    _mediaType(mediaType),
//    _name(name)
//    {
//    }
//
//    Poco::Net::MediaType getMediaType() const
//    {
//        return _mediaType;
//    }
//
//    std::string getName() const
//    {
//        return _name;
//    }
//    
//private:
//    Poco::Net::MediaType _mediaType;
//    std::string _name;
//    
//};
//
//
//

class BaseFormEventArgs: public BaseHTTPServerRequestEventArgs
{
public:
    BaseFormEventArgs(const Poco::Net::HTTPServerRequest& request,
                      const std::string& formUUID):
        BaseHTTPServerRequestEventArgs(request),
        formUUID(formUUID)
    {
    }

    const std::string formUUID;

};

class HTTPRawFormEventArgs: public BaseFormEventArgs
{
public:
    HTTPRawFormEventArgs(const Poco::Net::HTTPServerRequest& request,
                         const std::string& formUUID,
                         const std::string& form):
        BaseFormEventArgs(request, formUUID),
        form(form)
    {
    }

    const std::string form;
};


class HTTPFormEventArgs: public BaseFormEventArgs
{
public:
    HTTPFormEventArgs(const Poco::Net::HTTPServerRequest& request,
                      const std::string& formUUID,
                      const Poco::Net::NameValueCollection& form):
        BaseFormEventArgs(request, formUUID),
        form(form)
    {
    }

    const Poco::Net::NameValueCollection form;
};


class HTTPUploadEventArgs: public BaseFormEventArgs
{
public:
    HTTPUploadEventArgs(const Poco::Net::HTTPServerRequest& request,
                        const std::string& formUUID,
                        const std::string& formFieldName,
                        const std::string& originalFilename,
                        const std::string& filename,
                        const Poco::Net::MediaType& contentType,
                        std::streamsize numBytesTransferred):
        BaseFormEventArgs(request, formUUID),
        _formFieldName(formFieldName),
        _originalFilename(originalFilename),
        _filename(filename),
        _contentType(contentType),
        _numBytesTransferred(numBytesTransferred)
    {
    }


    const std::string& getFormFieldName() const
    {
        return _formFieldName;
    }


    const std::string& getOriginalFilename() const
    {
        return _originalFilename;
    }


    const std::string& getFilename() const
    {
        return _filename;
    }


    const Poco::Net::MediaType& getFileType() const
    {
        return _contentType;
    }

    
    std::streamsize getNumBytesTransferred() const
    {
        return _numBytesTransferred;
    }

private:
    const std::string& _formFieldName;
    const std::string& _originalFilename;
    const std::string& _filename;
    const Poco::Net::MediaType& _contentType;
    std::streamsize _numBytesTransferred;

};



class PostRouteEvents
{
public:
    ofEvent<HTTPFormEventArgs> onHTTPFormEvent;
    ofEvent<HTTPRawFormEventArgs> onHTTPRawFormEvent;

    ofEvent<HTTPUploadEventArgs> onHTTPUploadStartedEvent;
    ofEvent<HTTPUploadEventArgs> onHTTPUploadProgressEvent;
    ofEvent<HTTPUploadEventArgs> onHTTPUploadFinishedEvent;

};


} } // namespace ofx::HTTP
