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
