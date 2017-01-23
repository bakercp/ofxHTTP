//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/FormRequest.h"


namespace ofx {
namespace HTTP {


const std::string FormRequest::DEFAULT_MEDIA_TYPE = "application/octet-stream";


FormRequest::~FormRequest()
{
}


void FormRequest::addFormFields(const Poco::Net::NameValueCollection& formFields)
{
    for (auto& entry : formFields)
    {
        _form.add(entry.first, entry.second);
    }
}


void FormRequest::addFormFields(const std::multimap<std::string, std::string>& formFields)
{
    for (auto& entry : formFields)
    {
        _form.add(entry.first, entry.second);
    }
}


void FormRequest::addFormField(const std::string& name,
                               const std::string& value)
{
    _form.add(name, value);
}


void FormRequest::setFormField(const std::string& name,
                               const std::string& value)
{
    _form.set(name, value);
}


void FormRequest::clearFormFields()
{
    _form.clear();
}


} } // namespace ofx::HTTP
