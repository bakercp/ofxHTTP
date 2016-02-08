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


#pragma once


#include "Poco/Net/MediaType.h"
#include "Poco/Net/NameValueCollection.h"
#include "ofEvents.h"
#include "ofUtils.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofx/HTTP/ServerEvents.h"


namespace ofx {
namespace HTTP {


/// \brief A base class for a POST event data callback.
class BasePostEventArgs: public ServerEventArgs
{
public:
    /// \Create a BasePostEventArgs.
    /// \param e The server event data associated with this post.
    /// \param postId The post id associated with this post data.
    BasePostEventArgs(ServerEventArgs& e,
                      const std::string& postId):
        ServerEventArgs(e),
        _postId(postId)
    {
    }

    /// \brief Destroy the BasePostEventArgs.
    virtual ~BasePostEventArgs()
    {
    }

    /// \brief The post id.
    ///
    /// Each time a POST request, is processed it is assigned a unique id.
    /// This id allows us to track post progress updates and multi-part posts.
    ///
    /// \returns the session id or null UUID if not set.
    const std::string& getPostId() const
    {
        return _postId;
    }

protected:
    /// \brief The post id.
    std::string _postId;

};


/// \brief A callback for raw HTTP POST forms.
/// \note If either the response or headers are non-empty, a response with the
/// contents of the buffer (even if empty) will be returned along with the
/// headers.
class PostEventArgs: public BasePostEventArgs
{
public:
    PostEventArgs(ServerEventArgs& evt,
                  const std::string& postId,
                  const IO::ByteBuffer& data):
        BasePostEventArgs(evt, postId),
        _data(data)
    {
    }

    /// \brief Destroy the PostEventArgs.
    virtual ~PostEventArgs()
    {
    }

    /// \brief Get the raw POST form data.
    /// \returns the raw POST form data.
    const IO::ByteBuffer& getBuffer() const
    {
        return _data;
    }

protected:
    /// \brief The raw form data sent with the POST.
    const IO::ByteBuffer& _data;

};


/// \brief Event data for POST form events.
class PostFormEventArgs: public BasePostEventArgs
{
public:
    PostFormEventArgs(ServerEventArgs& evt,
                      const std::string& postId,
                      const Poco::Net::NameValueCollection& form):
        BasePostEventArgs(evt, postId),
        _form(form)
    {
    }

    virtual ~PostFormEventArgs()
    {
    }

    /// \returns the form data.
    const Poco::Net::NameValueCollection& getForm() const
    {
        return _form;
    }

protected:
    /// \brief The form data.
    const Poco::Net::NameValueCollection _form;

};


class PostUploadEventArgs: public BasePostEventArgs
{
public:
    enum UploadState
    {
        UPLOAD_STARTING,
        UPLOAD_PROGRESS,
        UPLOAD_FINISHED
    };

    PostUploadEventArgs(ServerEventArgs& evt,
                        const std::string& postId,
                        const std::string& formFieldName,
                        const std::string& originalFilename,
                        const std::string& filename,
                        const Poco::Net::MediaType& contentType,
                        uint64_t numBytesTransferred,
                        UploadState state):
        BasePostEventArgs(evt, postId),
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

    
    uint64_t getNumBytesTransferred() const
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
    uint64_t _numBytesTransferred;
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
