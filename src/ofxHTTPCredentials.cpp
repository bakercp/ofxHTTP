#include "ofxHTTPCredentials.h"

//------------------------------------------------------------------------------
ofxHTTPCredentials::ofxHTTPCredentials(const string& _username, const string& _password) :
username(_username),
password(_password)
{ }

//------------------------------------------------------------------------------
ofxHTTPCredentials::ofxHTTPCredentials() :
username(""),
password("")
{ }

//------------------------------------------------------------------------------
ofxHTTPCredentials::~ofxHTTPCredentials() { }

//------------------------------------------------------------------------------
void ofxHTTPCredentials::clear() {
    username = "";
    password = "";
}

//------------------------------------------------------------------------------
string ofxHTTPCredentials::getUsername() const { return username; }

//------------------------------------------------------------------------------
string ofxHTTPCredentials::getPassword() const { return password; }

//------------------------------------------------------------------------------
void ofxHTTPCredentials::setUsername(const string& _username) { username = _username; }

//------------------------------------------------------------------------------
void ofxHTTPCredentials::setPassword(const string& _password) { password = _password; }

//------------------------------------------------------------------------------
bool ofxHTTPCredentials::hasCredentials() const {
    return !username.empty() || !password.empty();
}

//------------------------------------------------------------------------------
string ofxHTTPCredentials::toString() const {
    return username + ":" + password;
}