// =============================================================================
//
// Copyright (c) 2012-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/IPVideoFrame.h"


namespace ofx {
namespace HTTP {


IPVideoFrameSettings::IPVideoFrameSettings():
    _width(DEFAULT_WIDTH),
    _height(DEFAULT_HEIGHT),
    _flipHorizontal(false),
    _flipVertical(false),
    _quality(OF_IMAGE_QUALITY_MEDIUM)
{
}


IPVideoFrameSettings::~IPVideoFrameSettings()
{
}


void IPVideoFrameSettings::setWidth(std::size_t width)
{
    _width = width;
}


std::size_t IPVideoFrameSettings::getWidth() const
{
    return _width;
}


void IPVideoFrameSettings::setHeight(std::size_t height)
{
    _height = height;
}


std::size_t IPVideoFrameSettings::getHeight() const
{
    return _height;
}


void IPVideoFrameSettings::setFlipHorizontal(bool flipHorizontal)
{
    _flipHorizontal = flipHorizontal;
}


bool IPVideoFrameSettings::getFlipHorizontal() const
{
    return _flipHorizontal;
}


void IPVideoFrameSettings::setFlipVertical(bool flipVertical)
{
    _flipVertical = flipVertical;
}


bool IPVideoFrameSettings::getFlipVertical() const
{
    return _flipVertical;
}


void IPVideoFrameSettings::setQuality(ofImageQualityType quality)
{
    _quality = quality;
}


ofImageQualityType IPVideoFrameSettings::getQuality() const
{
    return _quality;
}

        
IPVideoFrame::IPVideoFrame(const IPVideoFrameSettings& settings,
                           uint64_t timestamp,
                           const ofBuffer& buffer):
    _settings(settings),
    _timestamp(timestamp),
    _buffer(buffer)
{
}


IPVideoFrame::~IPVideoFrame()
{
}


IPVideoFrameSettings IPVideoFrame::getSettings() const
{
    return _settings;
}


uint64_t IPVideoFrame::getTimestamp() const
{
    return _timestamp;
}


ofBuffer& IPVideoFrame::getBuffer()
{
    return _buffer;
}


} } // namespace ofx::HTTP
