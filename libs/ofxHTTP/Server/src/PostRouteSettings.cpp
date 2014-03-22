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


#include "ofx/HTTP/Server/PostRouteSettings.h"


namespace ofx {
namespace HTTP {

    
const std::string PostRouteSettings::DEFAULT_POST_ROUTE    = "/post";
const std::string PostRouteSettings::DEFAULT_POST_FOLDER   = "uploads/";
const std::string PostRouteSettings::DEFAULT_POST_REDIRECT = "uploaded.html";
const std::string PostRouteSettings::DEFAULT_POST_HTTP_METHODS_ARRAY[] = { "POST" };
const BaseRouteSettings::HTTPMethodSet PostRouteSettings::DEFAULT_POST_HTTP_METHODS(INIT_SET_WITH_ARRAY(DEFAULT_POST_HTTP_METHODS_ARRAY));


PostRouteSettings::PostRouteSettings(const std::string& routePathPattern,
                                     bool requireSecurePort):
    BaseRouteSettings(routePathPattern,
                      requireSecurePort,
                      DEFAULT_POST_HTTP_METHODS),
    _requireUploadFolderInDataFolder(true),
    _uploadFolder(DEFAULT_POST_FOLDER),
    _autoCreateUploadFolder(false),
    _uploadRedirect(DEFAULT_POST_REDIRECT),
    _writeBufferSize(DEFAULT_POST_BUFFER_SIZE),
    _fieldLimit(DEFAULT_FIELD_LIMIT),
    _maximumFileUploadSize(DEFAULT_MAXIMUM_FILE_UPLOAD_SIZE)
{
}


PostRouteSettings::~PostRouteSettings()
{
}


void PostRouteSettings::setUploadFolder(const std::string& uploadFolder)
{
    _uploadFolder = uploadFolder;
}


const std::string& PostRouteSettings::getUploadFolder() const
{
    return _uploadFolder;
}


void PostRouteSettings::setUploadRedirect(const std::string& uploadRedirect)
{
    _uploadRedirect = uploadRedirect;
}


const std::string& PostRouteSettings::getUploadRedirect() const
{
    return _uploadRedirect;
}


void PostRouteSettings::setWriteBufferSize(std::size_t writeBufferSize)
{
    _writeBufferSize = writeBufferSize;
}


std::size_t PostRouteSettings::getWriteBufferSize() const
{
    return _writeBufferSize;
}


void PostRouteSettings::setFieldLimit(std::size_t fieldLimit)
{
    _fieldLimit = fieldLimit;
}


std::size_t PostRouteSettings::getFieldLimit() const
{
    return _fieldLimit;
}


void PostRouteSettings::setMaximumFileUploadSize(std::streamsize maximumFileUploadSize)
{
    _maximumFileUploadSize = maximumFileUploadSize;
}


std::streamsize PostRouteSettings::getMaximumFileUploadSize() const
{
    return _maximumFileUploadSize;
}


} } // namespace ofx::HTTP
