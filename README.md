ofxHTTP
=======

ofxHTTP is designed to work best using Project Generator.


NOTE: Requires the following addon:

- https://github.com/bakercp/ofxMediaType

Some examples require:

- https://github.com/bakercp/ofxJSON


A suite of HTTP tools in pure POCO, Web Server, Web Client, etc.


Other notes:

All web servers can be run from the cloud.  To run the examples, download openFrameworks onto the server.  Usually you will download a Linux64 build and will do this over ssh while connected to your web host.  Then build the core library according to the linux tutorials.  Then make sure that your security settings ("Security Groups" on Amazon EC2) allow incoming connections on the your chosen server ports (the default is 8080).  So, for port 8080, you would allow inbound connections from `8080	0.0.0.0/0`.  Then launch the server with `make && make run` to build and run it the server.  It is up to you to figure out the best way to enable the server at system startup and keep the server running if there are any crashes.  Enjoy!
