//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <istream>
#include "Poco/UUID.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/PartHandler.h"
#include "ofLog.h"
#include "ofx/HTTP/BaseRoute.h"
#include "ofx/HTTP/PostRouteEvents.h"


namespace ofx {
namespace HTTP {


/// \brief Settings for a PostRoute.
class PostRouteSettings: public BaseRouteSettings
{
public:
    /// \brief Create PostRouteSettings.
    /// \param routePathPattern The regex pattern that this route will handle.
    /// \param requireSecurePort True if this route requires
    ///        communication on an SSL encrypted port.
    PostRouteSettings(const std::string& routePathPattern = DEFAULT_POST_ROUTE,
                      bool requireSecurePort = false,
                      bool requireAuthentication = false);

    /// \brief Destroy the PostRouteSetting.
    virtual ~PostRouteSettings();

    /// \brief Set the upload folder for file uploads.
    /// \param uploadFolder The path to the upload folder.
    void setUploadFolder(const std::string& uploadFolder);

    /// \returns the current file upload folder.
    const std::string& getUploadFolder() const;

    /// \brief Set the page to forward for upload.
    ///
    /// If set to an empty string, and 200 code will be returned with an empty
    /// body when the POST completes.
    ///
    /// \param uploadRedirect The URI to the redirected page.
    void setUploadRedirect(const std::string& uploadRedirect);

    /// \returns the upload redirect URI.
    std::string getUploadRedirect() const;

    void setWriteBufferSize(std::size_t writeBufferSize);
    std::size_t getWriteBufferSize() const;

    void setFieldLimit(std::size_t fieldLimit);
    std::size_t getFieldLimit() const;

    /// \brief Set the maximum file upload size in bytes.
    /// \param maximumFileUploadSize The maximum file upload size in bytes.
    void setMaximumFileUploadSize(uint64_t maximumFileUploadSize);

    /// \brief Get the maximum file upload size.
    /// \returns The maximum file upload size in bytes.
    uint64_t getMaximumFileUploadSize() const;

    static const std::string DEFAULT_POST_ROUTE;
    static const std::string DEFAULT_POST_FOLDER;
    static const std::string DEFAULT_POST_REDIRECT;

    /// \brief Default values.
    enum Defaults
    {
        /// \brief File upload buffer.
        DEFAULT_POST_BUFFER_SIZE = 8192,
        /// \brief Maximum number of form fields.
        DEFAULT_FIELD_LIMIT = 100,
        /// \brief Maximum file upload size (2 MB)
        DEFAULT_MAXIMUM_FILE_UPLOAD_SIZE = 2097152
    };

    /// \brief An unfortunate compromise until C++11.
    /// \note C++ is not able to initialize static collections until
    ///        after C++11.  This is a compromise until then.
    static const std::string DEFAULT_POST_HTTP_METHODS_ARRAY[];

    /// \brief The default HTTP methods for this route.
    static const HTTPMethodSet DEFAULT_POST_HTTP_METHODS;
    
private:
    std::string _uploadFolder;
    std::string _uploadRedirect;
    std::size_t _writeBufferSize;
    std::size_t _fieldLimit;
    uint64_t _maximumFileUploadSize;
    
};


/// \brief A route for handing HTTP POST requests. 
class PostRoute: public BaseRoute_<PostRouteSettings>
{
public:
    /// \brief A typedef for the PostRouteSettings.
    typedef PostRouteSettings Settings;

    /// \brief Create the PostRoute with the given settings.
    /// \param settings The route settings.
    PostRoute(const Settings& settings = Settings());
    
    /// \brief Destroy the PostRoute.
    virtual ~PostRoute();

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

    /// \returns a reference to the PostRouteEvents.
    PostRouteEvents& getEvents();

    /// \brief The PostRouteEvents.
    PostRouteEvents events;

    /// \brief Register event listeners for this route.
    ///
    /// The listener class must implement the following callbacks:
    ///
    /// onHTTPPostEvent(...), onHTTPFormEvent(...), onHTTPUploadEvent(...)
    ///
    /// \tparam ListenerClass The lister class to register.
    /// \param listener A pointer to the listener class.
    /// \param priority The listener priority.
    template <class ListenerClass>
    void registerPostEvents(ListenerClass* listener, int priority = OF_EVENT_ORDER_AFTER_APP);

    /// \brief Unregister event listeners for this route.
    /// \tparam ListenerClass The lister class to uregister.
    /// \param listener A pointer to the listener class.
    /// \param priority The listener priority.
    template <class ListenerClass>
    void unregisterPostEvents(ListenerClass* listener, int priority = OF_EVENT_ORDER_AFTER_APP);

};


template <class ListenerClass>
void PostRoute::registerPostEvents(ListenerClass* listener, int priority)
{
    ofAddListener(events.onHTTPPostEvent, listener, &ListenerClass::onHTTPPostEvent, priority);
    ofAddListener(events.onHTTPFormEvent, listener, &ListenerClass::onHTTPFormEvent, priority);
    ofAddListener(events.onHTTPUploadEvent, listener, &ListenerClass::onHTTPUploadEvent, priority);
}


template <class ListenerClass>
void PostRoute::unregisterPostEvents(ListenerClass* listener, int priority)
{
    ofRemoveListener(events.onHTTPPostEvent, listener, &ListenerClass::onHTTPPostEvent, priority);
    ofRemoveListener(events.onHTTPFormEvent, listener, &ListenerClass::onHTTPFormEvent, priority);
    ofRemoveListener(events.onHTTPUploadEvent, listener, &ListenerClass::onHTTPUploadEvent, priority);
}


/// \brief A flexible POST route handler.
///
/// Form data must be encoded with "multipart/form-data" or
/// "application/x-www-form-urlencoded".  "text/plain" form
/// encoding is supported, but not parsed.
class PostRouteHandler: public BaseRouteHandler_<PostRoute>
{
public:
    /// \brief A typedef for PostRouteSettings
    typedef PostRouteSettings Settings;

    /// \brief Create a PostRouteHandler.
    /// \param route The parent PostRoute.
    PostRouteHandler(PostRoute& route);

    /// \brief Destroy the PostRouteHandler.
    virtual ~PostRouteHandler();

    void handleRequest(ServerEventArgs& evt);

    /// \brief A constant defining "text/plain".
    static const Poco::Net::MediaType POST_CONTENT_TYPE_TEXT_PLAIN;

    /// \brief A constant defining "multipart/form-data".
    static const Poco::Net::MediaType POST_CONTENT_TYPE_MULTIPART;
    
    /// \brief A constant defining "application/x-www-form-urlencoded".
    static const Poco::Net::MediaType POST_CONTENT_TYPE_URLENCODED;
    
    /// \brief A constant defining "application/json".
    static const Poco::Net::MediaType POST_CONTENT_TYPE_JSON;

};


class PostRouteFileHandler: public Poco::Net::PartHandler
{
public:
    PostRouteFileHandler(PostRoute& route,
                         ServerEventArgs& evt,
                         const std::string& postId);

    virtual ~PostRouteFileHandler();

    void handlePart(const Poco::Net::MessageHeader& header,
                    std::istream& stream);

    bool isContentTypeValid(const std::string& contentType) const;

private:
    /// \brief A reference to the parent
    PostRoute& _route;

    /// \brief A reference to the server event args.
    ServerEventArgs& _evt;

    /// \brief The post id.
    const std::string& _postId;
    
};


} } // namespace ofx::HTTP
