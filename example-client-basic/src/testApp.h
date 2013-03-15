#pragma once

#include "ofMain.h"

#include "Poco/URI.h"

//#include "ofxHTTPResponseHandler.h"
#include "ofxHTTPClient.h"
#include "ofxHTTPGet.h"
//#include "ofxHTTPResponse.h"

#include "ofxHTTPStreamUtils.h"
using Poco::URI;




////
#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/FTPStreamFactory.h"
#include <memory>
#include <iostream>


using Poco::URIStreamOpener;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPStreamFactory;
using Poco::Net::FTPStreamFactory;

///

class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
        ofxHTTPClient client;
    
};
