//// =============================================================================
////
//// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
////
//// Permission is hereby granted, free of charge, to any person obtaining a copy
//// of this software and associated documentation files (the "Software"), to deal
//// in the Software without restriction, including without limitation the rights
//// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//// copies of the Software, and to permit persons to whom the Software is
//// furnished to do so, subject to the following conditions:
////
//// The above copyright notice and this permission notice shall be included in
//// all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//// THE SOFTWARE.
////
//// =============================================================================
//
//
//#include "ofx/HTTP/Server/PostRouteSettings.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//    
//const std::string PostRouteSettings::DEFAULT_UPLOAD_ROUTE    = "/post";
//const std::string PostRouteSettings::DEFAULT_UPLOAD_FOLDER   = "uploads/";
//const std::string PostRouteSettings::DEFAULT_UPLOAD_REDIRECT = "uploaded.html";
//const std::size_t PostRouteSettings::DEFAULT_BUFFER_SIZE     = 8192;
//
//
//PostRouteSettings::PostRouteSettings(const std::string& routePathPattern):
//    BaseRouteSettings(routePathPattern),
//    _requireUploadFolderInDataFolder(true),
//    _uploadFolder(DEFAULT_UPLOAD_FOLDER),
//    _autoCreateUploadFolder(false),
//    _uploadRedirect(DEFAULT_UPLOAD_REDIRECT),
//    _writeBufferSize(DEFAULT_BUFFER_SIZE),
//    _requireValidContentType(false),
//    _autoRename(true)
//{
//}
//
//
//PostRouteSettings::~PostRouteSettings()
//{
//}
//
//
//void PostRouteSettings::setRequireUploadFolderInDataFolder(bool requireUploadFolderInDataFolder)
//{
//    _requireUploadFolderInDataFolder = requireUploadFolderInDataFolder;
//}
//
//
//bool PostRouteSettings::getRequireUploadFolderInDataFolder() const
//{
//    return _requireUploadFolderInDataFolder;
//}
//
//
//void PostRouteSettings::setUploadFolder(const std::string& uploadFolder)
//{
//    _uploadFolder = uploadFolder;
//}
//
//
//std::string PostRouteSettings::getUploadFolder() const
//{
//    return _uploadFolder;
//}
//
//
//void PostRouteSettings::setAutoCreateUploadFolder(bool autoCreateUploadFolder)
//{
//    _autoCreateUploadFolder = autoCreateUploadFolder;
//}
//
//
//bool PostRouteSettings::getAutoCreateUploadFolder() const
//{
//    return _autoCreateUploadFolder;
//}
//
//
//void PostRouteSettings::setUploadRedirect(const std::string& uploadRedirect)
//{
//    _uploadRedirect = uploadRedirect;
//}
//
//
//std::string PostRouteSettings::getUploadRedirect() const
//{
//    return _uploadRedirect;
//}
//
//
//void PostRouteSettings::setWriteBufferSize(std::size_t writeBufferSize)
//{
//    _writeBufferSize = writeBufferSize;
//}
//
//
//std::size_t PostRouteSettings::getWriteBufferSize() const
//{
//    return _writeBufferSize;
//}
//
//
//void PostRouteSettings::setRequireValidContentType(bool requireValidContentType)
//{
//    _requireValidContentType = requireValidContentType;
//}
//
//
//bool PostRouteSettings::getRequireValidContentType() const
//{
//    return _requireValidContentType;
//}
//
//
//void PostRouteSettings::setValidContentTypes(const std::vector<Poco::Net::MediaType>& validContentTypes)
//{
//    _validContentTypes = validContentTypes;
//}
//
//
//std::vector<Poco::Net::MediaType> PostRouteSettings::getValidContentTypes() const
//{
//    return _validContentTypes;
//}
//
//
//void PostRouteSettings::setAutoRename(bool autoRename)
//{
//    _autoRename = autoRename;
//}
//
//
//bool PostRouteSettings::getAutoRename() const
//{
//    return _autoRename;
//}
//
//
//} } // namespace ofx::HTTP
