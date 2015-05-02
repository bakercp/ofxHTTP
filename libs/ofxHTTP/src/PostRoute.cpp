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


#include "ofx/HTTP/PostRoute.h"
#include "Poco/Buffer.h"
#include "Poco/StreamCopier.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/Net/HTMLForm.h"


namespace ofx {
namespace HTTP {


const std::string PostRouteSettings::DEFAULT_POST_ROUTE    = "/post";
const std::string PostRouteSettings::DEFAULT_POST_FOLDER   = "uploads/";
const std::string PostRouteSettings::DEFAULT_POST_REDIRECT = "uploaded.html";
const std::string PostRouteSettings::DEFAULT_POST_HTTP_METHODS_ARRAY[] = { "POST" };
const BaseRouteSettings::HTTPMethodSet PostRouteSettings::DEFAULT_POST_HTTP_METHODS(INIT_SET_WITH_ARRAY(DEFAULT_POST_HTTP_METHODS_ARRAY));


PostRouteSettings::PostRouteSettings(const std::string& routePathPattern,
                                     bool requireSecurePort):
    BaseRouteSettings(routePathPattern,
                      requireSecurePort,
                      DEFAULT_POST_HTTP_METHODS),
    _uploadFolder(DEFAULT_POST_FOLDER),
    _uploadRedirect(DEFAULT_POST_REDIRECT),
    _writeBufferSize(DEFAULT_POST_BUFFER_SIZE),
    _fieldLimit(DEFAULT_FIELD_LIMIT),
    _maximumFileUploadSize(DEFAULT_MAXIMUM_FILE_UPLOAD_SIZE)
{
}


PostRouteSettings::~PostRouteSettings()
{
}


void PostRouteSettings::setUploadFolder(const std::string& uploadFolder)
{
    _uploadFolder = uploadFolder;
}


const std::string& PostRouteSettings::getUploadFolder() const
{
    return _uploadFolder;
}


void PostRouteSettings::setUploadRedirect(const std::string& uploadRedirect)
{
    _uploadRedirect = uploadRedirect;
}


const std::string& PostRouteSettings::getUploadRedirect() const
{
    return _uploadRedirect;
}


void PostRouteSettings::setWriteBufferSize(std::size_t writeBufferSize)
{
    _writeBufferSize = writeBufferSize;
}


std::size_t PostRouteSettings::getWriteBufferSize() const
{
    return _writeBufferSize;
}


void PostRouteSettings::setFieldLimit(std::size_t fieldLimit)
{
    _fieldLimit = fieldLimit;
}


std::size_t PostRouteSettings::getFieldLimit() const
{
    return _fieldLimit;
}


void PostRouteSettings::setMaximumFileUploadSize(unsigned long long maximumFileUploadSize)
{
    _maximumFileUploadSize = maximumFileUploadSize;
}


unsigned long long PostRouteSettings::getMaximumFileUploadSize() const
{
    return _maximumFileUploadSize;
}


PostRoute::PostRoute(const Settings& settings):
    BaseRoute_<PostRouteSettings>(settings)
{
}


PostRoute::~PostRoute()
{
}


Poco::Net::HTTPRequestHandler* PostRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    return new PostRouteHandler(*this);
}


PostRouteEvents& PostRoute::getEvents()
{
    return events;
}


const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_TEXT_PLAIN = Poco::Net::MediaType("text/plain");
const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_MULTIPART  = Poco::Net::MediaType("multipart/form-data");
const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_URLENCODED = Poco::Net::MediaType("application/x-www-form-urlencoded");
const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_JSON = Poco::Net::MediaType("application/json");


PostRouteHandler::PostRouteHandler(PostRoute& parent):
    _parent(parent)
{
}


PostRouteHandler::~PostRouteHandler()
{
}


void PostRouteHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                     Poco::Net::HTTPServerResponse& response)
{
    _parent.handleRequest(request, response);

    Poco::UUID sessionId = _parent.getSessionId(request, response);

    // This uuid helps us track form progress updates.
    Poco::UUID formUUID = Poco::UUIDGenerator::defaultGenerator().createOne();

    // Get the content type header (already checked in parent route).
    Poco::Net::MediaType contentType(request.get("Content-Type", ""));

    if (contentType.matches(POST_CONTENT_TYPE_URLENCODED) ||
        contentType.matches(POST_CONTENT_TYPE_MULTIPART))
    {
        // Prepare the upload directory if needed.
        if (contentType.matches(POST_CONTENT_TYPE_MULTIPART))
        {
            ofDirectory _uploadFolder(_parent.getSettings().getUploadFolder());

            if (!_uploadFolder.exists())
            {
                ofLogError("PostRouteHandler::handleRequest") << "Upload folder does not exist and cannot be created.";
                response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                _parent.handleErrorRequest(request, response);
                return;
            }
        }

        PostRouteFileHandler postRoutePartHandler(_parent, sessionId, request, formUUID);

        Poco::Net::HTMLForm form(contentType.toString());
        form.setFieldLimit(_parent.getSettings().getFieldLimit());
        form.load(request, request.stream(), postRoutePartHandler);

        PostFormEventArgs args(sessionId,
                               request,
                               response,
                               formUUID,
                               form);

        ofNotifyEvent(_parent.events.onHTTPFormEvent, args, &_parent);

    }
    else
    {
        // Poco::Net::HTMLForm, like php does not handle text/plain because
        // it cannot be unambiguously encoded.  Here we simply return
        // the raw text with the event.

        std::string result;

        Poco::StreamCopier::copyToString(request.stream(), result);

        ofBuffer buffer(result);

        PostEventArgs args(sessionId, request, response, formUUID, buffer);

        ofNotifyEvent(_parent.events.onHTTPPostEvent, args, &_parent);

    }

    if (response.sent())
    {
        return;
    }
    else if(!_parent.getSettings().getUploadRedirect().empty())
    {
        response.redirect(_parent.getSettings().getUploadRedirect());
        return;
    }
    else
    {
        // done
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentLength(0);
        response.send();
        return;
    }
}


