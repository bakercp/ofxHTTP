//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "Poco/URI.h"
#include "Poco/String.h"
#include "Poco/Timestamp.h"
#include "Poco/UTF8String.h"
#include "Poco/Net/HTTPCookie.h"
#include "ofUtils.h"


namespace ofx {
namespace HTTP {


class Cookie: public Poco::Net::HTTPCookie
{
public:
    Cookie(const Poco::Net::HTTPCookie& cookie);
    Cookie(const Poco::Net::HTTPCookie& cookie, Poco::Timestamp createdAt);

    virtual ~Cookie();

    /// \returns The Cooke's created-at timestamp.
    Poco::Timestamp createdAt() const;

//    bool isExpired(Poco::Timestamp expiredAt = Poco::Timestamp()) const;
//    bool isSession() const;
//    
//    bool matchesDomain(const Poco::URI& uri) const;
//    bool matchesPath(const Poco::URI& uri) const;
//    bool matchesURI(const Poco::URI& uri,
//                    bool matchSessionCookies = true) const;
//    // matches uri
//    // makes sure the cookie
//    bool matches(const Cookie& cookie) const;
//    
//    std::string toString() const;

protected:
    /// \brief The Cookie's created-at timestamp.
    Poco::Timestamp _createdAt;
      
};


} } // namespace ofx::HTTP
