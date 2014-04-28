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


#include "ofx/HTTP/Client/BaseClient.h"
#include "Poco/InflatingStream.h"


namespace ofx {
namespace HTTP {
namespace Client {


const std::string BaseClient::ACCEPT_ENCODING_HEADER = "Accept-Encoding";
const std::string BaseClient::CONTENT_ENCODING_HEADER = "Content-Encoding";


BaseClient::BaseClient(AbstractSessionProvider& sessionProvider,
                       AbstractRequestResponseProcessor& proxyProcessor,
                       AbstractRequestResponseProcessor& authenticationProcessor,
                       AbstractRequestResponseProcessor& redirectProcessor):
    _isRunning(true),
    _sessionProvider(sessionProvider),
    _proxyProcessor(proxyProcessor),
    _authenticationProcessor(authenticationProcessor),
    _redirectProcessor(redirectProcessor)
    //, _pDecodedResponseStream(0)
{
}


BaseClient::~BaseClient()
{
//    delete _pDecodedResponseStream;
}


std::istream& BaseClient::execute(Client::BaseRequest& request,
                                  Client::BaseResponse& response,
                                  Context& context)
{
    context.setResolvedURI(Poco::URI(request.getURI()));

    _sessionProvider.createSession(context.getResolvedURI(), context);

    _proxyProcessor.processRequest(request, context);

    _authenticationProcessor.processRequest(request, context);

    processRequest(request, context);

    request.prepareRequest();

    // Set the headers indicating the encodings we can decode.
    // request.set(ACCEPT_ENCODING_HEADER, "gzip, deflate");

    std::ostream& requestStream = context.getSession()->sendRequest(request);

    request.writeRequestBody(requestStream);

    std::istream& responseStream = context.getSession()->receiveResponse(response);

    processResponse(request, response, context);

    Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();

    if (status == Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY   // 301
     || status == Poco::Net::HTTPResponse::HTTP_FOUND               // 302
     || status == Poco::Net::HTTPResponse::HTTP_SEE_OTHER           // 303
     || status == Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT) // 307
    {
        _redirectProcessor.handleResponse(request,
                                          response,
                                          context);

        return execute(request, response, context);
    }
    else if (status == Poco::Net::HTTPResponse::HTTP_USEPROXY)
    {
        _proxyProcessor.handleResponse(request, response, context);

        return execute(request, response, context);
    }
    else if (status == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
    {
        // consume because we are reusing the session.
        Utils::consume(responseStream);

        Utils::dumpHeaders(request, response);

        _authenticationProcessor.handleResponse(request,
                                                response,
                                                context);

        return execute(request, response, context);
    }
    else
    {
//        delete _pDecodedResponseStream;
//        _pDecodedResponseStream = 0;
//
//        if (response.has(CONTENT_ENCODING_HEADER))
//        {
//            std::string contentEncoding = response.get(CONTENT_ENCODING_HEADER);
//
//            if (0 == Poco::UTF8::icompare(contentEncoding, "gzip"))
//            {
//                _pDecodedResponseStream = new Poco::InflatingInputStream(responseStream, Poco::InflatingStreamBuf::STREAM_GZIP);
//                return *_pDecodedResponseStream;
//            }
//            else if (0 == Poco::UTF8::icompare(contentEncoding, "deflate"))
//            {
//                _pDecodedResponseStream = new Poco::InflatingInputStream(responseStream, Poco::InflatingStreamBuf::STREAM_ZLIB);
//                return *_pDecodedResponseStream;
//            }
//            else
//            {
//                ofLogWarning() << "Returning with unknown content encoding: " << contentEncoding;
//                return responseStream;
//            }
//        }
//        else
//        {
            return responseStream;
//        }
    }
}


bool BaseClient::isRunning() const
{
    return _isRunning;
}

    
void BaseClient::cancel()
{
    _isRunning = false;
}


} } } // namespace ofx::HTTP::Client
