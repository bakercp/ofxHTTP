//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofx/HTTP/Response"


namespace ofx {
namespace HTTP {



///// \brief An HTTP POST Request with a JSON request body.
//class JSONResponse: public Response
//{
//public:
//    /// \brief Construct a JSONResponse
//    JSONResponse();
//
//    /// \brief Construct a JSONResponse
//    /// \param json The JSON content.
//    JSONResponse(const ofJson& json);
//
//    /// \brief Destroy the JSONRequest.
//    virtual ~JSONRequest();
//
//    /// \brief Set the JSON content.
//    /// \param json The JSON content.
//    virtual void setJSON(const ofJson& json);
//
//    /// \returns a const reference to the request JSON.
//    virtual const ofJson& getJSON() const;
//
//    /// \brief The JSON media type.
//    static const std::string JSON_MEDIA_TYPE;
//
//protected:
//    /// \brief Custom prepareRequest() to set the content length;
//    virtual void prepareRequest() override;
//
//    /// \brief Writes from the form buffer.
//    /// \param requestStream The stream to write the request body.
//    void writeRequestBody(std::ostream& requestStream) override;
//    
//    /// \brief The JSON data to send.
//    mutable ofJson _json;
//
//private:
//    /// \brief An output buffer.
//    std::stringstream _outBuffer;
//    
//};




} } // namespace ofx::HTTP
