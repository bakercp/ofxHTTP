//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/Response.h"
#include "Poco/Net/MediaType.h"


namespace ofx {
namespace HTTP {


const std::string Response::CONTENT_RANGE = "Content-Range";
const std::string Response::BYTES_UNIT = "bytes";


Response::~Response()
{
}

    
int64_t Response::estimatedContentLength() const
{
    int64_t contentLength = getContentLength64();

    // If needed, try to calculate it based on the form data.
    if (contentLength == UNKNOWN_CONTENT_LENGTH)
    {
        // We only support a very specific content range in the form:
        // Content-Range: bytes 0-4068/4069
        // This could be expanded using regex, e.g.
        // https://code.google.com/p/googleappengine/source/browse/trunk/java/src/main/com/google/appengine/api/blobstore/ByteRange.java
        auto tokens = ofSplitString(get(CONTENT_RANGE, ""), " ", true, true);

        if (tokens.size() == 2 && tokens[0] == BYTES_UNIT)
        {
            auto _entryTokens = ofSplitString(tokens[1], "/", true, true);

            if (_entryTokens.size() == 2)
            {
                auto _rangeTokens = ofSplitString(tokens[1], "-", true, true);

                if (_rangeTokens.size() == 2)
                {
                    int rangeStart = ofToInt(_rangeTokens[0]);
                    int rangeEnd = ofToInt(_rangeTokens[1]);

                    if (rangeEnd > rangeStart)
                    {
                        contentLength = rangeEnd - rangeStart + 1;
                    }
                }
            }
        }
    }
    
    return contentLength;
}


bool Response::hasResponseStream() const
{
    return _responseStream != nullptr;
}


std::istream& Response::stream()
{
    _bufferResponse();

    if (isBuffered())
    {
        return *_bbis;
    }
    else
    {
        return _responseStream->stream();
    }
}


bool Response::isBufferable() const
{
    return estimatedContentLength() != UNKNOWN_CONTENT_LENGTH;
}


bool Response::isBuffered() const
{
    return _bbis != nullptr;
}


ofBuffer Response::buffer()
{
    _bufferResponse();
    return ofBuffer(_buffer.getData(), _buffer.size());
}


IO::ByteBuffer Response::byteBuffer()
{
    _bufferResponse();
    return _buffer;
}


bool Response::isPixels() const
{
    Poco::Net::MediaType mediaType(getContentType());
    return mediaType.matches("image");
}


ofPixels Response::pixels()
{
    ofPixels pixels;

    if (!ofLoadImage(pixels, buffer()))
    {
        ofLogError("Response::pixels") << "Unable to load pixels.";
    }

    return pixels;
}


bool Response::isJson() const
{
    Poco::Net::MediaType mediaType(getContentType());

    return mediaType.matches("application", "json")
        || mediaType.matches("text", "json");
}


ofJson Response::json()
{
    ofJson json;

    try
    {
        stream() >> json;
    }
    catch (const std::exception& exc)
    {
        ofLogError("Response::json") << "Error parsing json: " << exc.what();
    }
    catch (...)
    {
        ofLogError("Response::json") << "Error parsing json.";
    }

    return json;
}

    
bool Response::isXml() const
{
    Poco::Net::MediaType mediaType(getContentType());

    return mediaType.matches("application", "xml")
        || mediaType.matches("text", "xml");
}


ofXml Response::xml()
{
    ofXml xml;

    if (!xml.parse(byteBuffer().toString()))
    {
        ofLogError("Response::xml") << "Unable to parse xml.";
    }

    return xml;
}


bool Response::toFile(const std::filesystem::path& path)
{
    return ofBufferToFile(path, buffer());
}


bool Response::isInformational() const
{
    return getStatus() >= 100
        && getStatus() <  200;
}


bool Response::isSuccess() const
{
    return getStatus() >= 200
        && getStatus() <  300;
}


bool Response::isRedirection() const
{
    return getStatus() >= 300
        && getStatus() <  400;
}


bool Response::isClientError() const
{
    return getStatus() >= 400
        && getStatus() <  500;
}


bool Response::isServerError() const
{
    return getStatus() >= 500;
}


std::string Response::statusAndReason() const
{
    return std::to_string(getStatus()) + ": " + getReason();
}


void Response::_bufferResponse()
{
    if (isBufferable())
    {
        if (!isBuffered())
        {
            _buffer.writeBytes(_responseStream->stream());
        }
        
        _bbis = std::make_unique<IO::ByteBufferInputStream>(_buffer);
    }
}



} } // namespace ofx::HTTP
