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


#include "ofx/HTTP/ServerEvents.h"
#include "ofx/HTTP/SSEFrame.h"


namespace ofx {
namespace HTTP {


class SSERouteHandler;
class SSEConnection;


/// \brief The base server sent event arguments.
class SSEEventArgs: public ServerEventArgs
{
public:
    /// \brief Create a SSEEventArgs object with the provided params.
    /// \param args The server event arguments.
    /// \param connection A reference to the associated SSEConnection.
    SSEEventArgs(ServerEventArgs& args,
                 SSEConnection& connection):
        ServerEventArgs(args),
        _connection(connection)
    {
    }

    /// \returns the associated connection.
    SSEConnection& connection()
    {
        return _connection;
    }

private:
    /// \brief A reference to the SSEConnection.
    SSEConnection& _connection;

};


class SSECloseEventArgs: public SSEEventArgs
{
public:
    SSECloseEventArgs(ServerEventArgs& evt,
                      SSEConnection& connection,
                      uint16_t code,
                      const std::string& reason):
        SSEEventArgs(evt, connection),
        _code(code),
        _reason(reason)
    {
    }

    /// \returns A code, 0 if unset.
    uint16_t code() const
    {
        return _code;
    }

    /// \returns A UTF-8 encoded client close reason, empty if unset.
    const std::string& reason() const
    {
        return _reason;
    }

protected:
    uint16_t _code;
    
    const std::string& _reason;

};


class SSEFrameEventArgs: public SSEEventArgs
{
public:
    /// \brief Create a SSEFrameEventArgs object.
    /// \param args The server event arguments.
    /// \param connection A reference to the associated SSEConnection.
    /// \param frame The server sent event frame.
    SSEFrameEventArgs(ServerEventArgs& args,
                      SSEConnection& connection,
                      const SSEFrame& frame):
        SSEEventArgs(args, connection),
        _frame(frame)
    {
    }
    
    /// \returns A const reference to the SSEFrame associated with the event.
    const SSEFrame& frame() const
    {
        return _frame;
    }
    
private:
    /// \brief A reference to the SSEFrame associated with the event.
    const SSEFrame& _frame;
    
};


/// \breif A typedef for SSEOpenEventArgs.
typedef SSEEventArgs SSEOpenEventArgs;


/// \brief A collection of events called by SSEConnections.
class SSEEvents
{
public:
    /// \brief An event that is called when an SSE connection is opened.
    ofEvent<SSEOpenEventArgs>  onSSEOpenEvent;

    /// \brief An event that is called when an SSE connection is closed.
    ofEvent<SSECloseEventArgs> onSSECloseEvent;

    /// \brief An event that is called when a SSE frame is sent.
    ofEvent<SSEFrameEventArgs> onSSEFrameSentEvent;

};


} } // namespace ofx::HTTP
