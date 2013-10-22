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


#include "ofx/HTTP/Server/FileUploadRouteHandler.h"


namespace ofx {
namespace HTTP {


//------------------------------------------------------------------------------
FileUploadRouteHandler::FileUploadRouteHandler(FileUploadRouteInterface& parent):
    BaseRouteHandler(parent),
    _parent(parent),
    contentLength(0)
{
}

//------------------------------------------------------------------------------
FileUploadRouteHandler::~FileUploadRouteHandler()
{
}

//------------------------------------------------------------------------------
void FileUploadRouteHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                           Poco::Net::HTTPServerResponse& response)
{
    ofDirectory uploadsDirectory(_parent.getSettings().getUploadFolder());

    if(!uploadsDirectory.exists())
    {
        if(_parent.getSettings().getAutoCreateUploadFolder())
        {
            uploadsDirectory.create();
        }
        else
        {
            ofLogError("ServerUploadRouteHandler::handleRequest") << "Upload folder does not exist and cannot be created.";
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            _parent.handleRequest(request,response);
            return;
        }
    }


    if(_parent.getSettings().getAutoCreateUploadFolder())
    {
        uploadsDirectory.create();
    }

    Poco::Path dataFolder(ofToDataPath("",true));
    Poco::Path uploadFolder(ofToDataPath(_parent.getSettings().getUploadFolder(),true));

    std::string dataFolderString = dataFolder.toString();
    std::string uploadFolderString = uploadFolder.toString();

    // upload folder validity check
    if(_parent.getSettings().getRequireUploadFolderInDataFolder() &&
       (uploadFolderString.length() < dataFolderString.length() ||
        uploadFolderString.substr(0,dataFolderString.length()) != dataFolderString))
    {
           ofLogError("ServerUploadRouteHandler::handleRequest") << "Upload folder is not a sub directory of the data folder.";
           response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            _parent.handleRequest(request,response);
            return;
    }

    if(ofGetLogLevel() <= OF_LOG_VERBOSE)
    {
        Poco::Net::NameValueCollection::ConstIterator iter = request.begin();
        while(iter != request.end())
        {
            ofLogVerbose("FileUploadRouteHandler::handleRequest") << "Part: " << (*iter).first << "=" << (*iter).second;
            ++iter;
        }
    }

    if(request.has("Content-Length"))
    {
        std::istringstream value(request.get("Content-Length"));
        value >> contentLength;
    }

    // this is a reason to break apart the requesthandler factory and the handler
    // multiple handlers could be useful to keep local variables to share with the
    // part handler.

    Poco::Net::HTMLForm form(request, request.stream(), *this);

    if(!_parent.getSettings().getUploadRedirect().empty())
    {
        response.redirect(_parent.getSettings().getUploadRedirect());
    }
    else
    {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentLength(0);
        response.send();
    }

}

//------------------------------------------------------------------------------
bool FileUploadRouteHandler::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
                                              bool isSecurePort) const
{
    return request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST;
}

//------------------------------------------------------------------------------
void FileUploadRouteHandler::handlePart(const Poco::Net::MessageHeader& header,
                                        std::istream& stream)
{
    Poco::Net::NameValueCollection::ConstIterator iter = header.begin();
    while(iter != header.end())
    {
        ofLogVerbose("FileUploadRouteHandler::handlePart") << "Part: " << (*iter).first << "=" << (*iter).second;
        ++iter;
    }


    if(header.has("Content-Type"))
    {
        std::string contentType = header["Content-Type"];
        if(_parent.getSettings().getRequireValidContentType() && !isContentTypeValid(contentType))
        {
            ofLogError("FileUploadRouteHandler::handlePart") << "Invalid content type: " << contentType;
            return; // reject
        }
    }
    else
    {
        ofLogError("FileUploadRouteHandler::handlePart") << "No Content-Type header.";
        return;
    }

    // is this an uploaded file?
    if(header.has("Content-Disposition"))// && header.has("form-data"))
    {
        std::string contentDisposition = header["Content-Disposition"];
        Poco::Net::NameValueCollection parameters;
        Poco::Net::MessageHeader::splitParameters(contentDisposition.begin(),contentDisposition.end(),parameters);
        
        if(parameters.has("filename"))
        {
            try
            {
                std::string fileName = ofToDataPath(_parent.getSettings().getUploadFolder() + "/" + parameters["filename"],true);

                ofFile file(fileName, ofFile::WriteOnly);

                FileUploadEventArgs args(fileName,contentLength,0);
                ofNotifyEvent(_parent.getEventsRef().onUploadStarted,args);

                ofLogVerbose("FileUploadRouteHandler::handlePart") << "Writing file to absolute path : " << file.getAbsolutePath();
                
                //std::streamsize sz = Poco::StreamCopier::copyStream(stream,file,_settings.getWriteBufferSize());

                // The section below is from StreamCopier::copyStream,
                // and might be used for upload progress feedback
                
                Poco::Buffer<char> buffer(_parent.getSettings().getWriteBufferSize());
                std::streamsize sz = 0;
                stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
                std::streamsize n = stream.gcount();
                while (n > 0)
                {
                    sz += n;
                    file.write(buffer.begin(), n);
                    if (stream && file)
                    {
                        stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
                        n = stream.gcount();
                    }
                    else n = 0;

                    args = FileUploadEventArgs(fileName,contentLength,sz);
                    ofNotifyEvent(_parent.getEventsRef().onUploadProgress,args);
                }

                file.close();

                args = FileUploadEventArgs(fileName,contentLength,sz);
                ofNotifyEvent(_parent.getEventsRef().onUploadFinished,args);
            }
            catch (const Poco::Exception& exc)
            {
                ofLogError("FileUploadRouteHandler::::handlePart") << exc.displayText();
            }
            catch (const std::exception& exc)
            {
                ofLogError("FileUploadRouteHandler::::handlePart") << exc.what();
            }
            catch ( ... )
            {
                ofLogError("FileUploadRouteHandler::::handlePart") << "Uncaught thread exception: Unknown exception.";
            }
        }
        else
        {
            ofLogError("FileUploadRouteHandler::::handlePart") << "No filename in header.";
        }
    }
}

//------------------------------------------------------------------------------
bool FileUploadRouteHandler::isContentTypeValid(const std::string& contentType) const
{
    Poco::Net::MediaType mediaType(contentType);

    std::vector<Poco::Net::MediaType> validContentTypes = _parent.getSettings().getValidContentTypes();
    std::vector<Poco::Net::MediaType>::const_iterator iter = validContentTypes.begin();

    while(iter != validContentTypes.end())
    {
        if((*iter).matchesRange(mediaType)) return true;
        ++iter;
    }

    return false;
}

    
} } // namespace ofx::HTTP
