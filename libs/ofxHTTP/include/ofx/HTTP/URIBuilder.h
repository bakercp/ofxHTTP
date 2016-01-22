// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Poco/URI.h"
#include "Poco/Net/HTMLForm.h"


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
    /// \param value The boolean value.
    ///     otherwise will set the value as a "true" or "false".
    template <typename T>
    void set(const std::string& key, const T& value)
    {
        std::ostringstream ss;
        ss << value;
        Poco::Net::NameValueCollection::set(key, ss.str());
    }

    /// \brief Add an key / value pair in the name value collection.
    ///
    /// This method converts the value to a string if possible.
    ///
    /// \param key The key.
    /// \param value The boolean value.
    ///     otherwise will set the value as a "true" or "false".
    template <typename T>
    void add(const std::string& key, const T& value)
    {
        std::ostringstream ss;
        ss << value;
        Poco::Net::NameValueCollection::add(key, ss.str());
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
