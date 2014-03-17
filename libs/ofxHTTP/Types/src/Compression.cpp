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


#include "Compression.h"


namespace ofx {
namespace HTTP {


CompressorEntry::CompressorEntry(const Poco::Net::MediaType& mediaType):
    _mediaType(mediaType)
{
}


CompressorEntry::~CompressorEntry()
{
}


void CompressorEntry::addCompressionType(Compression::Type compressionType)
{
    removeCompressionType(compressionType);
    _validCompressionTypes.push_back(compressionType);
}


void CompressorEntry::removeCompressionType(Compression::Type compressionType)
{
    remove(_validCompressionTypes.begin(),
           _validCompressionTypes.end(),
           compressionType);
}


void CompressorEntry::clearCompressionTypes()
{
    _validCompressionTypes.clear();
}


Poco::Net::MediaType CompressorEntry::getMediaType() const
{
    return _mediaType;
}


std::vector<Compression::Type> CompressorEntry::getValidCompressionTypes() const
{
    return _validCompressionTypes;
}


std::string CompressorEntry::getValidCompressionTypesAsCSV() const
{
    std::string outputString;
    std::vector<std::string> compressionTypes;
    std::vector<Compression::Type>::const_iterator iter = _validCompressionTypes.begin();
    while(iter != _validCompressionTypes.end())
    {
        switch(*iter)
        {
            case Compression::DEFLATE:
                outputString += "deflate";
                break;
            case Compression::GZIP:
                outputString += "gzip";
                break;
            default:
                break;
        }

        if(iter + 1 != _validCompressionTypes.end())
        {
            outputString += ", ";
        }

        ++iter;
    }

    return outputString;
}


} } // namespace ofx::HTTP
