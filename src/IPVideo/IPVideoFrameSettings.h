// =============================================================================
//
// Copyright (c) 2012-2013 Christopher Baker <http://christopherbaker.net>
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


#include "ofImage.h"


namespace ofx {
namespace HTTP {


class IPVideoFrameSettings
{
public:
    IPVideoFrameSettings();
    virtual ~IPVideoFrameSettings();

    void setWidth(std::size_t width);
    std::size_t getWidth();

    void setHeight(std::size_t height);
    std::size_t getHeight();

    void setFlipHorizontal(bool flipHorizontal);
    bool getFlipHorizontal() const;

    void setFlipVertical(bool flipVertical);
    bool getFlipVertical() const;

    void setQuality(ofImageQualityType quality);
    ofImageQualityType getQuality() const;

    static const std::size_t DEFAULT_WIDTH;
    static const std::size_t DEFAULT_HEIGHT;

private:
    std::size_t _width;
    std::size_t _height;
    bool _flipHorizontal;
    bool _flipVertical;
    ofImageQualityType _quality;
};


} } // namespace ofx::HTTP
