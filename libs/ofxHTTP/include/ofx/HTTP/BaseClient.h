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


#pragma once


#include "Poco/TeeStream.h"
#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/ClientProgressStream.h"
#include "ofx/IO/ByteBuffer.h"
#include "ofx/IO/ByteBufferStream.h"

//#include "ofx/HTTP/BaseRequest.h"
//#include "ofx/HTTP/BaseResponse.h"


namespace ofx {
namespace HTTP {


/// \brief A base HTTP cient for executing HTTP client requests.
class BaseClient:
    public AbstractRequestStreamListener,
    public AbstractResponseStreamListener
{
public:
    /// \brief Create a generic BaseClient.
    BaseClient();

    /// \brief Create a BaseClient with the given filters.
    /// \param requestFilters A collection of request header filters.
    /// \param responseFilters A collection of response header filters.
    BaseClient(std::vector<AbstractRequestFilter*> requestFilters,
               std::vector<AbstractResponseFilter*> responseFilters);

    /// \brief Destroy the BaseClient.
    virtual ~BaseClient();

    /// \brief Execute a request and get the raw input stream.
    ///
    /// This is the workhorse function for all client/server interactions. All
    /// functions eventually call this function for exectution.
    ///
    /// \param request The request to execute.
    /// \param response The response data.
    /// \param context The context data used for the transaction.
    /// \throws Various exceptions.
    std::istream& execute(BaseRequest& request,
                          BaseResponse& response,
                          Context& context);


    template <typename RequestType>
    std::unique_ptr<BufferedResponse<RequestType>> executeBuffered(std::unique_ptr<RequestType> request)
    {
        return executeBuffered<RequestType, BufferedResponse<RequestType>>(std::move(request));
    }

    
    template <typename RequestType, typename ResponseType>
    std::unique_ptr<ResponseType> executeBuffered(std::unique_ptr<RequestType> request)
    {
        auto response = std::make_unique<ResponseType>(std::move(request),
                                                       std::move(createContext()));

        try
        {
            // The response now owns the request and context, so we use it from there.
            response->bufferStream(execute(response->request(),
                                           *response,
                                           response->context()));
        }
        catch (const Poco::Exception& exc)
        {
            auto exception = std::make_unique<Poco::Exception>(exc);
            response->setException(std::move(exception));
        }
        catch (const std::exception& exc)
        {
            auto exception = std::make_unique<Poco::Exception>(exc.what());
            response->setException(std::move(exception));
        }
        catch (...)
        {
            auto exception = std::make_unique<Poco::Exception>("Unknown exception in BaseClient::execute.");
            response->setException(std::move(exception));
        }

        return response;
    }

    /// \brief Submit a request and get the results via callback.
    ///
    /// Listeners are should subscribe to the events in the ClientEvents member.
    /// Exceptions will be passed as errors to the event listeners. Filter
    /// events, response events and progress events will also be broadcast to
    /// event listeners. See ClientEvents for more.
    ///
    /// \param request The request to execute.
    /// \param response The response data.
    /// \param context The context data used during the submission.
    void submit(BaseRequest& request,
                BaseResponse& response,
                Context& context);

    /// \brief Submit a request and get the results via callback.
    ///
    /// Listeners are should subscribe to the events in the ClientEvents member.
    /// Exceptions will be passed as errors to the event listeners. Filter
    /// events, response events and progress events will also be broadcast to
    /// event listeners. See ClientEvents for more.
    ///
    /// \param request The request to execute.
    /// \param response The response data.
    void submit(BaseRequest& request,
                BaseResponse& response);

    /// \brief Add a request header filter.
    ///
    /// Request filters are applied to the BaseRequest in the order they are
    /// are added. Request filters are often used to introduce additional header
    /// or other client information such as authentication credentials.
    ///
    /// The client does not own the request filter, but merely holds a pointer.
    /// Request filter designers must ensure thread safety if multiple clients
    /// share the same filter.
    ///
    /// \param filter The request filter to add.
    void addRequestFilter(AbstractRequestFilter* filter);

    /// \brief Add a response header filter.
    ///
    /// Response filters are applied to the BaseResponse in the order they are
    /// are added. Response filters are often used to introduce additional
    /// header data or analyze the response information to extract client
    /// information such as cookies or credentials.
    ///
    /// The client does not own the response filter, but merely holds a pointer.
    /// Response filter designers must ensure thread safety if multiple clients
    /// share the same filter.
    ///
    /// \param filter The response header filter to add.
    void addResponseFilter(AbstractResponseFilter* filter);

    /// \brief Remove the given request header filter.
    /// \param filter The filter to remove.
    void removeRequestFilter(AbstractRequestFilter* filter);

    /// \brief Remove the given response header filter.
    /// \param filter The filter to remove.
    void removeResponseFilter(AbstractResponseFilter* filter);

    /// \brief Set the request stream filter.
    ///
    /// Clients are permitted to allow a single request stream filter. This
    /// request stream filter allows the user to filter and potentially modify
    /// the request data stream. It is often used to apply data compression or
    /// keep keep track of upload progress.
    ///
    /// Only one stream filter is permitted at a time. Users who need additional
    /// functionality can create more complex filters that combine the work of
    /// multiple filters.
    ///
    /// The client does not own the request stream filter, but merely holds a
    /// pointer. Response stream filter designers must ensure thread safety if
    /// multiple clients share the same filter.
    ///
    /// Setting a request stream filter will replace any existing filter.
    ///
    /// \param filter The request stream filter.
    void setRequestStreamFilter(AbstractRequestStreamFilter* filter);

    /// \brief Set the response stream filter.
    ///
    /// Clients are permitted to allow a single response stream filter. This
    /// response stream filter allows the user to filter and potentially modify
    /// the respnse data stream. It is often used to apply data decompression or
    /// keep keep track of download progress.
    ///
    /// Only one stream filter is permitted at a time. Users who need additional
    /// functionality can create more complex filters that combine the work of
    /// multiple filters.
    ///
    /// The client does not own the response stream filter, but merely holds a
    /// pointer. Response stream filter designers must ensure thread safety if
    /// multiple clients share the same filter.
    ///
    /// Setting a response stream filter will replace any existing filter.
    ///
    /// \param filter The response stream filter.
    void setResponseStreamFilter(AbstractResponseStreamFilter* filter);

    /// \brief Remove the current request stream filter.
    void removeRequestStreamFilter();

    /// \brief Remove the current request stream filter.
    void removeResponseStreamFilter();

    /// \brief Set the client session settings.
    ///
    /// A copy of these settings are passed with the session Context.
    ///
    /// \param sessionSettings The client session settings to set.
    void setSessionSettings(const ClientSessionSettings& sessionSettings);

    /// \brief Get the this client's session settings.
    /// \returns a reference to theis client's session settings.
    ClientSessionSettings& getSessionSettings();

    /// \brief This client's session settings.
    /// \returns a reference to theis client's session settings.
    const ClientSessionSettings& getSessionSettings() const;

    /// \brief A collection of client events.
    ClientEvents events;

    /// \brief Create a client context to use for a request.
    virtual std::unique_ptr<Context> createContext();

protected:
    /// \brief A list of request filters.
    ///
    /// These filters are automatically applied to the outgoing request headers.
    std::vector<AbstractRequestFilter*> _requestFilters;

    /// \brief A list of response filters.
    ///
    /// These filters are automatically applied to the incoming response headers.
    std::vector<AbstractResponseFilter*> _responseFilters;

    /// \brief This client's session settings.
    ClientSessionSettings _sessionSettings;

private:
    /// \brief An internal function that applies the request filters.
    /// \param request The request to filter.
    /// \param context The context of the request.
    void _filterRequest(BaseRequest& request, Context& context);

    /// \brief An internal function that applies the response filters.
    /// \param request The request.
    /// \param response The response to filter.
    /// \param context The context of the request.
    void _filterResponse(BaseRequest& request,
                         BaseResponse& response,
                         Context& context);

    /// \brief An internal function that establishes a connection.
    /// \param request The filtered request.
    /// \param context The context of the request.
    /// \returns a reference to an output stream to write to.
    std::ostream& _send(BaseRequest& request, Context& context);

    /// \brief An internal function that receives a response.
    /// \param request The submitted request.
    /// \param response The response headers generated.
    /// \param context The context of the request.
    /// \returns a reference to an input stream to be read.
    std::istream& _receive(BaseRequest& request,
                           BaseResponse& response,
                           Context& context);

    void progress(const BaseRequest& request,
                  Context& context,
                  std::streamsize totalBytesTransferred) override;

    void progress(const BaseRequest& request,
                  const BaseResponse& response,
                  Context& context,
                  std::streamsize totalBytesTransferred) override;

    /// \brief A pointer to the client progress request output stream.
    std::unique_ptr<std::ostream> _pClientProgressRequestStream = nullptr;

    /// \brief A pointer to the client progress request input stream.
    std::unique_ptr<std::istream> _pClientProgressResponseStream = nullptr;

    /// \brief A pointer to the request stream filter.
    AbstractRequestStreamFilter* _pRequestStreamFilter = nullptr;

    /// \brief A pointer to the response stream filter.
    AbstractResponseStreamFilter* _pResponseStreamFilter = nullptr;
    
};


} } // namespace ofx::HTTP
