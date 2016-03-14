// =============================================================================
//
// Copyright (c) 2012-2016 Christopher Baker <http://christopherbaker.net>
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
