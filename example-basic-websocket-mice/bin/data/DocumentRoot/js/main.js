// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
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


function onOpen(ws) 
{
    ofLogNotice("Connection Opened " + ws);
}

function onMessage(evt) 
{
    console.log("Got a message from openFrameworks: " + evt.data);
}

function onClose()
{
  console.log("Connection closed.");
}

function onError()
{
  console.log("Connection Error.");
}

$(document).ready( function()
{
    // create the new socket
    var ws = new ofxHTTPBasicWebSocketClient();//['of','off']);
    console.log(ws.getWebSocketURL());
    console.log(ws.getKeepAliveInterval());

    // // set callbacks
    ws.setOnOpen(onOpen);
    ws.setOnMessage(onMessage);
    ws.setOnClose(onClose);
    ws.setOnError(onError);

    // when we want to send a message, place a string in here:
    // ws.send(x + "," + y);
  
    // // connect to the websocket
    ws.connect();

});
