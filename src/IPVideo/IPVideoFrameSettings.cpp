//
//  IPVideoFrameSettings.cpp
//  example
//
//  Created by Christopher P. Baker on 10/9/13.
//
//

#include "IPVideoFrameSettings.h"


namespace ofx {
namespace HTTP {


const std::size_t IPVideoFrameSettings::DEFAULT_WIDTH = 320;
const std::size_t IPVideoFrameSettings::DEFAULT_HEIGHT = 240;


IPVideoFrameSettings::IPVideoFrameSettings():
    _width(320),
    _height(240),
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

std::size_t IPVideoFrameSettings::getWidth()
{
    return _width;
}

void IPVideoFrameSettings::setHeight(std::size_t height)
{
    _height = height;
}
std::size_t IPVideoFrameSettings::getHeight()
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


} } // namespace ofx::HTTP
