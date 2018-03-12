//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Poco/URI.h"
#include "Poco/Net/HTMLForm.h"
#include "ofUtils.h"


namespace ofx {
namespace HTTP {


/// \brief A base utility for building URIs.
class URIBuilder: public Poco::Net::NameValueCollection
{
public:
    /// \brief Construct an empty URI.
    URIBuilder();

    /// \brief Construct a URI using a base URI.
    /// \param uri A base URI.  Query parameters will be extracted.
    URIBuilder(const Poco::URI& uri);

    /// \brief Destroy the URIBuilder.
    virtual ~URIBuilder();

    /// \brief Set a boolean key / value pair in the name value collection.
    /// \param key The key.
    /// \param value The boolean value.
    /// \param numerical if true, will set the value as a "1" or "0",
    ///     otherwise will set the value as a "true" or "false".
    void set(const std::string& key, bool value, bool numerical = true);

    /// \brief Add a boolean key / value pair in the name value collection.
    /// \param key The key.
    /// \param value The boolean value.
    /// \param numerical if true, will set the value as a "1" or "0",
    ///     otherwise will set the value as a "true" or "false".
    void add(const std::string& key, bool value, bool numerical = true);

    /// \brief Set an key / value pair in the name value collection.
    ///
    /// This method converts the value to a string if possible.
    ///
    /// \param key The key.
    /// \param value The value.
    /// \tparam T The type to set.
    template <typename T>
    void set(const std::string& key, const T& value)
    {
        Poco::Net::NameValueCollection::set(key, ofToString(value));
    }

    /// \brief Set an key / value pair in the name value collection.
    ///
    /// This method converts the value to a string if possible.
    ///
    /// \param key The key.
    /// \param value The value.
    /// \tparam T The type to set.
    template <typename T>
    void set(const std::string& key, const T& value, int preceision)
    {
        Poco::Net::NameValueCollection::set(key, ofToString(value, preceision));
    }

    /// \brief Add an key / value pair in the name value collection.
    ///
    /// This method converts the value to a string if possible.
    ///
    /// \param key The key.
    /// \param value The boolean value.
    ///     otherwise will set the value as a "true" or "false".
    /// \tparam T The type to set.
    template <typename T>
    void add(const std::string& key, const T& value)
    {
        Poco::Net::NameValueCollection::set(key, ofToString(value));
    }

    /// \tparam T The type to set.
    template <typename T>
    void add(const std::string& key, const T& value, int preceision)
    {
        Poco::Net::NameValueCollection::set(key, ofToString(value, preceision));
    }

    /// \brief Get the built URI as a Poco::URI object.
    /// \returns the built URI.
    Poco::URI toURI() const;

    /// \brief Get the built URI as a std::string.
    /// \returns the built URI as a std::string.
    std::string toString() const;

private:
    /// \brief The Poco::URI used as a building foundation.
    Poco::URI _uri;

};


} } // namespace ofx::HTTP
