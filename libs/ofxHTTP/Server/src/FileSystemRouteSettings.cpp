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


#include "ofx/HTTP/Server/FileSystemRouteSettings.h"


namespace ofx {
namespace HTTP {


const std::string FileSystemRouteSettings::DEFAULT_DOCUMENT_ROOT = "DocumentRoot/";
const std::string FileSystemRouteSettings::DEFAULT_INDEX         = "index.html";


//------------------------------------------------------------------------------
FileSystemRouteSettings::FileSystemRouteSettings(const std::string& routePathPattern):
    BaseRouteSettings(routePathPattern),
    _defaultIndex(DEFAULT_INDEX),
    _documentRoot(DEFAULT_DOCUMENT_ROOT),
    _autoCreateDocumentRoot(false),
    _requireDocumentRootInDataFolder(true)
{
}

//------------------------------------------------------------------------------
FileSystemRouteSettings::~FileSystemRouteSettings()
{
}

//------------------------------------------------------------------------------
void FileSystemRouteSettings::setDefaultIndex(const std::string& defaultIndex)
{
    _defaultIndex = defaultIndex;
}

//------------------------------------------------------------------------------
std::string FileSystemRouteSettings::getDefaultIndex() const
{
    return _defaultIndex;
}

//------------------------------------------------------------------------------
void FileSystemRouteSettings::setDocumentRoot(const std::string& documentRoot)
{
    _documentRoot = documentRoot;
}

//------------------------------------------------------------------------------
std::string FileSystemRouteSettings::getDocumentRoot() const
{
    return _documentRoot;
}

//------------------------------------------------------------------------------
void FileSystemRouteSettings::setAutoCreateDocumentRoot(bool autoCreateDocumentRoot)
{
    _autoCreateDocumentRoot = autoCreateDocumentRoot;
}

//------------------------------------------------------------------------------
bool FileSystemRouteSettings::getAutoCreateDocumentRoot() const
{
    return _autoCreateDocumentRoot;
}

//------------------------------------------------------------------------------
void FileSystemRouteSettings::setRequireDocumentRootInDataFolder(bool requireDocumentRootInDataFolder)
{
    _requireDocumentRootInDataFolder = requireDocumentRootInDataFolder;
}

//------------------------------------------------------------------------------
bool FileSystemRouteSettings::getRequireDocumentRootInDataFolder() const
{
    return _requireDocumentRootInDataFolder;
}


} } // namespace ofx::HTTP
