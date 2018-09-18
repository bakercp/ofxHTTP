//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/PostRouteEvents.h"


namespace ofx {
namespace HTTP {


    
BasePostEventArgs::BasePostEventArgs(ServerEventArgs& evt,
                                     const std::string& postId):
    ServerEventArgs(evt),
    _postId(postId)
{
}
    

BasePostEventArgs::~BasePostEventArgs()
{
}

    
std::string BasePostEventArgs::postId() const
{
    return _postId;
}
    

std::string BasePostEventArgs::getPostId() const
{
    return _postId;
}

    
PostEventArgs::PostEventArgs(ServerEventArgs& evt,
                             const std::string& postId,
                             const IO::ByteBuffer& data):
    BasePostEventArgs(evt, postId),
    _data(data)
{
}
        

PostEventArgs::~PostEventArgs()
{
}
    
    
const IO::ByteBuffer& PostEventArgs::buffer() const
{
    return _data;
}

    
const IO::ByteBuffer& PostEventArgs::getBuffer() const
{
    return _data;
}
    

PostFormEventArgs::PostFormEventArgs(ServerEventArgs& evt,
                                     const std::string& postId,
                                     const Poco::Net::NameValueCollection& form):
    BasePostEventArgs(evt, postId),
    _form(form)
{
}
    
    
PostFormEventArgs::~PostFormEventArgs()
{
}

    
    
const Poco::Net::NameValueCollection& PostFormEventArgs::form() const
{
    return _form;
}


const Poco::Net::NameValueCollection& PostFormEventArgs::getForm() const
{
    return _form;
}

    
PostUploadEventArgs::PostUploadEventArgs(ServerEventArgs& evt,
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
        
        
PostUploadEventArgs::~PostUploadEventArgs()
{
}
        

const std::string& PostUploadEventArgs::formFieldName() const
{
    return _formFieldName;
}


const std::string& PostUploadEventArgs::getFormFieldName() const
{
    return _formFieldName;
}


const std::string& PostUploadEventArgs::originalFilename() const
{
    return _originalFilename;
}

    
const std::string& PostUploadEventArgs::getOriginalFilename() const
{
    return _originalFilename;
}

    
const std::string& PostUploadEventArgs::filename() const
{
    return _filename;
}

    
const std::string& PostUploadEventArgs::getFilename() const
{
    return _filename;
}

    
const Poco::Net::MediaType& PostUploadEventArgs::fileType() const
{
    return _contentType;
}


const Poco::Net::MediaType& PostUploadEventArgs::getFileType() const
{
    return _contentType;
}


uint64_t PostUploadEventArgs::numBytesTransferred() const
{
    return _numBytesTransferred;
}

    
uint64_t PostUploadEventArgs::getNumBytesTransferred() const
{
    return _numBytesTransferred;
}

    
PostUploadEventArgs::UploadState PostUploadEventArgs::state() const
{
    return _state;
}

    
PostUploadEventArgs::UploadState PostUploadEventArgs::getState() const
{
    return _state;
}


} } // namespace ofx::HTTP
