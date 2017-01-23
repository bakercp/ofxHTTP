//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//

$(document).ready(function() {
  var eventList = document.getElementById("event-list");

  var evtSource = new EventSource("/event-source");

  evtSource.onmessage = function(e) {
    var newElement = document.createElement("li");
    newElement.innerHTML = "message: " + e.data;
    eventList.appendChild(newElement);
  };

  evtSource.addEventListener("ping", function(e) {
    var newElement = document.createElement("li");
    var obj = JSON.parse(e.data);
    newElement.innerHTML = "ping at " + obj.time;
    eventList.appendChild(newElement);
  }, false);

});
