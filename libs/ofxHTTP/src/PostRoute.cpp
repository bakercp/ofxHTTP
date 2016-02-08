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


#include "ofx/HTTP/PostRoute.h"
#include "Poco/Buffer.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/MediaType.h"


namespace ofx {
namespace HTTP {


const std::string PostRouteSettings::DEFAULT_POST_ROUTE    = "/post";
const std::string PostRouteSettings::DEFAULT_POST_FOLDER   = "uploads/";
const std::string PostRouteSettings::DEFAULT_POST_REDIRECT = "uploaded.html";
const std::string PostRouteSettings::DEFAULT_POST_HTTP_METHODS_ARRAY[] = { "POST" };
const BaseRouteSettings::HTTPMethodSet PostRouteSettings::DEFAULT_POST_HTTP_METHODS(INIT_SET_WITH_ARRAY(DEFAULT_POST_HTTP_METHODS_ARRAY));


PostRouteSettings::PostRouteSettings(const std::string& routePathPattern,
                                     bool requireSecurePort,
                                     bool requireAuthentication):
    BaseRouteSettings(routePathPattern,
                      requireSecurePort,
                      requireAuthentication,
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


void PostRouteSettings::setMaximumFileUploadSize(uint64_t maximumFileUploadSize)
{
    _maximumFileUploadSize = maximumFileUploadSize;
}


uint64_t PostRouteSettings::getMaximumFileUploadSize() const
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


Poco::Net::HTTPRequestHandler* PostRoute::createRequestHandler(const Poco::Net::HTTPServerRequest&)
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


PostRouteHandler::PostRouteHandler(PostRoute& route):
    BaseRouteHandler_<PostRoute>(route)
{
}


PostRouteHandler::~PostRouteHandler()
{
}


void PostRouteHandler::handleRequest(ServerEventArgs& evt)
{
    try
    {
        Poco::Net::HTTPServerResponse& response = evt.getResponse();

        // This uuid helps us track form progress updates.
        std::string postId = Poco::UUIDGenerator::defaultGenerator().createOne().toString();

        // Get the content type header (already checked in parent route).
        Poco::Net::MediaType contentType(evt.getRequest().get("Content-Type", ""));

        if (contentType.matches(POST_CONTENT_TYPE_URLENCODED) ||
            contentType.matches(POST_CONTENT_TYPE_MULTIPART))
        {
            // Prepare the upload directory if needed.
            if (contentType.matches(POST_CONTENT_TYPE_MULTIPART))
            {
                ofDirectory _uploadFolder(getRoute().getSettings().getUploadFolder());

                if (!_uploadFolder.exists())
                {
                    ofLogError("PostRouteHandler::handleRequest") << "Upload folder does not exist and cannot be created.";
                    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                    getRoute().handleRequest(evt);
                    return;
                }
            }

            PostRouteFileHandler postRoutePartHandler(getRoute(),
                                                      evt,
                                                      postId);

            Poco::Net::HTMLForm form(contentType.toString());
            form.setFieldLimit(getRoute().getSettings().getFieldLimit());
            form.load(evt.getRequest(), evt.getRequest().stream(), postRoutePartHandler);

            PostFormEventArgs args(evt,
                                   postId,
                                   form);

            ofNotifyEvent(getRoute().events.onHTTPFormEvent, args, &getRoute());

            if (form.has("destination") && !form.get("destination").empty())
            {
                response.redirect(form.get("destination"));
                return;
            }
        }
        else
        {
            // Poco::Net::HTMLForm, like php does not handle text/plain because
            // it cannot be unambiguously encoded.  Here we simply return
            // the raw text with the event.

            std::string result;

            Poco::StreamCopier::copyToString(evt.getRequest().stream(),
                                             result);

            IO::ByteBuffer buffer(result);

            PostEventArgs args(evt,
                               postId,
                               buffer);

            ofNotifyEvent(getRoute().events.onHTTPPostEvent, args, &getRoute());
        }

        if (response.sent())
        {
            return;
        }
        else if (!getRoute().getSettings().getUploadRedirect().empty())
        {
            response.redirect(getRoute().getSettings().getUploadRedirect());
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
    catch (const Poco::Exception& exc)
    {
        evt.getResponse().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, exc.displayText());
    }
    catch (const std::exception& exc)
    {
        evt.getResponse().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, exc.what());
    }
    catch (...)
    {
        evt.getResponse().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    }
}


PostRouteFileHandler::PostRouteFileHandler(PostRoute& route,
                                           ServerEventArgs& evt,
                                           const std::string& postId):
    _route(route),
    _evt(evt),
    _postId(postId)
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

        if (!_route.getSettings().getValidContentTypes().empty() && !isContentTypeValid(contentType))
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
        _route.getSettings().getMaximumFileUploadSize() > 0)
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

                ss << _route.getSettings().getUploadFolder();
                ss << "/";
                ss << Poco::UUIDGenerator::defaultGenerator().createOne().toString();
                ss << ".";
                ss << Poco::Path(formFileName).getExtension();

                std::string newFilename = ofToDataPath(ss.str(), true);

                ofFile file(newFilename, ofFile::WriteOnly,true);

                Poco::Net::MediaType contentType(header["Content-Type"]);

                PostUploadEventArgs args(_evt,
                                         _postId,
                                         formFieldName,
                                         formFileName,
                                         newFilename,
                                         contentType,
                                         0,
                                         PostUploadEventArgs::UPLOAD_STARTING);

                ofNotifyEvent(_route.events.onHTTPUploadEvent, args, &_route);

                ofLogVerbose("PostRouteFileHandler::handlePart") << "Writing file to absolute path : " << file.getAbsolutePath();

                // The section below is from StreamCopier::copyStream,
                // and might be used for upload progress feedback

                Poco::Buffer<char> buffer(_route.getSettings().getWriteBufferSize());

                stream.read(buffer.begin(), _route.getSettings().getWriteBufferSize());

                unsigned long long sz = 0;
                unsigned long long n = stream.gcount();

                while (n > 0)
                {
                    if (sz > _route.getSettings().getMaximumFileUploadSize())
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
                                    _route.getSettings().getWriteBufferSize());

                        n = stream.gcount();
                    }
                    else
                    {
                        n = 0;
                    }

                    PostUploadEventArgs uploadArgs(_evt,
                                                   _postId,
                                                   formFieldName,
                                                   formFileName,
                                                   newFilename,
                                                   contentType,
                                                   sz,
                                                   PostUploadEventArgs::UPLOAD_PROGRESS);

                    ofNotifyEvent(_route.events.onHTTPUploadEvent,
                                  uploadArgs,
                                  &_route);
                }

                file.close();

                PostUploadEventArgs finishedArgs(_evt,
                                                 _postId,
                                                 formFieldName,
                                                 formFileName,
                                                 newFilename,
                                                 contentType,
                                                 sz,
                                                 PostUploadEventArgs::UPLOAD_FINISHED);

                ofNotifyEvent(_route.events.onHTTPUploadEvent,
                              finishedArgs,
                              &_route);

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
    
    const BaseRouteSettings::MediaTypeSet& validContentTypes = _route.getSettings().getValidContentTypes();

    BaseRouteSettings::MediaTypeSet::const_iterator iter = validContentTypes.begin();
    
    while (iter != validContentTypes.end())
    {
        Poco::Net::MediaType type(*iter);

        if (type.matchesRange(mediaType))
        {
            return true;
        }
        
        ++iter;
    }

    return false;
}


} } // namespace ofx::HTTP
