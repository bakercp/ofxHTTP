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
#include "ofxIO.h"
#include "ofxHTTP.h"


/// \brief A small class to help us keep track of our tasks' progress.
class TaskProgress
{
public:
    enum State
    {
        PENDING,
        SUCCESS,
        FAILURE
    };

    TaskProgress():
        progress(0),
        state(PENDING), 
        fade(100)
    {
    }

    std::string name;
    Poco::UUID uuid;
    float progress;
    std::string message;
    State state;
    int fade;

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

    void onTaskQueued(const ofx::TaskQueuedEventArgs& args);
    void onTaskStarted(const ofx::TaskStartedEventArgs& args);
    void onTaskCancelled(const ofx::TaskCancelledEventArgs& args);
    void onTaskFinished(const ofx::TaskFinishedEventArgs& args);
    void onTaskFailed(const ofx::TaskFailedEventArgs& args);
    void onTaskProgress(const ofx::TaskProgressEventArgs& args);
    void onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args);

    typedef std::map<Poco::UUID, TaskProgress> TaskMap;

    // A local map of the tasks that we have submitted.
    TaskMap tasks;

    /// \brief An HTTP client task queue.
    ofx::HTTP::DefaultClientTaskQueue clientTaskQueue;

};