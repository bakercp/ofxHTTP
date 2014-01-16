//// =============================================================================
////
//// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
////
//// Permission is hereby granted, free of charge, to any person obtaining a copy
//// of this software and associated documentation files (the "Software"), to deal
//// in the Software without restriction, including without limitation the rights
//// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//// copies of the Software, and to permit persons to whom the Software is
//// furnished to do so, subject to the following conditions:
////
//// The above copyright notice and this permission notice shall be included in
//// all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//// THE SOFTWARE.
////
//// =============================================================================
//
//
//#include "ofx/HTTP/Server/PostRoute.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
//const Poco::Net::MediaType PostRoute::POST_CONTENT_TYPE_MULTIPART  = Poco::Net::MediaType("multipart/form-data");
//const Poco::Net::MediaType PostRoute::POST_CONTENT_TYPE_TEXT_PLAIN = Poco::Net::MediaType("text/plain");
//const Poco::Net::MediaType PostRoute::POST_CONTENT_TYPE_URLENCODED = Poco::Net::MediaType("application/x-www-form-urlencoded");
//
//
//PostRoute::PostRoute(const Settings& settings):
//    _settings(settings)
//{
//}
//
//
//PostRoute::~PostRoute()
//{
//}
//
//
//std::string PostRoute::getRoutePathPattern() const
//{
//    return _settings.getRoutePathPattern();
//}
//
//
//bool PostRoute::canHandleRequest(const Poco::Net::HTTPServerRequest& request,
//                                 bool isSecurePort) const
//{
//    // require a POST request
//    if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST) return false;
//
//    // require a Content-Type header
//    if (!request.has("Content-Type")) return false;
//
//    // get the content type header
//    Poco::Net::MediaType contentType(request.get("Content-Type"));
//
//    // require the Content-Type to match a typical form "enctype"
//    return contentType.matches(POST_CONTENT_TYPE_URLENCODED) ||
//           contentType.matches(POST_CONTENT_TYPE_MULTIPART)  ||
//           contentType.matches(POST_CONTENT_TYPE_TEXT_PLAIN);
//}
//
//
//Poco::Net::HTTPRequestHandler* PostRoute::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
//{
//    return new PostRouteHandler(*this);
//}
//
//
//PostRouteEvents& PostRoute::getEventsRef()
//{
//    return events;
//}
//
//
//} } // namespace ofx::HTTP
