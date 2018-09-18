//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
    /// \param evt The server event data associated with this post.
    /// \param postId The post id associated with this post data.
    BasePostEventArgs(ServerEventArgs& evt, const std::string& postId);

    /// \brief Destroy the BasePostEventArgs.
    virtual ~BasePostEventArgs();

    /// \brief The post id.
    ///
    /// Each time a POST request, is processed it is assigned a unique id.
    /// This id allows us to track post progress updates and multi-part posts.
    ///
    /// \returns the session id or null UUID if not set.
    std::string postId() const;

    OF_DEPRECATED_MSG("Use postId().", std::string getPostId() const);

protected:
    /// \brief The post id.
    const std::string _postId;

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
                  const IO::ByteBuffer& data);

    /// \brief Destroy the PostEventArgs.
    virtual ~PostEventArgs();

    /// \brief Get the raw POST form data.
    /// \returns the raw POST form data.
    const IO::ByteBuffer& buffer() const;

    OF_DEPRECATED_MSG("Use buffer().", const IO::ByteBuffer& getBuffer() const);

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
                      const Poco::Net::NameValueCollection& form);

    virtual ~PostFormEventArgs();

    /// \returns the form data.
    const Poco::Net::NameValueCollection& form() const;

    OF_DEPRECATED_MSG("Use form().", const Poco::Net::NameValueCollection& getForm() const);

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
                        UploadState state);


    virtual ~PostUploadEventArgs();
    
    /// \returns the form field name for the upload.
    const std::string& formFieldName() const;
    OF_DEPRECATED_MSG("Use formFieldName().", const std::string& getFormFieldName() const);
    
    /// \returns the original filename specified in the form.
    const std::string& originalFilename() const;
    OF_DEPRECATED_MSG("Use originalFilename().", const std::string& getOriginalFilename() const);

    /// \returns the new filename generated for the upload.
    const std::string& filename() const;
    OF_DEPRECATED_MSG("Use filename().", const std::string& getFilename() const);

    /// \returns the MIME type of the uploaded file.
    const Poco::Net::MediaType& fileType() const;
    OF_DEPRECATED_MSG("Use fileType().", const Poco::Net::MediaType& getFileType() const);

    /// \returns the number of bytes transfered since the upload began.
    uint64_t numBytesTransferred() const;
    OF_DEPRECATED_MSG("Use numBytesTransferred().", uint64_t getNumBytesTransferred() const);

    /// \returns the current upload state.
    UploadState state() const;
    OF_DEPRECATED_MSG("Use state().", UploadState getState() const);

private:
    /// \brief The form field for this upload.
    const std::string _formFieldName;
    
    /// \brief The original filename specified in the upload.
    const std::string _originalFilename;
    
    /// \brief The generated filename where the file is saved after upload.
    const std::string _filename;
    
    /// \brief The MIME type of the file upload.
    const Poco::Net::MediaType _contentType;
    
    /// \brief The number of bytes transferred since the upload began.
    uint64_t _numBytesTransferred;
    
    /// \brief The current state of the upload.
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
