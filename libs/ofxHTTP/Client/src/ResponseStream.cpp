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


#include "ofx/HTTP/Client/ResponseStream.h"
#include "Poco/CountingStream.h"
#include "Poco/TeeStream.h"


namespace ofx {
namespace HTTP {
namespace Client {


ResponseStream::ResponseStream(Poco::Net::HTTPResponse& httpResponseRef,
                               Poco::Net::HTTPResponseStream* pResponseStream,
                               Poco::Exception* pException):
    _httpResponseRef(httpResponseRef),
    _pResponseStream(pResponseStream),
    _pException(pException)
{
}


ResponseStream::~ResponseStream()
{
    // deleting a null pointer is a noop
    delete _pResponseStream; // cleans up the stream and the backing session
    _pResponseStream = 0;

    delete _pException;
    _pException = 0;
}


bool ResponseStream::hasResponseStream() const
{
    return 0 != _pResponseStream;
}


bool ResponseStream::hasException() const
{
    return 0 != _pException;
}


Poco::Exception* ResponseStream::getException() const
{
    return _pException;
}


std::istream& ResponseStream::getResponseStreamRef()
{
    return *_pResponseStream;
}


ResponseStream::SharedPtr ResponseStream::createResponseStream(BaseRequest& httpRequest,
                                                               Context::SharedPtr context)
{

    // init default client ssl context if none exists
    ofSSLManager::initializeClient();

    HTTPResponse httpResponse;

    SessionSettings& sessionSettings = context->getSessionSettingsRef(); 
    CredentialStore& credentials     = context->getCredentialStoreRef();
    CookieStore&     cookies         = context->getCookieStoreRef();

    int redirects = 0;

    try
    {
        ofLogVerbose("ResponseStream::createResponseStream") << "Beginning redirect loop. Max redirects = " << context->getSessionSettingsRef().getMaxRedirects() ;

        Poco::URI redirectedProxyUri;

        Poco::URI requestURI(httpRequest.getURI());

        while (redirects < sessionSettings.getMaxRedirects())
        {

            Poco::Net::HTTPClientSession* pClientSession = 0;
            bool proxyRedirectRequested = false;
            bool authenticationRequested = false;
            // bool authenticationAttempted = false;

            ofLogVerbose("ResponseStream::createResponseStream") << "Beginning retry loop : redirect # " << redirects;

            do
            {
                // pClientSession will be deleted if proxy is redirected
                if (0 == pClientSession)
                {
                    ofLogVerbose("ResponseStream::createResponseStream") << "CREATING NEW CLIENT ";

                    if(requestURI.getScheme() == "https")
                    {
                        pClientSession = new Poco::Net::HTTPSClientSession(requestURI.getHost(),
                                                                           requestURI.getPort());
                    }
                    else
                    {
                        pClientSession = new Poco::Net::HTTPClientSession(requestURI.getHost(),
                                                                          requestURI.getPort());
                    }

//                    pClientSession->setTimeout(Poco::Timespan(20, 0));
//                    pClientSession->setKeepAliveTimeout(Poco::Timespan(20, 0));
//                    pClientSession->setKeepAlive(true);

                    ofLogVerbose("ResponseStream::createResponseStream") << "New session created - host: " <<
                    pClientSession->getHost() << " port: " << pClientSession->getPort();

                    if(sessionSettings.isProxyEnabled())
                    {
                        if(redirectedProxyUri.empty())
                        {
                            ofLogVerbose("ResponseStream::createResponseStream") << "Using proxy " <<
                            sessionSettings.getProxyHost() << " @ " << sessionSettings.getProxyPort();

                            pClientSession->setProxyHost(sessionSettings.getProxyHost());
                            pClientSession->setProxyPort(sessionSettings.getProxyPort());
                        }
                        else
                        {
                            ofLogVerbose("ResponseStream::createResponseStream") << "Using redirected proxy " <<
                            redirectedProxyUri.getHost() << " @ " << redirectedProxyUri.getPort();

                            // use the redirected proxy url, rather than the one provided in settings
                            pClientSession->setProxyHost(redirectedProxyUri.getHost());
                            pClientSession->setProxyPort(redirectedProxyUri.getPort());
                        }

                        if(sessionSettings.hasProxyCredentials())
                        {
                            ofLogVerbose("ResponseStream::createResponseStream") << "With proxy username:password : " <<
                            sessionSettings.getProxyUsername() << ":" << sessionSettings.getProxyPassword();

                            pClientSession->setProxyUsername(sessionSettings.getProxyUsername());
                            pClientSession->setProxyPassword(sessionSettings.getProxyPassword());
                        }
                        else
                        {
                            ofLogVerbose("ResponseStream::createResponseStream") << "Without proxy credentials.";
                        }
                    }

                }
                else
                {
                    ofLogVerbose("ResponseStream::createResponseStream") <<
                        "Using existing session - host: " <<
                        pClientSession->getHost() <<
                        " port: " << pClientSession->getPort();
                }


                std::string path = requestURI.getPathAndQuery();

                if(path.empty())
                {
                    path = "/";
                }

                ofLogVerbose("ResponseStream::createResponseStream") << "Using path: " << path;

                // apply defaults from the session first
                // TODO: default headers could also include useragent, etc ...
                if(sessionSettings.hasDefaultHeaders())
                {
                    ofLogVerbose("ResponseStream::createResponseStream") << "Writing default headers:";
                    Poco::Net::NameValueCollection defaultHeaders = sessionSettings.getDefaultHeaders();
                    Poco::Net::NameValueCollection::ConstIterator iter = defaultHeaders.begin();

                    while(iter != defaultHeaders.end())
                    {
                        ofLogVerbose("ResponseStream::createResponseStream") << (*iter).first << "=" << (*iter).second;
                        httpRequest.set((*iter).first, (*iter).second);
                        ++iter;
                    }
                }

                // TODO: this getting and setting user agent should be intelligently placed in the default headers
                if(!sessionSettings.getUserAgent().empty())
                {
                    ofLogVerbose("ResponseStream::createResponseStream") << "Setting user agent to: " << sessionSettings.getUserAgent();
                    httpRequest.set("User-Agent", sessionSettings.getUserAgent());
                }

                ////// ALL OF THIS NEEDS TO GO ELSEWHERE -- maybe inside the context?
                if(authenticationRequested)
                {
                    credentials.authenticate(*pClientSession, httpRequest, httpResponse);
                }
                else
                {
                    credentials.authenticate(*pClientSession, httpRequest);
                }


                /// Finalize the request
                httpRequest.finalizeRequest();

                // call back into the request to pull the request data
                ofLogVerbose("ResponseStream::createResponseStream") << "Preparing request.";

                NameValueCollection::ConstIterator iter = httpRequest.begin();

                cout << "HEADERS-----" << endl;
                while (iter != httpRequest.end())
                {
                    cout << (*iter).first << " = " << (*iter).second << endl;
                    ++iter;
                }
                cout << "HEADERS-----" << endl;

                cout << "URI: " << requestURI.toString() << endl;

                
                //////////////////////////////////////////////////////////////////
                /////////////////////// -- SEND REQUEST -- ///////////////////////
                //////////////////////////////////////////////////////////////////


                ofLogVerbose("ResponseStream::createResponseStream") << "Sending request.";

                std::ostream& requestStream = pClientSession->sendRequest(httpRequest);

                Poco::TeeOutputStream tee(requestStream);

                Poco::CountingOutputStream cos;

                tee.addStream(cos);
                tee.addStream(std::cout);

                ofLogVerbose("ResponseStream::createResponseStream") << "Sending request body.";

                httpRequest.writeRequestBody(tee); // upload, put etc

                cout << "numchars written = " <<  cos.chars() << endl;

                //////////////////////////////////////////////////////////////////
                ///////////////////////// -- RESPONSE -- /////////////////////////
                //////////////////////////////////////////////////////////////////
                ofLogVerbose("ResponseStream::createResponseStream") << "Receiving response.";
                std::istream& responseStream = pClientSession->receiveResponse(httpResponse);

                cookies.store(httpResponse);


                if (httpResponse.getStatus() == HTTPResponse::HTTP_MOVED_PERMANENTLY) // 301
                {
                    // Set it for the next go-around.
                    httpRequest.setURI(requestURI.toString());

                    ++redirects;

                    delete pClientSession;
                    pClientSession = 0;

                    ofLogVerbose("ResponseStream::createResponseStream") << "Redirecting to: " << requestURI.toString();
                }
                else if (httpResponse.getStatus() == HTTPResponse::HTTP_FOUND // 302
                      || httpResponse.getStatus() == HTTPResponse::HTTP_SEE_OTHER // 303
                      || httpResponse.getStatus() == HTTPResponse::HTTP_TEMPORARY_REDIRECT) // 307
                {
                    httpRequest.add("Referrer", requestURI.toString());
                    httpRequest.setContentType("");

                    requestURI.resolve(httpResponse.get("Location"));

                    // Set it for the next go-around.
                    httpRequest.setURI(requestURI.toString());

                    // TODO: TAKE CARE OF THE HEADERS ... CLEAN THEM OUT
                    httpRequest.setMethod(Poco::Net::HTTPRequest::HTTP_GET);

                    ++redirects;

                    delete pClientSession;
                    pClientSession = 0;

                    ofLogVerbose("ResponseStream::createResponseStream") << "Redirecting to: " << requestURI.toString();

                }
                else if (httpResponse.getStatus() == HTTPResponse::HTTP_OK)
                {
                    ofLogVerbose("ResponseStream::createResponseStream") << "Got valid stream, returning.";
                    return ResponseStream::makeShared(httpResponse,
                                                      new Poco::Net::HTTPResponseStream(responseStream,
                                                                                        pClientSession),
                                                      0); // No exception.
                }
                else if (httpResponse.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED
                         && !authenticationRequested)
                {
                    authenticationRequested = true;

                    Poco::NullOutputStream nos;

                    cout << endl;

                    std::streamsize n = Poco::StreamCopier::copyStream(responseStream, cout); // consume

                    cout << endl;
                    ofLogVerbose("ResponseStream::createResponseStream") <<
                    "Got HTTP_UNAUTHORIZED, trying to authorize with response this time. (" << n << " bytes consumed)";

                }
                else if (httpResponse.getStatus() == HTTPResponse::HTTP_USEPROXY &&
                         !proxyRedirectRequested)
                {
                    // The requested resource MUST be accessed through the proxy
                    // given by the Location field. The Location field gives the
                    // URI of the proxy. The recipient is expected to repeat this
                    // single request via the proxy. 305 responses MUST only be generated by origin servers.
                    // only use for one single request!
                    redirectedProxyUri.resolve(httpResponse.get("Location"));

                    delete pClientSession;
                    pClientSession = 0;

                    proxyRedirectRequested = true; // only allow useproxy once

                    Poco::NullOutputStream nos;

                    cout << endl;

                    std::streamsize n = Poco::StreamCopier::copyStream(responseStream, cout); // consume

                    cout << endl;

                    ofLogVerbose("ResponseStream::createResponseStream") <<
                    "Got HTTP_USEPROXY, trying to use redirected proxy. (" << n << " bytes consumed)";

                }
                else
                {
                    ofLogVerbose("ResponseStream::createResponseStream") << "Received unhandled response " <<
                    httpResponse.getStatus() << " b/c " << httpResponse.getReason();
                    return ResponseStream::makeShared(httpResponse,
                                                      new Poco::Net::HTTPResponseStream(responseStream,
                                                                                        pClientSession),
                                                      new Poco::Exception(httpResponse.getReason(),
                                                                          requestURI.toString()));
                }

            }
            while (authenticationRequested || proxyRedirectRequested);
        }
    }
    catch(const Poco::SyntaxException& exc)
    {
        ofLogVerbose("ResponseStream::createResponseStream") << "Got Syntax exception " << exc.displayText();
        return ResponseStream::makeShared(httpResponse,
                                          0,
                                          exc.clone());
    }
    catch(const Poco::Net::HostNotFoundException& exc)
    {
        ofLogVerbose("ResponseStream::createResponseStream") << "Got exception " << exc.displayText();
        return ResponseStream::makeShared(httpResponse,
                                          0,
                                          exc.clone());
    }
    catch(const Poco::Exception& exc)
    {
        ofLogVerbose("ResponseStream::createResponseStream") << "Got exception " << exc.displayText();
        return ResponseStream::makeShared(httpResponse,
                                          0,
                                          exc.clone());
    }
    catch(...)
    {
        ofLogVerbose("ResponseStream::createResponseStream") << "Got unknown exception";
        return ResponseStream::makeShared(httpResponse,
                                          0,
                                          new Poco::Exception("Unknown exception"));
    }
    
    return ResponseStream::makeShared(httpResponse,
                                      0,
                                      new Poco::IOException("Too many redirects while opening URI", httpRequest.getURI()));
    
}


} } } // namespace ofx::HTTP::Client
