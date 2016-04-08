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


#pragma once


#include "ofx/HTTP/BaseRequest.h"
#include "Poco/Net/HTMLForm.h"


namespace ofx {
namespace HTTP {


/// \brief A Custom Base HTTP Request.
class FormRequest: public BaseRequest
{
public:
    using BaseRequest::BaseRequest;

    /// \brief Destroy this BaseReuqest.
    virtual ~FormRequest();

    /// \brief Add a name value pair to upload with this request.
    ///
    /// When form fields are added to anything other then POST and PUT requests,
    /// the form fields will be converted to query parameters and appended to
    /// the URI before request submission.
    ///
    /// \param name The field name.
    /// \param value The field value.
    void addFormField(const std::string& name,
                      const std::string& value);

    /// \brief Set a name value pair to upload with this request.
    ///
    /// When form fields are added to anything other then POST and PUT requests,
    /// the form fields will be converted to query parameters and appended to
    /// the URI before request submission.
    ///
    /// \param name The field name.
    /// \param value The field value.
    void setFormField(const std::string& name,
                      const std::string& value);

    /// \brief Add additional form fields.
    ///
    /// When form fields are added to anything other then POST and PUT requests,
    /// the form fields will be converted to query parameters and appended to
    /// the URI before request submission.
    ///
    /// \param formFields The form fields to add.
    void addFormFields(const std::multimap<std::string, std::string>& formFields);

    /// \brief Clear all form fields.
    void clearFormFields();

    /// \brief The default MIME type.
    static const std::string DEFAULT_MEDIA_TYPE;

    /// \brief A friend class.
    friend class BaseClient;

};


} } // namespace ofx::HTTP
