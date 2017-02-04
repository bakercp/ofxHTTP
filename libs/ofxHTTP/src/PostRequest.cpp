//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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


} } // namespace ofx::HTTP
