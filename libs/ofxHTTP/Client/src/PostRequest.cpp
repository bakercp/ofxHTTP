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


#include "ofx/HTTP/Client/PostRequest.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/PartSource.h"
#include "Poco/Net/StringPartSource.h"
#include "ofUtils.h"


namespace ofx {
namespace HTTP {
namespace Client {


const std::string PostRequest::DEFAULT_MEDIA_TYPE = "application/octet-stream";


PostRequest::PostRequest(const std::string& uri):
    BaseRequest(Poco::Net::HTTPRequest::HTTP_POST,
                uri,
                Poco::Net::HTTPMessage::HTTP_1_0),
    _form(std::shared_ptr<Poco::Net::HTMLForm>(new Poco::Net::HTMLForm()))
{
}


PostRequest::PostRequest(const std::string& uri,
                         const std::string& httpVersion):
    BaseRequest(Poco::Net::HTTPRequest::HTTP_POST, uri, httpVersion),
    _form(std::shared_ptr<Poco::Net::HTMLForm>(new Poco::Net::HTMLForm()))
{
}


PostRequest::~PostRequest()
{
}


void PostRequest::setFormEncoding(FormEncoding formEncoding)
{
    if (FORM_ENCODING_URL == formEncoding)
    {
        _form->setEncoding(Poco::Net::HTMLForm::ENCODING_URL);
    }
    else
    {
        _form->setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
    }
}


PostRequest::FormEncoding PostRequest::getFormEncoding() const
{
    if (_form->getEncoding().compare(Poco::Net::HTMLForm::ENCODING_URL) == 0)
    {
        return FORM_ENCODING_MULTIPART;
    }
    else
    {
        return FORM_ENCODING_URL;
    }
}


void PostRequest::addField(const std::string& name,
                           const std::string& value)
{
    _form->add(name, value);
}


void PostRequest::addFile(const std::string& name,
                          const std::string& path,
                          const std::string& mediaType)
{

    std::string absPath = ofToDataPath(path, true);

    try
    {
        _form->addPart(name, new Poco::Net::FilePartSource(absPath, mediaType));
        _form->setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
    }
    catch (Poco::OpenFileException& exc)
    {
        ofLogError("PostRequest::addFile") << exc.displayText();
    }
}


void PostRequest::addBuffer(const std::string& name,
                            const ofBuffer& buffer,
                            const std::string& mediaType)
{
    _form->addPart(name, new Poco::Net::StringPartSource(buffer.getText(),
                                                         mediaType));
    _form->setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
}


void PostRequest::clear()
{
    _form = std::shared_ptr<Poco::Net::HTMLForm>(new Poco::Net::HTMLForm());
}


void PostRequest::finalizeRequest()
{
    _form->prepareSubmit(*this);
}


void PostRequest::writeRequestBody(std::ostream& requestStream)
{
    cout << "Writing request stream" << endl;
    _form->write(requestStream);
    clear();
}


} } } // ofx::HTTP::Request
