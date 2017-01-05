//
// Copyright (c) 2012 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


int main()
{
    ofSetupOpenGL(320, 240, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
