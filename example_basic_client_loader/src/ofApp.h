// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include "ofMain.h"
#include "ofxHTTP.h"


/// \brief A lame and hacky way of keep track of task progress.
class TaskProgress: public ofRectangle
{
public:
    TaskProgress(): progress(0), autoClearTime(0)
    {
    }

    void draw()
    {
        ofPushMatrix();
        ofTranslate(x, y);
        ofFill();

        if (progress < 0) // Failed.
        {
            ofSetColor(255, 0, 0);
        }
        else if (progress > 0)
        {
            ofSetColor(0, 255, 0, 50);
        }
        else
        {
            ofSetColor(255, 80);
        }

        ofRect(0, 0, width, height);

        if (progress > 0)
        {
            ofFill();
            ofSetColor(255, 255, 0, 75);
            ofRect(0, 0, progress * width, height);
        }

        ofSetColor(255);
        ofDrawBitmapString(name + " " + ofToString(progress * 100) + "%: " + message, ofPoint(10, 14, 0));

        ofPopMatrix();
    }

    std::string name;
    Poco::UUID uuid;
    float progress;
    std::string message;
    unsigned long long autoClearTime;
    
};


class ofApp: public ofBaseApp
{
public:
    void setup();
    void exit();
    void update();
    void draw();

    void keyPressed(int key);

    void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLPrivateKeyPassphraseRequired(std::string& args);

    void onTaskStarted(const ofx::TaskStartedEventArgs& args);
    void onTaskCancelled(const ofx::TaskCancelledEventArgs& args);
    void onTaskFinished(const ofx::TaskFinishedEventArgs& args);
    void onTaskFailed(const ofx::TaskFailedEventArgs& args);
    void onTaskProgress(const ofx::TaskProgressEventArgs& args);
    void onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args);

    // A local map of the tasks that we have submitted.
    std::map<Poco::UUID, TaskProgress> tasks;

    /// \brief An HTTP client task queue.
    ofx::HTTP::DefaultClientTaskQueue clientTaskQueue;

};