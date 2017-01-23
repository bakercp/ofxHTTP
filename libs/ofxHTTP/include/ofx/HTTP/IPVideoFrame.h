//
// Copyright (c) 2012 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofFileUtils.h"
#include "ofImage.h"


namespace ofx {
namespace HTTP {


class IPVideoFrameSettings
{
public:
    IPVideoFrameSettings();
    virtual ~IPVideoFrameSettings();

    void setWidth(int width);
    int getWidth() const;

    void setHeight(int height);
    int getHeight() const;

    void setFlipHorizontal(bool flipHorizontal);
    bool getFlipHorizontal() const;

    void setFlipVertical(bool flipVertical);
    bool getFlipVertical() const;

    void setQuality(ofImageQualityType quality);
    ofImageQualityType getQuality() const;

    enum
    {
        NO_RESIZE = -1,
    };

private:
    std::size_t _width = NO_RESIZE;
    std::size_t _height = NO_RESIZE;
    bool _flipHorizontal = false;
    bool _flipVertical = false;
    ofImageQualityType _quality = OF_IMAGE_QUALITY_BEST;

};


class IPVideoFrame
{
public:
    IPVideoFrame(const IPVideoFrameSettings& settings,
                 uint64_t timestamp,
                 const ofBuffer& buffer);
    
    virtual ~IPVideoFrame();

    const IPVideoFrameSettings& settings() const;

    uint64_t timestamp() const;

    ofBuffer& buffer();

private:
    IPVideoFrameSettings _settings;
    uint64_t _timestamp;
    ofBuffer _buffer;
    
};


} } // namespace ofx::HTTP
