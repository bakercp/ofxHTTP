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
    FileUploadRouteSettings(const std::string& routePathPattern = DEFAULT_UPLOAD_ROUTE);
    virtual ~FileUploadRouteSettings();

    void setRequireUploadFolderInDataFolder(bool requireUploadFolderInDataFolder);
    bool getRequireUploadFolderInDataFolder() const;

    void setUploadFolder(const std::string& uploadFolder);
    std::string getUploadFolder() const;

    void setAutoCreateUploadFolder(bool autoCreateUploadFolder);
    bool getAutoCreateUploadFolder() const;

    void setUploadRedirect(const std::string& uploadRedirect);
    std::string getUploadRedirect() const;

    void setWriteBufferSize(std::size_t writeBufferSize);
    std::size_t getWriteBufferSize() const;

    void setRequireValidContentType(bool requireValidContentType);
    bool getRequireValidContentType() const;

    void setValidContentTypes(const std::vector<Poco::Net::MediaType>& valid);
    std::vector<Poco::Net::MediaType> getValidContentTypes() const;

    void setAutoRename(bool autoRename);
    bool getAutoRename() const;

    static const std::string DEFAULT_UPLOAD_ROUTE;
    static const std::string DEFAULT_UPLOAD_FOLDER;
    static const std::string DEFAULT_UPLOAD_REDIRECT;
    static const std::size_t DEFAULT_BUFFER_SIZE;

private:
    bool        _requireUploadFolderInDataFolder;
    std::string _uploadFolder;
    bool        _autoCreateUploadFolder;
    std::string _uploadRedirect;
    std::size_t _writeBufferSize;
    bool        _autoRename;

    bool _requireValidContentType;
    std::vector<Poco::Net::MediaType> _validContentTypes;

};


} } // namespace ofx::HTTP
