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


#include "ofSSLManager.h"

#include "ofx/HTTP/IPVideo/BasicIPVideoServer.h"

#include "ofx/HTTP/Server/BasicPostServer.h"
#include "ofx/HTTP/Server/BasicServer.h"
#include "ofx/HTTP/Server/SessionCache.h"

#include "ofx/HTTP/WebSocket/BasicWebSocketServer.h"
#include "ofx/HTTP/WebSocket/WebSocketEvents.h"
#include "ofx/HTTP/WebSocket/WebSocketRoute.h"
#include "ofx/HTTP/WebSocket/WebSocketFrame.h"
#include "ofx/HTTP/WebSocket/WebSocketConnection.h"

#include "ofx/HTTP/Client/ResponseStream.h"
#include "ofx/HTTP/Client/Context.h"
#include "ofx/HTTP/Client/GetRequest.h"
#include "ofx/HTTP/Client/PostRequest.h"
#include "ofx/HTTP/Client/PutRequest.h"
#include "ofx/HTTP/Client/ClientEvents.h"
#include "ofx/HTTP/Client/BaseClient.h"

#include "ofx/HTTP/Client/DefaultSessionProvider.h"
#include "ofx/HTTP/Client/DefaultProxyProcessor.h"
#include "ofx/HTTP/Client/DefaultAuthenticationProcessor.h"
#include "ofx/HTTP/Client/DefaultRedirectProcessor.h"
#include "ofx/HTTP/Client/DefaultClientHeaders.h"
#include "ofx/HTTP/Client/DefaultCookieProcessor.h"
