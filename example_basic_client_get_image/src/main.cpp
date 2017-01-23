//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


int main()
{
    ofSetupOpenGL(100, 100, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
