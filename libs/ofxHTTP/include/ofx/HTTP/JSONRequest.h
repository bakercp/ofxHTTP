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


namespace ofx {
namespace HTTP {



/// \brief An HTTP POST Request with a JSON request body.
class JSONRequest: public BaseRequest
{
public:
    /// \brief Construct a JSONRequest with a given uri and http version.
    /// \param uri the Post endpoint uri.
    /// \param httpVersion Either HTTP/1.0 or HTTP/1.1.
    JSONRequest(const std::string& uri,
                const std::string& httpVersion);

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
    mutable ofJson _json;

private:
    /// \brief An output buffer.
    std::stringstream _outBuffer;
    
};




} } // namespace ofx::HTTP
