

#include "BaseServerSettings.h"


namespace ofx {
namespace HTTP {


const std::string BaseServerSettings::DEFAULT_HOST = "127.0.0.1";
const unsigned short BaseServerSettings::DEFAULT_PORT = 8080;
const bool BaseServerSettings::DEFAULT_USE_SSL = false;


//------------------------------------------------------------------------------
BaseServerSettings::BaseServerSettings(const std::string& host,
                                       const unsigned short port,
                                       bool useSSL):
    _host(host),
    _port(port),
    _useSSL(useSSL)
{
}

//------------------------------------------------------------------------------
BaseServerSettings::~BaseServerSettings()
{
}

//------------------------------------------------------------------------------
void BaseServerSettings::setHost(const std::string& host)
{
    _host = host;
}

//------------------------------------------------------------------------------
std::string BaseServerSettings::getHost() const
{
    return _host;
}

//------------------------------------------------------------------------------
void BaseServerSettings::setPort(const unsigned short port)
{
    _port = port;
}

//------------------------------------------------------------------------------
unsigned short BaseServerSettings::getPort() const
{
    return _port;
}

//------------------------------------------------------------------------------
void BaseServerSettings::setUseSSL(bool useSSL)
{
    _useSSL = useSSL;
}

//------------------------------------------------------------------------------
bool BaseServerSettings::getUseSSL() const
{
    return _useSSL;
}

//------------------------------------------------------------------------------
Poco::URI BaseServerSettings::getURI() const
{
    Poco::URI uri;

    uri.setScheme(_useSSL ? "https" : "http");
    uri.setHost(_host);
    uri.setPort(_port);

    return uri;
}    


} } // namespace ofx::HTTP
