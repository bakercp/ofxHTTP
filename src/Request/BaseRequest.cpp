#include "BaseRequest.h"


namespace ofx {
namespace HTTP {
namespace Request {


//------------------------------------------------------------------------------
BaseRequest::BaseRequest(const std::string& httpMethod,
                         const Poco::URI& uri) :
    _httpMethod(httpMethod),
    _httpVersion(Poco::Net::HTTPRequest::HTTP_1_1),
    _uri(uri)
{
    setFormFieldsFromURI(uri);
}

//------------------------------------------------------------------------------
BaseRequest::BaseRequest(const std::string& httpMethod,
                         const Poco::URI&   uri,
                         const std::string& httpVersion) :
    _httpMethod(httpMethod),
    _httpVersion(httpVersion),
    _uri(uri)
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
    return _httpMethod;
}

//------------------------------------------------------------------------------
std::string BaseRequest::getVersion() const
{
    return _httpVersion;
}

//------------------------------------------------------------------------------
Poco::URI BaseRequest::getURI() const
{
    return _uri;
}

//------------------------------------------------------------------------------
void BaseRequest::addCookie(const std::string& name,
                            const std::string& value,
                            bool bIsValueEscaped)
{
    std::string escapedValue = value;

    if(!bIsValueEscaped) {
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
    std::vector<Cookie>::iterator iter = _cookies.begin();
    while(iter != _cookies.end()) {
        if((*iter).matches(cookie)) {
            iter = _cookies.erase(iter);
        } else {
            ++iter;
        }
    }
    
    if(!cookie.isExpired()) {
        _cookies.push_back(cookie);
    }
}

//------------------------------------------------------------------------------
void BaseRequest::addCookies(const std::vector<Cookie>& cookies)
{
    std::vector<Cookie>::const_iterator iter = cookies.begin();
    while(iter != cookies.end()) {
        addCookie(*iter);
        ++iter;
    }
}

//------------------------------------------------------------------------------
void BaseRequest::clearCookies()
{
    _cookies.clear();
}

//------------------------------------------------------------------------------
void BaseRequest::addFormField(const std::string& name,
                               const std::string& value)
{
    _form.add(name, value);
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
    return !_form.empty();
}

//------------------------------------------------------------------------------
bool BaseRequest::hasFormField(const std::string& name) const
{
    return _form.has(name);
}

//------------------------------------------------------------------------------
void BaseRequest::removeFormField(const std::string& name)
{
    _form.erase(name);
}

//------------------------------------------------------------------------------
void BaseRequest::clearFormFields()
{
    _form.clear();
}

//------------------------------------------------------------------------------
void BaseRequest::addHeader(const std::string& name, const std::string& value)
{
    _headers.add(name,value);
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
    _headers.erase(name);
}

//------------------------------------------------------------------------------
bool BaseRequest::hasHeaders() const
{
    return !_headers.empty();
}

//------------------------------------------------------------------------------
bool BaseRequest::hasHeader(const std::string& name) const
{
    return _headers.has(name);
}

//------------------------------------------------------------------------------
void BaseRequest::clearHeaders()
{
    _headers.clear();
}

//------------------------------------------------------------------------------
void BaseRequest::setFormFieldsFromURI(const Poco::URI& uri)
{
    // attempt to extract uri query as form fields
    std::vector<string> queryTokens = ofSplitString(uri.getQuery(),"&",true,true);
    
    std::vector<string>::const_iterator iter = queryTokens.begin();
    while(iter != queryTokens.end()) {
        std::string queryToken = (*iter);
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

//------------------------------------------------------------------------------
void BaseRequest::sendRequestBody(std::ostream& requestStream) const
{
}


} } }
