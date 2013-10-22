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


#include "ofx/HTTP/Server/FileUploadRouteSettings.h"


namespace ofx {
namespace HTTP {

    
const std::string FileUploadRouteSettings::DEFAULT_UPLOAD_ROUTE    = "/upload";
const std::string FileUploadRouteSettings::DEFAULT_UPLOAD_FOLDER   = "uploads/";
const std::string FileUploadRouteSettings::DEFAULT_UPLOAD_REDIRECT = "uploaded.html";
const std::size_t FileUploadRouteSettings::DEFAULT_BUFFER_SIZE     = 8192;


//------------------------------------------------------------------------------
FileUploadRouteSettings::FileUploadRouteSettings(const std::string& routePathPattern):
    BaseRouteSettings(routePathPattern),
    _requireUploadFolderInDataFolder(true),
    _uploadFolder(DEFAULT_UPLOAD_FOLDER),
    _autoCreateUploadFolder(false),
    _uploadRedirect(DEFAULT_UPLOAD_REDIRECT),
    _writeBufferSize(DEFAULT_BUFFER_SIZE),
    _requireValidContentType(false),
    _autoRename(true)
{
}

//------------------------------------------------------------------------------
FileUploadRouteSettings::~FileUploadRouteSettings()
{
}

//------------------------------------------------------------------------------
void FileUploadRouteSettings::setRequireUploadFolderInDataFolder(bool requireUploadFolderInDataFolder)
{
    _requireUploadFolderInDataFolder = requireUploadFolderInDataFolder;
}

//------------------------------------------------------------------------------
bool FileUploadRouteSettings::getRequireUploadFolderInDataFolder() const
{
    return _requireUploadFolderInDataFolder;
}

//------------------------------------------------------------------------------
void FileUploadRouteSettings::setUploadFolder(const std::string& uploadFolder)
{
    _uploadFolder = uploadFolder;
}

//------------------------------------------------------------------------------
std::string FileUploadRouteSettings::getUploadFolder() const
{
    return _uploadFolder;
}

//------------------------------------------------------------------------------
void FileUploadRouteSettings::setAutoCreateUploadFolder(bool autoCreateUploadFolder)
{
    _autoCreateUploadFolder = autoCreateUploadFolder;
}

//------------------------------------------------------------------------------
bool FileUploadRouteSettings::getAutoCreateUploadFolder() const
{
    return _autoCreateUploadFolder;
}

//------------------------------------------------------------------------------
void FileUploadRouteSettings::setUploadRedirect(const std::string& uploadRedirect)
{
    _uploadRedirect = uploadRedirect;
}

//------------------------------------------------------------------------------
std::string FileUploadRouteSettings::getUploadRedirect() const
{
    return _uploadRedirect;
}

//------------------------------------------------------------------------------
void FileUploadRouteSettings::setWriteBufferSize(std::size_t writeBufferSize)
{
    _writeBufferSize = writeBufferSize;
}

//------------------------------------------------------------------------------
std::size_t FileUploadRouteSettings::getWriteBufferSize() const
{
    return _writeBufferSize;
}

//------------------------------------------------------------------------------
void FileUploadRouteSettings::setRequireValidContentType(bool requireValidContentType)
{
    _requireValidContentType = requireValidContentType;
}

//------------------------------------------------------------------------------
bool FileUploadRouteSettings::getRequireValidContentType() const
{
    return _requireValidContentType;
}

//------------------------------------------------------------------------------
void FileUploadRouteSettings::setValidContentTypes(const std::vector<Poco::Net::MediaType>& validContentTypes)
{
    _validContentTypes = validContentTypes;
}

//------------------------------------------------------------------------------
std::vector<Poco::Net::MediaType> FileUploadRouteSettings::getValidContentTypes() const
{
    return _validContentTypes;
}

//------------------------------------------------------------------------------
void FileUploadRouteSettings::setAutoRename(bool autoRename)
{
    _autoRename = autoRename;
}

//------------------------------------------------------------------------------
bool FileUploadRouteSettings::getAutoRename() const
{
    return _autoRename;
}


} } // namespace ofx::HTTP
