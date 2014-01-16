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
#include <istream>
#include "Poco/Net/MediaType.h"
#include "ofx/HTTP/Server/BaseRouteSettings.h"


namespace ofx {
namespace HTTP {


class FileUploadRouteSettings: public BaseRouteSettings
{
public:
    typedef std::set<Poco::Net::MediaType> MediaTypeSet;

    FileUploadRouteSettings(const std::string& routePathPattern = DEFAULT_UPLOAD_ROUTE,
                            bool requireSecurePort = false);
        ///< \param routePathPattern The regex pattern that this route
        ///<        will handle.
        ///< \param requireSecurePorttrue True if this route requires
        ///<        communication on an SSL encrypted port.

    virtual ~FileUploadRouteSettings();

    void setRequireUploadFolderInDataFolder(bool requireUploadFolderInDataFolder);
    bool getRequireUploadFolderInDataFolder() const;

    void setUploadFolder(const std::string& uploadFolder);
    const std::string& getUploadFolder() const;

    void setAutoCreateUploadFolder(bool autoCreateUploadFolder);
    bool getAutoCreateUploadFolder() const;

    void setUploadRedirect(const std::string& uploadRedirect);
    const std::string& getUploadRedirect() const;

    void setWriteBufferSize(std::size_t writeBufferSize);
    std::size_t getWriteBufferSize() const;

    void setValidContentTypes(const MediaTypeSet& validContentTypes);
    MediaTypeSet getValidContentTypes() const;

    void setAutoRename(bool autoRename);
    bool getAutoRename() const;

    static const std::string DEFAULT_UPLOAD_ROUTE;
        ///< \brief The default upload route path regex.

    static const std::string DEFAULT_UPLOAD_FOLDER;
        ///< \brief The default upload folder location.
        ///< \details This path will be passed to 
    
    static const std::string DEFAULT_UPLOAD_REDIRECT;
        ///< \brief This path is relative to the site's document root.

    enum
    {
        DEFAULT_BUFFER_SIZE = 8192
            ///< \brief The default filey upload buffer size.
    };

    static const std::string FILEUPLOAD_HTTP_METHODS_ARRAY[];
        ///< \brief An unfortunate compromise until C++11.
        ///< \note C++ is not able to initialize static collections until
        ///<        after C++11.  This is a compromise until then.

    static const HTTPMethodSet FILEUPLOAD_HTTP_METHODS;
        ///< \brief The default HTTP methods for this route.


private:
    bool _requireUploadFolderInDataFolder;
    std::string _uploadFolder;
    bool _autoCreateUploadFolder;
    std::string _uploadRedirect;
    std::size_t _writeBufferSize;
    bool _autoRename;

    MediaTypeSet _validContentTypes;

};


} } // namespace ofx::HTTP
