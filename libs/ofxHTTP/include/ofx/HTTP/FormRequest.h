//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/Request.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/NameValueCollection.h"


namespace ofx {
namespace HTTP {


/// \brief A Custom Base HTTP Request.
class FormRequest: public Request
{
public:
    using Request::Request;

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
    void addFormFields(const Poco::Net::NameValueCollection& formFields);

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
