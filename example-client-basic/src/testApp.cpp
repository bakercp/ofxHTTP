#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    string url = "http://koivi.com/archives/php-http-auth/protect.php";

    ofxHTTPGet get(url);

    client.setCredentials(ofxHTTPAuthScope(get.getURI()),ofxHTTPCredentials("tester","testing"));

    ofxHTTPResponseStream::Ptr response = client.exectute(get);
    
    ofxHTTPStreamUtils::consume(response);
    
    cout << "===========" << endl;
    response = client.exectute(get);

    
    if(!response->hasException()) {
        ofxHTTPStreamUtils::copyTo(*response->getResponseStream(), cout);
    } else {
        cout << response->getException()->message() << endl;
    }

    
//    
//    
//    NameValueCollection headers = response->getHeaders();
//    
//    NameValueCollection::ConstIterator iter = headers.begin();
//    
//    while(iter != headers.end()) {
//        cout << "HEADER: " << (*iter).first << "=" << (*iter).second << endl;
//        ++iter;
//    }
//
//    vector<HTTPCookie> cookies = response->getCookies();
//    
//    for(int i = 0; i < cookies.size(); i++) {
//        cout << "COOKIE: " << cookies[i].toString() << endl;
//    }
////
//
//    if(!response->hasException()) {
//        NullOutputStream null;
//
//        StreamCopier::copyStream(*response->getResponseStream(), cout);
//        
//    } else {
//        cout << response->getException()->message() << endl;
//    }
//    cout << "=========================================================" << endl;
//    ofxHTTPGet get1(url);
//    ofxHTTPResponseStream::Ptr response2 = client.exectute(get1);
//    if(!response2->hasException()) {
//        NullOutputStream null;
//        StreamCopier::copyStream(*response2->getResponseStream(), cout);
//    } else {
//        cout << response2->getException()->message() << endl;
//    }

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}