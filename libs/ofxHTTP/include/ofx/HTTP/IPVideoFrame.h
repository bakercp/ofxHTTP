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

    void setWidth(std::size_t width);
    std::size_t getWidth() const;

    void setHeight(std::size_t height);
    std::size_t getHeight() const;

    void setFlipHorizontal(bool flipHorizontal);
    bool getFlipHorizontal() const;

    void setFlipVertical(bool flipVertical);
    bool getFlipVertical() const;

    void setQuality(ofImageQualityType quality);
    ofImageQualityType getQuality() const;

    enum
    {
        DEFAULT_WIDTH = 320,
        DEFAULT_HEIGHT = 240
    };

private:
    std::size_t _width;
    std::size_t _height;
    bool _flipHorizontal;
    bool _flipVertical;
    ofImageQualityType _quality;

};


class IPVideoFrame
{
public:
    IPVideoFrame(const IPVideoFrameSettings& settings,
                 uint64_t timestamp,
                 const ofBuffer& buffer);
    
    virtual ~IPVideoFrame();

    IPVideoFrameSettings getSettings() const;

    uint64_t getTimestamp() const;

    ofBuffer& getBuffer();

private:
    IPVideoFrameSettings _settings;
    uint64_t _timestamp;
    ofBuffer _buffer;
    
};


} } // namespace ofx::HTTP
