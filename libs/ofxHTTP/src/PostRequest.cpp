// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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
    FormRequest(Poco::Net::HTTPRequest::HTTP_POST, uri, httpVersion)
{
}


PostRequest::~PostRequest()
{
}


void PostRequest::setFormEncoding(FormEncoding formEncoding)
{
    if (formEncoding == FORM_ENCODING_URL)
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
    if (_form.getEncoding() == Poco::Net::HTMLForm::ENCODING_URL)
    {
        return FORM_ENCODING_URL;
    }
    else
    {
        return FORM_ENCODING_MULTIPART;
    }
}


Poco::Net::HTMLForm& PostRequest::form()
{
    return _form;
}


void PostRequest::addFormPart(const FormPart& part)
{
    switch (part.type())
    {
        case FormPart::Type::STRING:
        {
            // _form takes ownership of StringPartSource.
            _form.addPart(part.name(), new Poco::Net::StringPartSource(part.value(),
                                                                       part.mediaType()));
            break;
        }
        case FormPart::Type::FILE:
        {
            try
            {
                // _form takes ownership of FilePartSource.
                _form.addPart(part.name(), new Poco::Net::FilePartSource(ofToDataPath(part.value(), true),
                                                                         part.mediaType()));
            }
            catch (const Poco::FileNotFoundException& exc)
            {
                ofLogError("PostRequest::addFile") << exc.displayText();
            }
            catch (const Poco::OpenFileException& exc)
            {
                ofLogError("PostRequest::addFile") << exc.displayText();
            }

            break;
        }
    }

    // We must set encoding to multipart.
    _form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
}


void PostRequest::addFormParts(const std::vector<FormPart>& parts)
{
    for (auto& part: parts)
    {
        addFormPart(part);
    }
}


void PostRequest::addFormFile(const std::string& name,
                              const std::string& path,
                              const std::string& mediaType)
{
    addFormPart(FormPart(FormPart::Type::FILE,
                         name,
                         path,
                         mediaType));
}


void PostRequest::addFormString(const std::string& name,
                                const std::string& buffer,
                                const std::string& mediaType)
{
    addFormPart(FormPart(FormPart::Type::STRING,
                         name,
                         buffer,
                         mediaType));
}


void PostRequest::prepareRequest()
{
    _form.prepareSubmit(*this);

    // \TODO Fix for Poco Bug
    // https://github.com/pocoproject/poco/issues/1331
    if (!getChunkedTransferEncoding())
    {
        // TODO: This long thing is a work-around for bug https://github.com/pocoproject/poco/issues/1337
        setContentLength(_form.getEncoding() == Poco::Net::HTMLForm::ENCODING_URL ? 0 : _form.calculateContentLength());
    }
}


void PostRequest::writeRequestBody(std::ostream& requestStream)
{
    _form.write(requestStream);
}


} } // namespace ofx::HTTP
