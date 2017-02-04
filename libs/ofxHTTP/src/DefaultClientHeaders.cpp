//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/HTTP/DefaultClientHeaders.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/Context.h"


namespace ofx {
namespace HTTP {


DefaultClientHeaders::DefaultClientHeaders()
{
}


DefaultClientHeaders::~DefaultClientHeaders()
{
}


void DefaultClientHeaders::requestFilter(Context& context,
                                         Request& request) const
{
    const ClientSessionSettings& settings = context.getClientSessionSettings();

    if (!settings.getUserAgent().empty())
    {
        request.set("User-Agent", settings.getUserAgent());
    }

//    request.set("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
//    request.set("Accept-Language", "en-US,en;q=0.8");

    for (const auto& entry: settings.getDefaultHeaders())
    {
        request.set(entry.first, entry.second);
    }
}



} } // namespace ofx::HTTP
