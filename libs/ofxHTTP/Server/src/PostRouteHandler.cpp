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


#include "ofx/HTTP/Server/PostRouteHandler.h"


namespace ofx {
namespace HTTP {


const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_MULTIPART  = Poco::Net::MediaType("multipart/form-data");
const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_TEXT_PLAIN = Poco::Net::MediaType("text/plain");
const Poco::Net::MediaType PostRouteHandler::POST_CONTENT_TYPE_URLENCODED = Poco::Net::MediaType("application/x-www-form-urlencoded");


PostRouteHandler::PostRouteHandler(PostRouteInterface& parent):
    BaseRouteHandler(parent),
    _parent(parent),
    contentLength(0)
{
}


PostRouteHandler::~PostRouteHandler()
{
}


void PostRouteHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                     Poco::Net::HTTPServerResponse& response)
{
    if(ofGetLogLevel() <= OF_LOG_VERBOSE)
    {
        Poco::Net::NameValueCollection::ConstIterator iter = request.begin();
        while(iter != request.end())
        {
            ofLogVerbose("PostRouteHandler::handleRequest") << "XYZ Part: " << (*iter).first << "=" << (*iter).second;
            ++iter;
        }
    }

    if(request.has("Content-Type"))
    {
        // get the content type header
        Poco::Net::MediaType contentType(request.get("Content-Type"));

        if (contentType.matches(POST_CONTENT_TYPE_URLENCODED))
        {
//            PostEventArgs args;
            std::istream& stream = request.stream();
            Poco::Buffer<char> buffer(_parent.getSettings().getWriteBufferSize());
            std::streamsize sz = 0;
            stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
            std::streamsize n = stream.gcount();
            std::stringstream ss;
            while (n > 0)
            {
                sz += n;
                ss.write(buffer.begin(), n);
                if (stream && ss)
                {
                    stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
                    n = stream.gcount();

//                    args = PostEventArgs(fileName,contentLength,sz);
//                    ofNotifyEvent(_parent.getEventsRef().onUploadProgress, args);

                }
                else n = 0;

            }





//            PostEventArgs args;

        }
        else if (contentType.matches(POST_CONTENT_TYPE_TEXT_PLAIN))
        {
            //            PostEventArgs args;
            std::istream& stream = request.stream();

            Poco::Buffer<char> buffer(_parent.getSettings().getWriteBufferSize());
            std::streamsize sz = 0;
            stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
            std::streamsize n = stream.gcount();
            std::stringstream ss;
            while (n > 0)
            {
                sz += n;
                ss.write(buffer.begin(), n);
                if (stream && ss)
                {
                    stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
                    n = stream.gcount();

                    //                    args = PostEventArgs(fileName,contentLength,sz);
                    //                    ofNotifyEvent(_parent.getEventsRef().onUploadProgress, args);
                    
                }
                else n = 0;
            }

            cout << ss.str() << endl;
        }
        else if (contentType.matches(POST_CONTENT_TYPE_MULTIPART))
        {
            ofLogError("PostRouteHandler::handleRequest") << "----------.";

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

            Poco::Path dataFolder(ofToDataPath("", true));
            Poco::Path uploadFolder(ofToDataPath(_parent.getSettings().getUploadFolder(), true));

            std::string dataFolderString = dataFolder.toString();
            std::string uploadFolderString = uploadFolder.toString();

            // upload folder validity check
            if(_parent.getSettings().getRequireUploadFolderInDataFolder() &&
               (uploadFolderString.length() < dataFolderString.length() ||
                uploadFolderString.substr(0, dataFolderString.length()) != dataFolderString))
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
                    ofLogVerbose("PostRouteHandler::handleRequest") << "XYZ Part: " << (*iter).first << "=" << (*iter).second;
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
        }
        else
        {
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR,
                                        "Unknown Content-Type: " + contentType.toString());
            _parent.handleRequest(request, response);
            return;
        }
    }
    else
    {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "No Content-Type Header");
        _parent.handleRequest(request, response);
        return;
    }


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


