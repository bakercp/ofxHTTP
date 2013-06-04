#include "BaseRequest.h"


namespace ofx {
namespace HTTP {
namespace Request {

    
//------------------------------------------------------------------------------
BaseRequest::BaseRequest(const std::string& _httpMethod,
                         const std::string& _uri,
                         const std::string& _httpVersion)
: httpMethod(_httpMethod)
, httpVersion(_httpVersion)
{
    
    // this is kind of a mess, but we want to create requests from string uris
    try {
        uri = Poco::URI(_uri);
        bHasValidURI = true;
    } catch(const Poco::SyntaxException& exc) {
        invalidURI = _uri;
        uri = Poco::URI("http://127.0.0.1");
        bHasValidURI = false;
        ofLogError("ofxHTTPBaseRequest::ofxHTTPBaseRequest") << "Syntax exeption: " << exc.message() << " Setting uri to http://127.0.0.1";
    }
    
    setFormFieldsFromURI(uri);
}

//------------------------------------------------------------------------------
BaseRequest::BaseRequest(const std::string& _httpMethod,
                         const Poco::URI& _uri,
                         const std::string& _httpVersion)
: httpMethod(_httpMethod)
, httpVersion(_httpVersion)
, bHasValidURI(true)
, uri(_uri)
{
    setFormFieldsFromURI(uri);
}

//------------------------------------------------------------------------------
BaseRequest::~BaseRequest()
{

}

//------------------------------------------------------------------------------
std::string BaseRequest::getMethod() const
{
    return httpMethod;
}

//------------------------------------------------------------------------------
std::string BaseRequest::getVersion() const
{
    return httpVersion;
}

//------------------------------------------------------------------------------
bool BaseRequest::hasValidURI() const
{
    return bHasValidURI;
}

//------------------------------------------------------------------------------
Poco::URI BaseRequest::getURI() const
{
    return uri;
}

//------------------------------------------------------------------------------
void BaseRequest::addCookie(const std::string& name,
                            const std::string& value,
                            bool isValueEscaped)
{
    std::string escapedValue = value;

    if(!isValueEscaped) {
        escapedValue = Poco::Net::HTTPCookie::escape(value);
    }

    addCookie(Poco::Net::HTTPCookie(name, escapedValue));
}

//------------------------------------------------------------------------------
void BaseRequest::addCookie(const Poco::Net::HTTPCookie& cookie)
{
    addCookie(Cookie(cookie));
}

//------------------------------------------------------------------------------
void BaseRequest::addCookie(const Cookie& cookie)
{
    // erase any matching cookies
    vector<Cookie>::iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        if((*iter).matches(cookie)) {
            iter = cookies.erase(iter);
        } else {
            ++iter;
        }
    }
    
    if(!cookie.isExpired()) {
        cookies.push_back(cookie);
    }
}

//------------------------------------------------------------------------------
void BaseRequest::addCookies(const std::vector<Cookie>& _cookies)
{
    std::vector<Cookie>::const_iterator iter = _cookies.begin();
    while(iter != _cookies.end()) {
        addCookie(*iter);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void BaseRequest::clearCookies()
{
    cookies.clear();
}

//------------------------------------------------------------------------------
    void BaseRequest::addFormField(const std::string& name,
                                   const std::string& value)
{
    form.add(name, value);
}

//------------------------------------------------------------------------------
void BaseRequest::addFormFields(const Poco::Net::NameValueCollection& formFields)
{
    Poco::Net::NameValueCollection::ConstIterator iter = formFields.begin();
    while(iter != formFields.end()) {
        addFormField((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
bool BaseRequest::hasFormFields() const
{
    return !form.empty();
}

//------------------------------------------------------------------------------
bool BaseRequest::hasFormField(const std::string& name) const
{
    return form.has(name);
}

//------------------------------------------------------------------------------
void BaseRequest::removeFormField(const std::string& name)
{
    form.erase(name);
}

//------------------------------------------------------------------------------
void BaseRequest::clearFormFields() {
    form.clear();
}

//------------------------------------------------------------------------------
void BaseRequest::addHeader(const std::string& name, const std::string& value)
{
    headers.add(name,value);
}

//------------------------------------------------------------------------------
void BaseRequest::addHeaders(const Poco::Net::NameValueCollection& _headers)
{
    Poco::Net::NameValueCollection::ConstIterator iter = _headers.begin();
    while(iter != _headers.end()) {
        addHeader((*iter).first,(*iter).second);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void BaseRequest::removeHeader(const std::string& name)
{
    headers.erase(name);
}

//------------------------------------------------------------------------------
bool BaseRequest::hasHeaders() const
{
    return !headers.empty();
}

//------------------------------------------------------------------------------
bool BaseRequest::hasHeader(const std::string& name) const
{
    return headers.has(name);
}

//------------------------------------------------------------------------------
void BaseRequest::clearHeaders()
{
    headers.clear();
}

//------------------------------------------------------------------------------
void BaseRequest::setFormFieldsFromURI(const Poco::URI& uri)
{
    // attempt to extract uri query as form fields
    std::vector<string> queryTokens = ofSplitString(uri.getQuery(),"&",true,true);
    
    std::vector<string>::const_iterator iter = queryTokens.begin();
    
    while(iter != queryTokens.end()) {
        string queryToken = (*iter);
        size_t index = queryToken.find_first_of("=");
        
        if(index != string::npos) {
            string name  = queryToken.substr(0,index);
            string value = queryToken.substr(index + 1);
            addFormField(name,value);
        } else {
            addFormField(queryToken);
        }
        ++iter;
    }
}


} } }
