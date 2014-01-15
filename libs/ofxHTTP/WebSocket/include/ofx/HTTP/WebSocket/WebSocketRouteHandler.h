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


#pragma once


#include <queue>
#include "Poco/Exception.h"
#include "Poco/Timespan.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofx/HTTP/Types/AbstractTypes.h"
#include "ofx/HTTP/WebSocket/WebSocketConnection.h"
#include "ofx/HTTP/WebSocket/WebSocketRouteSettings.h"
#include "ofx/HTTP/WebSocket/WebSocketEvents.h"
#include "ofx/HTTP/WebSocket/WebSocketFrame.h"
#include "ofx/HTTP/Utils/Utils.h"


namespace ofx {
namespace HTTP {


class WebSocketRoute;


// TODO: move default constants to enum
class WebSocketRouteHandler: public AbstractRouteHandler
    /// \brief A WebSocketRouteHandler registers new WebSocketConnections.
    /// \details Upon creating a new WebSocketConnection, the connection is
    ///         returned to the parent WebSocketRoute for management via
    ///         the WebSocketRouteInterface.
    ///         A WebSocketRouteHandler exist as long as the
    ///         WebSocketConnection remains alive.  Upon completion,
    ///         the WebSocketConnection is unregistered and cleaned up.
{
public:
    typedef WebSocketRouteSettings Settings;
        ///< \brief A typedef for Settings.

    WebSocketRouteHandler(WebSocketRoute& parent);
        ///< \brief Create a WebSocketRouteHandler with a reference to
        ///<        its parent route.
        ///< \param parent A reference to the parent route.  Used for
        ///<        long-term WebSocketConnection maintenance.

    virtual ~WebSocketRouteHandler();
        ///< \brief Destroy the WebSocketRouteHandler.

    virtual void handleRequest(Poco::Net::HTTPServerRequest& request,
                               Poco::Net::HTTPServerResponse& response);

private:
    WebSocketRoute& _parent;
        ///< \brief The parent WebSocketRouteInterface.

};


} } // namespace ofx::HTTP
            
