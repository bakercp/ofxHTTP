# ofxHTTP

## Description

An [openFrameworks](http://openframeworks.cc) addon for custom HTTP clients and servers.

## Features

This is the second development release.  The web socket examples have been removed and users are encouraged to use the [ofxJSONRPC](https://github.com/bakercp/ofxJSONRPC) examples instead.  They are much more useful.

### Clients
-   Clients work best with up-to-date CA Certificate bundles. A fairly recent example is included in the `ofxHTTP` client examples.  For up-to-date certificates (or if you just want to generate your own for security reasons), check out this page for more info: [http://curl.haxx.se/docs/caextract.html](http://curl.haxx.se/docs/caextract.html).
-   Certificate bundles are managed by [ofxSSLManager](https://github.com/bakercp/ofxSSLManager).

### Servers
If your server serves up files that are downloaded by the browser rather than displayed, make sure you have a mime.types file defined in your data folder [like this](https://github.com/bakercp/ofxHTTP/tree/master/example_basic_file_server/bin/data/media)

All web servers can be run from the cloud.  To run the examples, download openFrameworks onto the server.  Usually you will download a Linux64 build and will do this over ssh while connected to your web host.  Then build the core library according to the linux tutorials.  Then make sure that your security settings ("Security Groups" on Amazon EC2) allow incoming connections on the your chosen server ports (the default is 8080).  So, for port 8080, you would allow inbound connections from `8080	0.0.0.0/0`.  Then launch the server with `make && make run` to build and run it the server.  It is up to you to figure out the best way to enable the server at system startup and keep the server running if there are any crashes.

`ofxHTTP` Servers can also be proxied securely through NGINX. For example, an `ofxHTTP` server running on a cloud server with [Let's Encrypt](https://letsencrypt.org/) via [CERTBOT](https://certbot.eff.org/) might run with an NGINX configuration like this:

```
server {
        server_name server.christopherbaker.net;
        # Make sure to increase the upload size for files about 2MB.
        client_max_body_size 100M;
        location / {
                # The local port to proxy on.
                # This is the host:port that the ofxHTTP server is running on.
                # You configure this port in ofApp::setup() { ... }.
                proxy_pass http://127.0.0.1:8197;

                # Proxy settings that will allow websockets.
                proxy_http_version 1.1;
                proxy_set_header Upgrade $http_upgrade;
                proxy_set_header Connection "Upgrade";
        }

      # Everything below was added by Certbot https://certbot.eff.org/
      listen [::]:443 ssl; # managed by Certbot
      listen 443 ssl; # managed by Certbot
      ssl_certificate /etc/letsencrypt/live/server.christopherbaker.net/fullchain.pem; # managed by Certbot
      ssl_certificate_key /etc/letsencrypt/live/server.christopherbaker.net/privkey.pem; # managed by Certbot
      include /etc/letsencrypt/options-ssl-nginx.conf; # managed by Certbot
      ssl_dhparam /etc/letsencrypt/ssl-dhparams.pem; # managed by Certbot

}


server {
      if ($host = server.christopherbaker.net) {
          return 301 https://$host$request_uri;
      } # managed by Certbot


      listen 80;
      listen [::]:80;

      server_name server.christopherbaker.net;
      return 404; # managed by Certbot
}
```

Theoretically an `ofxHTTP` server could be configured for SSL encryption by setting up a server context with `ofxSSLManager`, but an NGINX proxy is likely simpler.

## Getting Started

To get started, generate the example project files using the openFrameworks [Project Generator](http://openframeworks.cc/learning/01_basics/how_to_add_addon_to_project/).

## Documentation

API documentation can be found [here](https://bakercp.github.io/ofxHTTP/index.html).

## Build Status

Linux, macOS [![Build Status](https://travis-ci.org/bakercp/ofxHTTP.svg?branch=master)](https://travis-ci.org/bakercp/ofxHTTP)

Visual Studio, MSYS [![Build status](https://ci.appveyor.com/api/projects/status/ltsg7y588u4gw9o7/branch/master?svg=true)](https://ci.appveyor.com/project/bakercp/ofxhttp/branch/master)

## Compatibility

### Branches

The `stable` branch of this repository is meant to be compatible with the openFrameworks [stable branch](https://github.com/openframeworks/openFrameworks/tree/stable), which corresponds to the latest official openFrameworks release.

The `master` branch of this repository is meant to be compatible with the openFrameworks [master branch](https://github.com/openframeworks/openFrameworks/tree/master).

Some past openFrameworks releases are supported via [releases](../../releases/), but only the [stable branch](../../tree/stable) and the [master branch](../../tree/master) are actively supported.

### Requirements
-   ofxPoco (included with openFrameworks)
-   [ofxIO](https://github.com/bakercp/ofxIO)
-   [ofxMediaType](https://github.com/bakercp/ofxMediaType)
-   [ofxSSLManager](https://github.com/bakercp/ofxSSLManager)
-   [ofxNetworkUtils](https://github.com/bakercp/ofxNetworkUtils)

Some examples may require:

-   [ofxJSON](https://github.com/bakercp/ofxJSON)

## Versioning

This project uses [Semantic Versioning](http://semver.org/), although strict adherence will only come into effect at version 1.0.0.

## Licensing

See [LICENSE.md](LICENSE.md).

## Contributing

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1.  Fork this repository.
2.  Create your feature branch (`git checkout -b my-new-feature`).
3.  Commit your changes (`git commit -am 'Add some feature'`).
4.  Push to the branch (`git push origin my-new-feature`).
5.  Create new Pull Request.
