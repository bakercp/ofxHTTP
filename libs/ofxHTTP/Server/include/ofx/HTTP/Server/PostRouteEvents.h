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


#include "Poco/UUID.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofUtils.h"
#include "ofx/HTTP/Server/ServerEvents.h"


namespace ofx {
namespace HTTP {


class BasePostRequestEventArgs: public BaseHTTPServerRequestEventArgs
{
public:
    BasePostRequestEventArgs(const Poco::UUID& sessionId,
                             const Poco::Net::HTTPServerRequest& request,
                             const Poco::UUID& formId):
        BaseHTTPServerRequestEventArgs(sessionId, request),
        _formId(formId)
    {
    }

    virtual ~BasePostRequestEventArgs()
    {
    }

    /// \brief The post id.
    ///
    /// Each time a POST request, is processed it is assigned a unique id.
    /// This id allows us to track post progress updates and multi-part posts.
    ///
    /// \returns the session id or Poco::UUID::null if not set.
    const Poco::UUID& getPostId() const
    {
        return _formId;
    }

protected:
    Poco::UUID _formId;

};


class BasePostRequestResponseEventArgs: public BaseHTTPServerRequestResponseEventArgs
{
public:
    BasePostRequestResponseEventArgs(const Poco::UUID& sessionId,
                                     const Poco::Net::HTTPServerRequest& request,
                                     Poco::Net::HTTPServerResponse& response,
                                     const Poco::UUID& postId):
        BaseHTTPServerRequestResponseEventArgs(sessionId, request, response),
        _postId(postId)
    {
    }


    virtual ~BasePostRequestResponseEventArgs()
    {
    }


    /// \brief The post id.
    ///
    /// Each time a POST request, is processed it is assigned a unique id.
    /// This id allows us to track post progress updates and multi-part posts.
    ///
    /// \returns the session id or Poco::UUID::null if not set.
    const Poco::UUID& getPostId() const
    {
        return _postId;
    }

protected:
    Poco::UUID _postId;

};


/// \brief A callback for raw HTTP POST forms.
/// \note If either the response or headers are non-empty, a response with the
/// contents of the buffer (even if empty) will be returned along with the
/// headers.
class PostEventArgs: public BasePostRequestResponseEventArgs
{
public:
    PostEventArgs(const Poco::UUID& sessionId,
                  const Poco::Net::HTTPServerRequest& request,
                  Poco::Net::HTTPServerResponse& response,
                  const Poco::UUID& postId,
                  const ofBuffer& data):
        BasePostRequestResponseEventArgs(sessionId,
                                         request,
                                         response,
                                         postId),
        _data(data)
    {
    }

    virtual ~PostEventArgs()
    {
    }


    const ofBuffer& getBuffer() const
    {
        return _data;
    }

protected:
    const ofBuffer& _data;
        ///< \brief The raw form data sent with the POST.

};


class PostFormEventArgs: public BasePostRequestResponseEventArgs
{
public:
    PostFormEventArgs(const Poco::UUID& sessionId,
                      const Poco::Net::HTTPServerRequest& request,
                      Poco::Net::HTTPServerResponse& response,
                      const Poco::UUID& postId,
                      const Poco::Net::NameValueCollection& form):
        BasePostRequestResponseEventArgs(sessionId,
                                         request,
                                         response,
                                         postId),
        _form(form)
    {
    }

    virtual ~PostFormEventArgs()
    {
    }


    const Poco::Net::NameValueCollection& getForm() const
    {
        return _form;
    }


protected:
    const Poco::Net::NameValueCollection _form;

};


class PostUploadEventArgs: public BasePostRequestEventArgs
{
public:
    enum UploadState
    {
        UPLOAD_STARTING,
        UPLOAD_PROGRESS,
        UPLOAD_FINISHED
    };

    PostUploadEventArgs(const Poco::UUID& sessionId,
                        const Poco::Net::HTTPServerRequest& request,
                        const Poco::UUID& formId,
                        const std::string& formFieldName,
                        const std::string& originalFilename,
                        const std::string& filename,
                        const Poco::Net::MediaType& contentType,
                        std::streamsize numBytesTransferred,
                        UploadState state):
        BasePostRequestEventArgs(sessionId, request, formId),
        _formFieldName(formFieldName),
        _originalFilename(originalFilename),
        _filename(filename),
        _contentType(contentType),
        _numBytesTransferred(numBytesTransferred),
        _state(state)
    {
    }


    virtual ~PostUploadEventArgs()
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

    UploadState getState() const
    {
        return _state;
    }

private:
    const std::string _formFieldName;
    const std::string _originalFilename;
    const std::string _filename;
    const Poco::Net::MediaType _contentType;
    std::streamsize _numBytesTransferred;
    UploadState _state;

};


class PostRouteEvents
{
public:
    ofEvent<PostEventArgs> onHTTPPostEvent;
    ofEvent<PostFormEventArgs> onHTTPFormEvent;
    ofEvent<PostUploadEventArgs> onHTTPUploadEvent;

};


} } // namespace ofx::HTTP
