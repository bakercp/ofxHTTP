//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofSSLManager.h"
#include "ofxIO.h"
#include "ofxMediaType.h"
#include "ofxNetworkUtils.h"
#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/AbstractServerTypes.h"
#include "ofx/HTTP/BaseClient.h"
#include "ofx/HTTP/Response.h"
#include "ofx/HTTP/Request.h"
#include "ofx/HTTP/ClientEvents.h"
#include "ofx/HTTP/ClientSessionProvider.h"
#include "ofx/HTTP/ClientState.h"
#include "ofx/HTTP/Context.h"
#include "ofx/HTTP/DefaultProxyProcessor.h"
#include "ofx/HTTP/DefaultRedirectProcessor.h"
#include "ofx/HTTP/DefaultClientHeaders.h"
#include "ofx/HTTP/DefaultCookieProcessor.h"
#include "ofx/HTTP/DefaultEncodingResponseStreamFilter.h"
#include "ofx/HTTP/FormRequest.h"
#include "ofx/HTTP/GetRequest.h"
#include "ofx/HTTP/HeadRequest.h"
#include "ofx/HTTP/HTTPClient.h"
#include "ofx/HTTP/Client.h"
#include "ofx/HTTP/GetRequestTask.h"
//#include "ofx/HTTP/HTTPClientTask.h"
#include "ofx/HTTP/HTTPUtils.h"
#include "ofx/HTTP/JSONRequest.h"
#include "ofx/HTTP/JSONWebToken.h"
#include "ofx/HTTP/OAuth10Credentials.h"
#include "ofx/HTTP/OAuth10RequestFilter.h"
#include "ofx/HTTP/OAuth20Credentials.h"
#include "ofx/HTTP/OAuth20RequestFilter.h"
#include "ofx/HTTP/PostRequest.h"
#include "ofx/HTTP/PutRequest.h"
#include "ofx/HTTP/Session.h"
#include "ofx/HTTP/SessionStore.h"
#include "ofx/HTTP/SimpleFileServer.h"
#include "ofx/HTTP/SimpleIPVideoServer.h"
#include "ofx/HTTP/SimplePostServer.h"
#include "ofx/HTTP/SimpleSSEServer.h"
#include "ofx/HTTP/SimpleWebSocketServer.h"
#include "ofx/HTTP/SSEEvents.h"
#include "ofx/HTTP/SSEFrame.h"
#include "ofx/HTTP/SSERoute.h"
#include "ofx/HTTP/SSEConnection.h"
#include "ofx/HTTP/URIBuilder.h"
#include "ofx/HTTP/WebSocketEvents.h"
#include "ofx/HTTP/WebSocketRoute.h"
#include "ofx/HTTP/WebSocketFrame.h"
#include "ofx/HTTP/WebSocketConnection.h"


namespace ofxHTTP = ofx::HTTP;
