//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <string>
#include <vector>
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "ofLog.h"
#include "ofUtils.h"


namespace ofx {
namespace HTTP {


/// \brief A collection of HTTP utilities.
/// \todo Remove ofLog / ofUtils dependencies.
class HTTPUtils
{
public:
    /// \brief Extract name-value pairs from text/plain encoded posts.
    /// \param textPlain The plain text post data.  This function assumes
    ///        that each form field is on its own line.  It then splits the
    ///        key / value pairs by finding the first '='.  All other '='
    ///        are ignored.
    /// \returns A map of the parameter names and their values.
    static Poco::Net::NameValueCollection splitTextPlainPost(const std::string& textPlain);

    /// \brief Decode URL-encoded parameters names and their values.
    /// \param urlEncoded An URL-encoded string.
    /// \returns A map of the parameter names and their values.
    /// \todo Remove ofSplitString in favor of Poco's string tokenizer.
    static Poco::Net::NameValueCollection splitAndURLDecode(const std::string& urlEncoded);

    /// \brief Decode URL-encoded parameters in a query.
    /// \param uri The URI from which to extract the query parameter map.
    /// \returns A map of the parameter names and their values.
    static Poco::Net::NameValueCollection getQueryMap(const Poco::URI& uri);

    /// \brief Encode query parameters.
    /// \param query The query paramaters as a map.
    /// \returns An URL-encoded query string.
    static std::string makeQueryString(const Poco::Net::NameValueCollection& query);

    /// \brief Dump headers from both requests and responses.
    /// \param request the HTTPRequest request.
    /// \param response the HTTPResponse response.
    static void dumpHeaders(const Poco::Net::HTTPRequest& request,
                            const Poco::Net::HTTPResponse& response,
                            ofLogLevel logLevel = OF_LOG_VERBOSE);

    /// \brief Dump headers from a server request.
    /// \param request the HTTPRequest request.
    static void dumpHeaders(const Poco::Net::HTTPRequest& request,
                            ofLogLevel logLevel = OF_LOG_VERBOSE);

    /// \brief Dump headers from a server response.
    /// \param response the HTTPResponse response.
    static void dumpHeaders(const Poco::Net::HTTPResponse& response,
                            ofLogLevel logLevel = OF_LOG_VERBOSE);

    /// \brief Dump a name value collection from a server response.
    /// \param nvc the Name value collection to print.
    /// \param logLevel The log level at which to print.
    static void dumpNameValueCollection(const Poco::Net::NameValueCollection& nvc,
                                        ofLogLevel logLevel = OF_LOG_VERBOSE);

    /// \brief Consume a stream.
    /// \warning This function will read a stream to the end.  This method will
    ///          block until the entire stream is consumed.  Thus the stream
    ///          must be of finite length or the this method will block
    ///          indefinitely.
    /// \param stream input stream.
    /// \returns the number of bytes consumed.
    static std::streamsize consume(std::istream& stream);

    /// \brief Join a list of values into a single delimited string.
    /// \param values The list of values to explode.
    /// \param delimiter The delimiter to use when exploding.
    /// \returns an exploded delimited list of strings.
    template <typename Type>
    static std::string join(const std::vector<Type>& values,
                            const std::string& delimiter = " ",
                            bool addTrailingDelimiter = false)
    {
        std::stringstream ss;

        for (auto i = 0; i < values.size(); ++i)
        {
            ss << values[i];

            if (addTrailingDelimiter || i < values.size() - 1)
            {
                ss << delimiter;
            }
        }

        return ss.str();
    }

};


} } // namespace ofx::HTTP
