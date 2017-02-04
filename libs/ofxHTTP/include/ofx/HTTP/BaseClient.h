//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//

//
//#pragma once
//
//
//#include "ofx/HTTP/AbstractClientTypes.h"
//#include "ofx/HTTP/ClientEvents.h"
//#include "ofx/HTTP/ClientProgressStream.h"
//#include "ofx/IO/FilteredStreams.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
///// \brief A base HTTP cient for executing HTTP client requests.
//class BaseClient
//{
//public:
//
//    /// \brief Create a generic BaseClient.
//    BaseClient();
//
//    /// \brief Create a BaseClient with the given filters.
//    /// \param requestFilters A collection of request header filters.
//    /// \param responseFilters A collection of response header filters.
//    BaseClient(std::vector<AbstractRequestFilter*> requestFilters,
//               std::vector<AbstractResponseFilter*> responseFilters);
//
//    /// \brief Destroy the BaseClient.
//    virtual ~BaseClient();
//
//    /// \brief Execute a request and get the raw input stream.
//    ///
//    /// This is the workhorse function for all client / server interactions. All
//    /// functions eventually call this function for exectution.
//    ///
//    /// \param request The request to execute.
//    /// \param response The response data.
//    /// \param context The context data used for the transaction.
//    /// \throws Various exceptions.
//
//    //    /// \brief Submit a request and get the results via callback.
//    //    ///
//    //    /// Listeners should subscribe to the events in the ClientEvents member.
//    //    /// Exceptions will be passed as errors to the event listeners. Filter
//    //    /// events, response events and progress events will also be broadcast to
//    //    /// event listeners. See ClientEvents for more.
//    //    ///
//    //    /// \param request The request to execute.
//    //    /// \param response The response data.
//    //    /// \param context The context data used during the submission.
//
//    void execute(Context& context,
//                 Request& request,
//                 Response& response);
//
////    /// \brief Execute a request and return the buffered data.
////    /// \param request A pointer to the Request to execute.
////    /// \returns a BufferedResponse.
////    /// \tparam RequestType The request type to execute and return.
////    template <typename RequestType>
////    std::unique_ptr<BufferedResponse<RequestType>> executeBuffered(std::unique_ptr<RequestType> request)
////    {
////        return executeBuffered<RequestType, BufferedResponse<RequestType>>(std::move(request));
////    }
////
////    /// \brief Execute a request and return the buffered data.
////    /// \param request A pointer to the Request to execute.
////    /// \returns a ResponseType.
////    /// \tparam RequestType The request type to execute.
////    /// \tparam ResponseType The response type to return.
////    template <typename RequestType, typename ResponseType>
////    std::unique_ptr<ResponseType> executeBuffered(std::unique_ptr<RequestType> request)
////    {
////        auto response = std::make_unique<ResponseType>(std::move(request),
////                                                       std::move(createContext()));
////
////        try
////        {
////            // The buffer to fill.
////            ofBuffer buffer(*execute(response->request(),
////                                     *response,
////                                     response->context()));
////
////            // Set the buffer.
////            response->setBuffer(buffer);
////        }
////        catch (const Poco::Exception& exc)
////        {
////            auto exception = std::make_unique<Poco::Exception>(exc);
////            response->setException(std::move(exception));
////        }
////        catch (const std::exception& exc)
////        {
////            auto exception = std::make_unique<Poco::Exception>(exc.what());
////            response->setException(std::move(exception));
////        }
////        catch (...)
////        {
////            auto exception = std::make_unique<Poco::Exception>("Unknown exception in BaseClient::execute.");
////            response->setException(std::move(exception));
////        }
////
////        return response;
////    }
////
////
//    /// \brief Add a request header filter.
//    ///
//    /// Request filters are applied to the Request in the order they are
//    /// are added. Request filters are often used to introduce additional header
//    /// or other client information such as authentication credentials.
//    ///
//    /// The client does not own the request filter, but merely holds a pointer.
//    /// Request filter designers must ensure thread safety if multiple clients
//    /// share the same filter.
//    ///
//    /// \param filter The request filter to add.
//    void addRequestFilter(AbstractRequestFilter* filter);
//
//    /// \brief Remove the given request header filter.
//    /// \param filter The filter to remove.
//    void removeRequestFilter(AbstractRequestFilter* filter);
//
//    /// \brief Add a response header filter.
//    ///
//    /// Response filters are applied to the Response in the order they are
//    /// are added. Response filters are often used to introduce additional
//    /// header data or analyze the response information to extract client
//    /// information such as cookies or credentials.
//    ///
//    /// The client does not own the response filter, but merely holds a pointer.
//    /// Response filter designers must ensure thread safety if multiple clients
//    /// share the same filter.
//    ///
//    /// \param filter The response header filter to add.
//    void addResponseFilter(AbstractResponseFilter* filter);
//
//    /// \brief Remove the given response header filter.
//    /// \param filter The filter to remove.
//    void removeResponseFilter(AbstractResponseFilter* filter);
//
//    /// \brief Add a request stream filter.
//    ///
//    /// Clients are permitted to add request stream filters. Rrequest stream
//    /// filters allows the user to filter and potentially modify
//    /// the request data stream. These are used rarely.
//    ///
//    /// Users should be aware that modifying a request stream may cause the
//    /// request stream to become inconsistent with the request headers. Thus,
//    /// special care should be taken to make appropriate modifications to the
//    /// headers.
//    ///
//    /// The client does not own the request stream filter, but merely holds a
//    /// pointer. Request stream filter designers must ensure thread safety if
//    /// multiple clients share the same filter.
//    ///
//    /// \param filter The request stream filter.
//    void addRequestStreamFilter(AbstractRequestStreamFilter* filter);
//
//    /// \brief Remove the current request stream filter.
//    void removeRequestStreamFilter(AbstractRequestStreamFilter* filter);
//
//    /// \brief Set the response stream filter.
//    ///
//    /// Clients are permitted to add response stream filters. Response stream
//    /// filters allow the user to filter and potentially modify the respnse data
//    /// stream. These are often used to apply data decompression.
//    ///
//    /// The client does not own the response stream filter, but merely holds a
//    /// pointer. Response stream filter designers must ensure thread safety if
//    /// multiple clients share the same filter.
//    ///
//    /// Setting a response stream filter will replace any existing filter.
//    ///
//    /// \param filter The response stream filter.
//    void addResponseStreamFilter(AbstractResponseStreamFilter* filter);
//
//    /// \brief Remove the current request stream filter.
//    void removeResponseStreamFilter(AbstractResponseStreamFilter* filter);
//
//protected:
//    /// \brief A list of request filters.
//    ///
//    /// These filters are automatically applied to the outgoing request headers.
//    std::vector<AbstractRequestFilter*> _requestFilters;
//
//    /// \brief A list of response filters.
//    ///
//    /// These filters are automatically applied to the incoming response headers.
//    std::vector<AbstractResponseFilter*> _responseFilters;
//
//    /// \brief A list of request stream filters.
//    std::vector<AbstractRequestStreamFilter*> _requestStreamFilters;
//
//    /// \brief A list of response stream filters.
//    std::vector<AbstractResponseStreamFilter*> _responseStreamFilters;
//
//private:
////    /// \brief An internal function that applies the request filters.
////    /// \param request The request to filter.
////    /// \param context The context of the request.
////    void _filterRequest(Request& request, Context& context);
////
////    /// \brief An internal function that applies the response filters.
////    /// \param request The request.
////    /// \param response The response to filter.
////    /// \param context The context of the request.
////    void _filterResponse(Request& request,
////                         Response& response,
////                         Context& context);
////
////    /// \brief An internal function that establishes a connection.
////    /// \param request The filtered request.
////    /// \param context The context of the request.
////    /// \returns a unique pointer to an output stream to write to.
////    std::unique_ptr<std::ostream> _send(Request& request, Context& context);
////
////    /// \brief An internal function that receives a response.
////    /// \param request The submitted request.
////    /// \param response The response headers generated.
////    /// \param context The context of the request.
////    /// \returns a unique pointer to an input stream to be read.
////    std::unique_ptr<std::istream> _receive(Request& request,
////                                           Response& response,
//
//
//    
//
//};
//
//
//} } // namespace ofx::HTTP
