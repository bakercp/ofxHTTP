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


#pragma once


#include "Poco/Net/MediaType.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "ofEvents.h"


namespace ofx {
namespace HTTP {


class BaseServerEvent: public ofEventArgs
{
public:
    typedef Poco::Net::NameValueCollection Headers;
    typedef Poco::Net::NameValueCollection::ConstIterator HeadersIter;

//    BaseServerEvent(const Headers& headers):
//        _headers(headers)
//    {
//    }
//
//
//    Headers getHeaders() const
//    {
//        return _headers;
//    }


//    std::size_t getContentLength() const
//    {
//        HeadersIter iter = _headers.find("Content-Length");
//
//        if (iter != _headers.end())
//        {
//            std::istringstream buffer((*iter).second);
//            std::size_t result;
//            buffer >> result;
//            return result;
//        }
//        else
//        {
//            return 0;
//        }
//    }
//
//
//    Poco::Net::MediaType getContentType() const
//    {
//        HeadersIter iter = _headers.find("Content-Type");
//
//        if (iter != _headers.end())
//        {
//            return Poco::Net::MediaType((*iter).second);
//        }
//        else
//        {
//            return Poco::Net::MediaType("text/plain");
//        }
//    }

protected:
    Headers _headers;

};




} } // namespace ofx::HTTP
