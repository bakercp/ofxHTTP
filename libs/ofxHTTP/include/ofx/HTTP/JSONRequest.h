//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofConstants.h"
#include "ofJson.h"
#include "ofx/HTTP/Request.h"


namespace ofx {
namespace HTTP {


/// \brief An HTTP POST Request with a JSON request body.
class JSONRequest: public Request
{
public:
    /// \brief Construct a JSONRequest with a given uri and http version.
    /// \param uri the POST endpoint uri.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    JSONRequest(const std::string& uri,
                const std::string& httpVersion = Poco::Net::HTTPMessage::HTTP_1_1);

    /// \brief Construct a JSONRequest with a given uri and http version.
    /// \param uri the Post endpoint uri.
    /// \param json The json to send.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    JSONRequest(const std::string& uri,
                const ofJson& json,
                const std::string& httpVersion);

    /// \brief Destroy the JSONRequest.
    virtual ~JSONRequest();

    /// \brief Set the JSON content.
    /// \param json The JSON content.
    virtual void setJSON(const ofJson& json);

    /// \returns a const reference to the request JSON.
    virtual const ofJson& getJSON() const;

    /// \brief The JSON media type.
    static const std::string JSON_MEDIA_TYPE;

protected:
    /// \brief Custom prepareRequest() to set the content length;
    virtual void prepareRequest() override;

    /// \brief Writes from the form buffer.
    /// \param requestStream The stream to write the request body.
    void writeRequestBody(std::ostream& requestStream) override;
    
    /// \brief The JSON data to send.
    ofJson _json;

private:
    /// \brief An output buffer.
    std::stringstream _outBuffer;
    
};


} } // namespace ofx::HTTP
