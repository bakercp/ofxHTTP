//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/IPVideoFrame.h"


namespace ofx {
namespace HTTP {


IPVideoFrameSettings::IPVideoFrameSettings()
{
}


IPVideoFrameSettings::~IPVideoFrameSettings()
{
}


void IPVideoFrameSettings::setWidth(int width)
{
    _width = width;
}


int IPVideoFrameSettings::getWidth() const
{
    return _width;
}


void IPVideoFrameSettings::setHeight(int height)
{
    _height = height;
}


int IPVideoFrameSettings::getHeight() const
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


const IPVideoFrameSettings& IPVideoFrame::settings() const
{
    return _settings;
}


uint64_t IPVideoFrame::timestamp() const
{
    return _timestamp;
}


ofBuffer& IPVideoFrame::buffer()
{
    return _buffer;
}


} } // namespace ofx::HTTP