void PostRouteHandler::handlePart(const Poco::Net::MessageHeader& header,
                                  std::istream& stream)
{
//    Poco::Net::NameValueCollection::ConstIterator iter = header.begin();
//
//    while(iter != header.end())
//    {
//        ofLogVerbose("PostRouteHandler::handlePart") << "XXPart: " << (*iter).first << "=" << (*iter).second;
//        ++iter;
//    }
//
//    if(header.has("Content-Type"))
//    {
//        std::string contentType = header["Content-Type"];
//
//        if(_parent.getSettings().getRequireValidContentType() && !isContentTypeValid(contentType))
//        {
//            ofLogError("PostRouteHandler::handlePart") << "Invalid content type: " << contentType;
//            return; // reject
//        }
//    }
//    else
//    {
//        ofLogError("PostRouteHandler::handlePart") << "No Content-Type header.";
//        return;
//    }
//
//    // is this an uploaded file?
//    if(header.has("Content-Disposition"))
//    {
//        std::string contentDisposition = header["Content-Disposition"];
//
//        Poco::Net::NameValueCollection parameters;
//
//        Poco::Net::MessageHeader::splitParameters(contentDisposition.begin(),
//                                                  contentDisposition.end(),
//                                                  parameters);
//
//
//        if (parameters.has("form-data"))
//        {
//
//        }
//        else
//        {
//
//        }
//
//
//

//        if(parameters.has("filename"))
//        {
//            try
//            {
//                std::string fileName = ofToDataPath(_parent.getSettings().getUploadFolder() + "/" + parameters["filename"], true);
//
//                ofFile file(fileName, ofFile::WriteOnly);
//
//                PostEventArgs args(fileName,contentLength,0);
////                ofNotifyEvent(_parent.getEventsRef().onUploadStarted,args);
//
//                ofLogVerbose("PostRouteHandler::handlePart") << "Writing file to absolute path : " << file.getAbsolutePath();
//                
//                //std::streamsize sz = Poco::StreamCopier::copyStream(stream,file,_settings.getWriteBufferSize());
//
//                // The section below is from StreamCopier::copyStream,
//                // and might be used for upload progress feedback
//                
//                Poco::Buffer<char> buffer(_parent.getSettings().getWriteBufferSize());
//                std::streamsize sz = 0;
//                stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
//                std::streamsize n = stream.gcount();
//                while (n > 0)
//                {
//                    sz += n;
//                    file.write(buffer.begin(), n);
//                    if (stream && file)
//                    {
//                        stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
//                        n = stream.gcount();
//                    }
//                    else n = 0;
//
//                    args = PostEventArgs(fileName,contentLength,sz);
////                    ofNotifyEvent(_parent.getEventsRef().onUploadProgress,args);
//                }
//
//                file.close();
//
//                args = PostEventArgs(fileName,contentLength,sz);
////                ofNotifyEvent(_parent.getEventsRef().onUploadFinished,args);
//            }
//            catch (const Poco::Exception& exc)
//            {
//                ofLogError("PostRouteHandler::::handlePart") << exc.displayText();
//            }
//            catch (const std::exception& exc)
//            {
//                ofLogError("PostRouteHandler::::handlePart") << exc.what();
//            }
//            catch ( ... )
//            {
//                ofLogError("PostRouteHandler::::handlePart") << "Uncaught thread exception: Unknown exception.";
//            }
//        }
//        else
//        {
//            ofLogError("PostRouteHandler::::handlePart") << "No filename in header.";
//        }
//    }
//    else
//    {
//        cout << "IN HERE IN HERE IN HERE" << std::endl;
//
//
//        Poco::Buffer<char> buffer(_parent.getSettings().getWriteBufferSize());
//        std::streamsize sz = 0;
//        stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
//        std::streamsize n = stream.gcount();
//        std::stringstream ss;
//        while (n > 0)
//        {
//            sz += n;
//            ss.write(buffer.begin(), n);
//            if (stream && ss)
//            {
//                stream.read(buffer.begin(),_parent.getSettings().getWriteBufferSize());
//                n = stream.gcount();
//            }
//            else n = 0;
//
//        }
//
//        std::cout << ">> " << ss.str() << std::endl;
//    }
}


bool PostRouteHandler::isContentTypeValid(const std::string& contentType) const
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