void PostRouteHandler::stop()
{
    // this should stop uploads, etc.
}


PostRouteFileHandler::PostRouteFileHandler(PostRoute& parent,
                                           const Poco::UUID& sessionId,
                                           const Poco::Net::HTTPServerRequest& request,
                                           const Poco::UUID& formUUID):
    _parent(parent),
    _sessionId(sessionId),
    _request(request),
    _formUUID(formUUID)
{
}


PostRouteFileHandler::~PostRouteFileHandler()
{
}


void PostRouteFileHandler::handlePart(const Poco::Net::MessageHeader& header,
                                      std::istream& stream)
{
    if (header.has("Content-Type"))
    {
        std::string contentType = header["Content-Type"];

        if (!_parent.getSettings().getValidContentTypes().empty() && !isContentTypeValid(contentType))
        {
            ofLogError("PostRouteFileHandler::handlePart") << "Invalid content type: " << contentType;
            return; // reject
        }
    }
    else
    {
        ofLogError("PostRouteFileHandler::handlePart") << "No Content-Type header.";
        return;
    }


    // Is this an uploaded file and are we allowing files to be uploaded?
    if (header.has("Content-Disposition") &&
        _parent.getSettings().getMaximumFileUploadSize() > 0)
    {
        std::string contentDisposition = header["Content-Disposition"];

        Poco::Net::NameValueCollection parameters;

        Poco::Net::MessageHeader::splitParameters(contentDisposition.begin(),
                                                  contentDisposition.end(),
                                                  parameters);

        std::string formFileName = parameters.get("filename", "");
        std::string formFieldName = parameters.get("name", "");

        if(!formFileName.empty())
        {
            try
            {
                std::stringstream ss;

                ss << _parent.getSettings().getUploadFolder();
                ss << "/";
                ss << Poco::UUIDGenerator::defaultGenerator().createOne().toString();
                ss << ".";
                ss << Poco::Path(formFileName).getExtension();

                std::string newFilename = ofToDataPath(ss.str(), true);

                ofFile file(newFilename, ofFile::WriteOnly,true);

                Poco::Net::MediaType contentType(header["Content-Type"]);

                PostUploadEventArgs args(_sessionId,
                                         _request,
                                         _formUUID,
                                         formFieldName,
                                         formFileName,
                                         newFilename,
                                         contentType,
                                         0,
                                         PostUploadEventArgs::UPLOAD_STARTING);

                ofNotifyEvent(_parent.events.onHTTPUploadEvent, args, &_parent);

                ofLogVerbose("PostRouteFileHandler::handlePart") << "Writing file to absolute path : " << file.getAbsolutePath();

                // The section below is from StreamCopier::copyStream,
                // and might be used for upload progress feedback

                Poco::Buffer<char> buffer(_parent.getSettings().getWriteBufferSize());

                std::streamsize sz = 0;

                stream.read(buffer.begin(), _parent.getSettings().getWriteBufferSize());

                unsigned long long n = stream.gcount();

                while (n > 0)
                {
                    if (sz > _parent.getSettings().getMaximumFileUploadSize())
                    {
                        ofLogError("PostRouteFileHandler::handlePart") << "File upload size exceeded.  Removing file.";
                        file.close();
                        ofFile::removeFile(newFilename, false);

                        return;
                    }

                    sz += n;

                    file.write(buffer.begin(), n);

                    if (stream && file)
                    {
                        stream.read(buffer.begin(),
                                    _parent.getSettings().getWriteBufferSize());

                        n = stream.gcount();
                    }
                    else
                    {
                        n = 0;
                    }

                    PostUploadEventArgs uploadArgs(_sessionId,
                                                   _request,
                                                   _formUUID,
                                                   formFieldName,
                                                   formFileName,
                                                   newFilename,
                                                   contentType,
                                                   sz,
                                                   PostUploadEventArgs::UPLOAD_PROGRESS);

                    ofNotifyEvent(_parent.events.onHTTPUploadEvent,
                                  uploadArgs,
                                  &_parent);
                }

                file.close();

                PostUploadEventArgs finishedArgs(_sessionId,
                                                 _request,
                                                 _formUUID,
                                                 formFieldName,
                                                 formFileName,
                                                 newFilename,
                                                 contentType,
                                                 sz,
                                                 PostUploadEventArgs::UPLOAD_FINISHED);

                ofNotifyEvent(_parent.events.onHTTPUploadEvent,
                              finishedArgs,
                              &_parent);

            }
            catch (const Poco::Exception& exc)
            {
                ofLogError("PostRouteFileHandler::handlePart") << exc.displayText();
            }
            catch (const std::exception& exc)
            {
                ofLogError("PostRouteFileHandler::handlePart") << exc.what();
            }
            catch ( ... )
            {
                ofLogError("PostRouteFileHandler::handlePart") << "Uncaught thread exception: Unknown exception.";
            }
        }
        else
        {
            ofLogError("PostRouteFileHandler::handlePart") << "No filename in header.";
        }
    }
}


bool PostRouteFileHandler::isContentTypeValid(const std::string& contentType) const
{
    Poco::Net::MediaType mediaType(contentType);
    
    std::set<Poco::Net::MediaType> validContentTypes = _parent.getSettings().getValidContentTypes();
    std::set<Poco::Net::MediaType>::const_iterator iter = validContentTypes.begin();
    
    while (iter != validContentTypes.end())
    {
        if ((*iter).matchesRange(mediaType))
        {
            return true;
        }
        
        ++iter;
    }
    
    return false;
}


} } // namespace ofx::HTTP
