/*==============================================================================
 
 Copyright (c) 2013 - Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ==============================================================================*/

// define a needle in a haystack style array find, implementing indexOf on demand
Array.prototype.hasObject = (
  !Array.indexOf ? function (o) {
    var l = this.length + 1;
    while (l -= 1) {
        if (this[l - 1] === o) {
            return true;
        }
    }
    return false;
  } : function (o) {
    return (this.indexOf(o) !== -1);
  }
);

function ofClamp(val,max,min) {
    return (val < min ? max :(val > min ? min : val));
}

function ofxHTTPWebSocket(_supportedProtocols) {
  var DATA_TYPE_BINARY = 0;
  var DATA_TYPE_ARRAY  = 1;
  var DATA_TYPE_STRING = 2;

  var self = this; // keep track of this class

  var ws  = null; // WebSocket
  this.getWebSocket = function() { return ws; }
  this.isConnected  = function() { return ws != null; }

  var wsDataType = "binary";
  this.getWebSocketDataType = function() { return }

  // var url = null; // WebSocket URL
  // this.getURL = function() { return ws; }

  var customPath = "";
  this.getCustomPath = function()            { return customPath;        }
  this.setCustomPath = function(_customPath) { customPath = _customPath; }

  // make sure that the appropriate websocket scheme is used
  // i.e. ws:// or wss:// for secure connections
  this.getWebSocketURL = function() {
    var scheme;
    var url = document.URL;
    if (url.substring(0, 5) == "https") {
      scheme = "wss://";
      url = url.substr(8);
    } else {
      scheme = "ws://";
      if (url.substring(0, 4) == "http") {
        url = url.substr(7);
      }
    }
    url = url.split('/');
    return scheme + url[0];
  }

  var suppotedSubProtocols = _supportedProtocols || [];
  // Privileged method pattern
  this.getSupportedSubProtocols = function() { return suppotedSubProtocols; }

  var keepAliveTimeout      = null;  // set with setTimeout()
  var keepAliveInterval     = 15000; // milliseconds
  this.getKeepAliveInterval = function() { return keepAliveInterval; }
  this.setKeepAliveInterval = function(_keepAliveInterval) { keepAliveInterval = _keepAliveInterval; }

  var retryConnectTimer     = null;  // set with setInterval()
  var retryConnectInterval  = 5000;  // milliseconds
  this.getRetryConnectInterval = function() { return retryConnectInterval; }
  this.setRetryConnectInterval = function(_retryConnectInterval) { retryConnectInterval = _retryConnectInterval; }
  var bIsRetryingConnect    = false; // are we in the process of retrying (could also see if retry timer is set?)
  this.isRetryingConnect    = function() { return bIsRetryingConnect; }
  var maxConnectRetries     = 10;    // we will allow this many retries 
  this.getMaxConnectRetries = function() { return maxConnectRetries; }
  this.setMaxConnectRetries = function(_maxConnectRetries) { 
    maxConnectRetries = _maxConnectRetries; 
    numConnectRetries = _maxConnectRetries;
  }
  var numConnectRetries     = maxConnectRetries;    // we will allow this many retries 
  this.getNumConnectRetries = function() { return numConnectRetries; }

  var messagesElement       = document.getElementById("ofxHTTPWebSocketMessages"); // div for message text
  this.getMessagesElement   = function() { return messagesElement; }
  this.setMessagesElement   = function(_messagesElement) { messagesElement = _messagesElement; }

  var statusElement         = document.getElementById("ofxHTTPWebSocketStatus");   // div for status text
  this.getStatusElement     = function() { return statusElement; }
  this.setStatusElement     = function(_statusElement) { statusElement = _statusElement; }

  this.setBinaryType        = function(_binaryType) {
    if(this.isConnected) {
      if(_binaryType == "blob" || _binaryType == "arraybuffer") {
        ws.binaryType = _binaryType;
      } else {
        ofLogError("Unknown binaryType: " + _binaryType);
      }
    } else {
      ofLogError("WebSocket has not been initialized.  Please call connect first.");
    }
  }

  this.getBufferedAmount    = function() { 
    if(ws != null) {
      return ws.bufferedAmount; 
    } else {
      ofLogError("WebSocket has not been initialized.  Please call connect first.");
    }
  }

  this.connect = function() {
      if ("WebSocket" in window) {
        try {
          ws = new WebSocket(self.getWebSocketURL(),self.getSupportedSubProtocols());

          ws.onopen = function(evt) {
            self.onOpen(evt); // our event
            self.keepAliveTimer = setInterval( function() { self.sendKeepAlive(); }, self.getKeepAliveInterval());
          } 
          ws.onmessage = function(evt) { 
            if(evt.data instanceof ArrayBuffer ) {
              self.onArrayBufferData(evt);
            } else if(evt.data instanceof Blob) {
              self.onBlobData(evt);
            } 

            // all messages are sent here
            self.onMessage(evt);

          }
          ws.onclose = function(evt) {
            clearInterval(self.keepAliveTimer);
            if(!evt.wasClean || evt.code == 1006 /* 1006 == abnormal closure */) {
              
              if(!self.getSupportedSubProtocols().length > 0 &&
                  self.getSupportedSubProtocols().hasObject(evt.target.protocol)) {
                //console.log("None of the proposed subprotocols [" + self.getSupportedSubProtocols() + "] were supported by the server.");
              } else {
                //console.log("Closed for an unknown reason, code: " + evt.code);
              }
            }
            self.onClose(evt); // our event
          }
          ws.onerror = function(evt) { self.onerror(evt); }

        } catch(exception) {
          ofLogError(exception);  
        } 
      } else {
         alert("This browser does not support WebSockets.  Please upgrade your browser.");
      }
    }


  this.disconnect = function() {
    if(ws != null) {
      window.clearInterval(keepAliveTimeout);

      ws.close();
      ws = null;
    } else {
      ofLogError("WebSocket has not been initialized.  Please call connect first.");
    }
  }

  this.send = function(val) {
    // this will be reset each time
    window.clearInterval(keepAliveTimeout);
    keepAliveTimeout = window.setInterval(this.sendKeepAlive(),keepAliveInterval);

    // lastSendTime = now
    ws.send(val);
    console.log("HIT: %s", val);
  }

  // override these!
  this.onUpdateMessage      = function(message) { ofLogNotice("onMessageUpdate"); }
  this.onUpdateStatus       = function(status)  { ofLogNotice("onStatusUpdate");  }

  this.sendKeepAlive        = function()        { ofLogNotice("sendKeepAlive");   }

  this.onOpen               = function(evt) { ofLogNotice("onOpen");    } 
  this.onMessage            = function(evt) { ofLogNotice("onMessage " + evt.data); }

  this.onArrayBufferData    = function(evt) { ofLogNotice("onArrayBufferData " + evt.data); }
  this.onBlobData           = function(evt) { ofLogNotice("onBlobData " + evt.data); }
  this.onStringData         = function(evt) { ofLogNotice("onStringData " + evt.data); }

  this.onClose              = function(evt) { ofLogNotice("onClose");   }
  this.onError              = function(evt) { ofLogNotice("onError");   }


}