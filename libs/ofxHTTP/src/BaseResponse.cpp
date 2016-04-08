// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/HTTP/BaseResponse.h"
#include "ofImage.h"


namespace ofx {
namespace HTTP {


BaseResponse::~BaseResponse()
{
}


BaseBufferedResponse::~BaseBufferedResponse()
{
}


std::string BaseBufferedResponse::error() const
{
    std::stringstream ss;

    if (getStatus() < 200 && getStatus() >= 300)
    {
        ss << "HTTPStatus: " << getStatus() << " Reason: " << getReason() << " ";
    }

    if (hasException())
    {
        ss << "Exception: " << _exception->displayText();
    }

    return ss.str();
}


const Poco::Exception* BaseBufferedResponse::exception() const
{
    return _exception.get();
}


bool BaseBufferedResponse::hasException() const
{
    return _exception != nullptr;
}

bool BaseBufferedResponse::isSuccess() const
{
    return !hasException() && getStatus() >= 200 && getStatus() < 300;
}


void BaseBufferedResponse::setException(std::unique_ptr<Poco::Exception>&& exception)
{
    _exception = std::move(exception);
}


BufferedResponse::~BufferedResponse()
{
}


const ofBuffer& BufferedResponse::data() const
{
    return _buffer;
}


ofPixels BufferedResponse::pixels() const
{
    ofPixels _pixels;
    
    if (!ofLoadImage(_pixels, _buffer))
    {
        ofLogError("BufferedResponse::pixels") << "Unable to interpret data as ofPixels.";
    }

    return _pixels;
}


ofJson BufferedResponse::json() const
{
    ofJson _json;

    try
    {
        _json = ofJson::parse(_buffer);
    }
    catch (const std::exception& exc)
    {
        ofLogError("BufferedResponse::json") << "Unable to interpret data as json: " << exc.what();
    }

    return _json;
}


void BufferedResponse::bufferStream(std::istream& responseStream)
{
    responseStream >> _buffer;
}


} } // namespace ofx::HTTP
