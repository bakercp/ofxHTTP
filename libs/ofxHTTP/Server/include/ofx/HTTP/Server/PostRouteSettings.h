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


#include <set>
#include <string>
#include <istream>
#include "Poco/Net/MediaType.h"
#include "ofx/HTTP/Server/BaseRouteSettings.h"


namespace ofx {
namespace HTTP {


/// \brief Settings for a PostRoute.
class PostRouteSettings: public BaseRouteSettings
{
public:
    /// \brief Create PostRouteSettings.
    /// \param routePathPattern The regex pattern that this route will handle.
    /// \param requireSecurePort True if this route requires
    ///        communication on an SSL encrypted port.
    PostRouteSettings(const std::string& routePathPattern = DEFAULT_POST_ROUTE,
                      bool requireSecurePort = false);

    /// \brief Destroy the PostRouteSetting.
    virtual ~PostRouteSettings();

    void setUploadFolder(const std::string& uploadFolder);
    const std::string& getUploadFolder() const;

    void setUploadRedirect(const std::string& uploadRedirect);
    const std::string& getUploadRedirect() const;

    void setWriteBufferSize(std::size_t writeBufferSize);
    std::size_t getWriteBufferSize() const;

    void setFieldLimit(std::size_t fieldLimit);
    std::size_t getFieldLimit() const;

    void setMaximumFileUploadSize(std::streamsize maximumFileUploadSize);
    std::streamsize getMaximumFileUploadSize() const;

    static const std::string DEFAULT_POST_ROUTE;
    static const std::string DEFAULT_POST_FOLDER;
    static const std::string DEFAULT_POST_REDIRECT;

    /// \brief Default values.
    enum Defaults
    {
        /// \brief File upload buffer.
        DEFAULT_POST_BUFFER_SIZE = 8192,
        /// \brief Maximum number of form fields.
        DEFAULT_FIELD_LIMIT = 100,
        /// \brief Maximum file upload size (2 MB)
        DEFAULT_MAXIMUM_FILE_UPLOAD_SIZE = 2097152
    };

    /// \brief An unfortunate compromise until C++11.
    /// \note C++ is not able to initialize static collections until
    ///        after C++11.  This is a compromise until then.
    static const std::string DEFAULT_POST_HTTP_METHODS_ARRAY[];

    /// \brief The default HTTP methods for this route.
    static const HTTPMethodSet DEFAULT_POST_HTTP_METHODS;

private:
    bool _requireUploadFolderInDataFolder;
    std::string _uploadFolder;
    bool _autoCreateUploadFolder;
    std::string _uploadRedirect;
    std::size_t _writeBufferSize;
    std::size_t _fieldLimit;
    std::streamsize _maximumFileUploadSize;

};


} } // namespace ofx::HTTP
