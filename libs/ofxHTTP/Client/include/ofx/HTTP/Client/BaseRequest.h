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


#pragma once


#include <string>
#include "ofTypes.h"
#include "ofLog.h"
#include "ofFileUtils.h"
#include "ofUtils.h"
#include "Poco/String.h"
#include "Poco/UTF8String.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTMLForm.h"


namespace ofx {
namespace HTTP {
namespace Client {


class BaseRequest: public Poco::Net::HTTPRequest
{
public:
    /// \brief A shared pointer to a base request.
    typedef std::shared_ptr<BaseRequest> SharedPtr;

    /// \brief Construct a Request with the given information.
    /// \param uri the endpoint uri.
    /// \param verion Either HTTP/1.0 or HTTP/1.1.
    /// \param requestId A unique UUID for this request.
	BaseRequest(const std::string& method,
                const std::string& uri,
                const std::string& httpVersion,
                const Poco::UUID& requestId = generateUUID());

    /// \brief Destroy this BaseReuqest.
    virtual ~BaseRequest();

    const Poco::UUID& getRequestId() const;

    static Poco::UUID generateUUID();

    /// \brief The default MIME type.
    static const std::string DEFAULT_MEDIA_TYPE;

    virtual void prepareRequest();

    virtual void writeRequestBody(std::ostream& requestStream);

protected:
    Poco::UUID _requestId;

    friend class BaseClient;

};


} } } // namespace ofx::HTTP::Client
