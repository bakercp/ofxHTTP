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


#include "ofx/HTTP/PostRequest.h"
#include "Poco/CountingStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/PartSource.h"
#include "Poco/Net/StringPartSource.h"


namespace ofx {
namespace HTTP {

    
PostRequest::PostRequest(const std::string& uri,
                         const std::string& httpVersion):
    BaseRequest(Poco::Net::HTTPRequest::HTTP_POST, uri, httpVersion)
{
}


PostRequest::~PostRequest()
{
}


void PostRequest::setFormEncoding(FormEncoding formEncoding)
{
    if (FORM_ENCODING_URL == formEncoding)
    {
        _form.setEncoding(Poco::Net::HTMLForm::ENCODING_URL);
    }
    else
    {
        _form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
    }
}


PostRequest::FormEncoding PostRequest::getFormEncoding() const
{
    if (0 == Poco::UTF8::icompare(_form.getEncoding(), Poco::Net::HTMLForm::ENCODING_URL))
    {
        return FORM_ENCODING_URL;
    }
    else
    {
        return FORM_ENCODING_MULTIPART;
    }
}


void PostRequest::addFormPart(const FormPart& part)
{
    _form.addPart(part.name, part.pSource);
    _form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
}


void PostRequest::addFormParts(const FormParts& parts)
{
    FormParts::const_iterator iter = parts.begin();

    while (iter != parts.end())
    {
        _form.addPart((*iter).name, (*iter).pSource);
        ++iter;
    }

    _form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
}


void PostRequest::addFormFile(const std::string& name,
                              const std::string& path,
                              const std::string& mediaType)
{

    std::string absPath = ofToDataPath(path, true);

    try
    {
        _form.addPart(name, new Poco::Net::FilePartSource(absPath, mediaType));
        _form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
    }
    catch (Poco::FileNotFoundException& exc)
    {
        ofLogError("PostRequest::addFile") << exc.displayText();
    }
    catch (Poco::OpenFileException& exc)
    {
        ofLogError("PostRequest::addFile") << exc.displayText();
    }
}


void PostRequest::addFormBuffer(const std::string& name,
                                const ofBuffer& buffer,
                                const std::string& mediaType)
{
    _form.addPart(name, new Poco::Net::StringPartSource(buffer.getText(),
                                                        mediaType));

    _form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
}


void PostRequest::prepareRequest()
{
    _form.prepareSubmit(*this);

    if (FORM_ENCODING_MULTIPART == getFormEncoding() &&
        Poco::Net::HTTPMessage::HTTP_1_0 == getVersion())
    {
        // If we are using 1.0 with file uploads, we must get a content
        // length before sending the data.
        _outBuffer.clear();
        Poco::CountingOutputStream cos(_outBuffer);
        _form.write(cos);
        set("Content-Length", ofToString(cos.chars()));
        setKeepAlive(false);
        setChunkedTransferEncoding(false);
    }
}


void PostRequest::writeRequestBody(std::ostream& requestStream)
{
    if (FORM_ENCODING_MULTIPART == getFormEncoding() &&
        Poco::Net::HTTPMessage::HTTP_1_0 == getVersion())
    {
        Poco::StreamCopier::copyStream(_outBuffer, requestStream);
    }
    else
    {
        _form.write(requestStream);
    }
}


} } // namespace ofx::HTTP
