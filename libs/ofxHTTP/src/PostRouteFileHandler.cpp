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


#include "ofx/HTTP/PostRouteFileHandler.h"
#include "ofx/HTTP/PostRoute.h"
#include "ofx/HTTP/Utils.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"


namespace ofx {
namespace HTTP {


PostRouteFileHandler::PostRouteFileHandler(const Poco::UUID& sessionUUID,
                                           const Poco::Net::HTTPServerRequest& request,
                                           PostRoute& parent,
                                           const Poco::UUID& formUUID):
    _sessionUUID(sessionUUID),
    _request(request),
    _parent(parent),
    _formUUID(formUUID)
{
}


PostRouteFileHandler::~PostRouteFileHandler()
{
}
    

void PostRouteFileHandler::handlePart(const Poco::Net::MessageHeader& header,
                                      std::istream& stream)
{
    if(header.has("Content-Type"))
    {
        std::string contentType = header["Content-Type"];

        if(!_parent.getSettings().getValidContentTypes().empty() && !isContentTypeValid(contentType))
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


    // is this an uploaded file?
    if(header.has("Content-Disposition"))// && header.has("form-data"))
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

                ofFile file(newFilename, ofFile::WriteOnly);

                Poco::Net::MediaType contentType(header["Content-Type"]);

                PostUploadEventArgs args(_sessionUUID,
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

                std::streamsize n = stream.gcount();

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

                    PostUploadEventArgs uploadArgs(_sessionUUID,
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

                PostUploadEventArgs finishedArgs(_sessionUUID,
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

    while(iter != validContentTypes.end())
    {
        if((*iter).matchesRange(mediaType))
        {
            return true;
        }

        ++iter;
    }
    
    return false;
}
    


} } // namespace ofx::HTTP
