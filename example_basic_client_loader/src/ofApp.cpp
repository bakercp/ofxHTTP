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


#include "ofApp.h"


void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_NOTICE);

    // Register for all SSL events.
    ofSSLManager::registerAllEvents(this);

    // Register for all client events.
    clientTaskQueue.registerTaskEvents(this);

    // Lauch three large download tasks.
    for (int i = 0; i < 3; ++i)
    {
        std::string name = "http://peach.blender.org/wp-content/uploads/big_big_buck_bunny.jpg";
        Poco::UUID uuid = clientTaskQueue.get(name);

        // Keep local track of the task's progress.
        TaskProgress task;
        task.name = name;
        task.uuid = uuid;
        tasks[uuid] = task;
    }

}

void ofApp::exit()
{
    ofSSLManager::unregisterAllEvents(this);
}


void ofApp::update()
{
    std::map<Poco::UUID, TaskProgress>::iterator iter = tasks.begin();

    unsigned long long now = ofGetElapsedTimeMillis();

    while (iter != tasks.end())
    {
        if (iter->second.progress < 0 && now > iter->second.autoClearTime)
        {
            tasks.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}


void ofApp::draw()
{
    ofBackground(0);

    std::map<Poco::UUID, TaskProgress>::iterator iter = tasks.begin();

    int y = 0;
    int height = 20;

    while (iter != tasks.end())
    {
        TaskProgress& t = iter->second;

        t.height = height;
        t.width = ofGetWidth();
        t.x = 0;
        t.y = y;

        t.draw();

        y += (height + 5);
        
        ++iter;
    }
}


void ofApp::keyPressed(int key)
{
    if ('c' == key)
    {
        clientTaskQueue.cancelAll();
    }
}


void ofApp::onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogVerbose("ofApp::onServerVerificationError") << args.errorMessage();
}


void ofApp::onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogError("ofApp::onClientVerificationError") << args.errorMessage();
}


void ofApp::onSSLPrivateKeyPassphraseRequired(std::string& args)
{
    ofLogError("ofApp::onPrivateKeyPassphraseRequired") << args;
}


void ofApp::onTaskStarted(const ofx::TaskStartedEventArgs& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks[args.getTaskId()].progress = 0.000001; // Just give it a nudge.
    }
    else
    {
        ofLogFatalError("ofApp::onTaskCancelled") << "Unknown UUID.";
    }
}


void ofApp::onTaskCancelled(const ofx::TaskCancelledEventArgs& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks[args.getTaskId()].progress = -1;
    }
    else
    {
        ofLogFatalError("ofApp::onTaskCancelled") << "Unknown UUID.";
    }
}


void ofApp::onTaskFinished(const ofx::TaskFinishedEventArgs& args)
{
    std::map<Poco::UUID, TaskProgress>::iterator iter = tasks.find(args.getTaskId());

    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        if (iter->second.progress < 0)
        {
            // There was an error, so let it be here for just a few seconds.
            tasks[args.getTaskId()].autoClearTime = ofGetElapsedTimeMillis() + 2000;
        }
        else
        {
            tasks.erase(iter);
        }
    }
    else
    {
        ofLogFatalError("ofApp::onTaskFinished") << "Unknown UUID.";
    }
}


void ofApp::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks[args.getTaskId()].progress = -1;
        tasks[args.getTaskId()].message = args.getException().displayText();
    }
    else
    {
        ofLogFatalError("ofApp::onTaskFailed") << "Unknown UUID.";
    }
}


void ofApp::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        tasks[args.getTaskId()].progress = args.getProgress();
    }
    else
    {
        ofLogFatalError("ofApp::onTaskProgress") << "Unknown UUID.";
    }
}


void ofApp::onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args)
{
    if (tasks.find(args.getTaskId()) != tasks.end())
    {
        ofBuffer buffer(reinterpret_cast<const char*>(args.getData().getBuffer().getDataPtr()),
                        args.getData().getBuffer().size());

        ofBufferToFile("out_" + ofToString(ofRandom(100000)) + ".jpg", buffer);
    }
    else
    {
        ofLogFatalError("ofApp::onTaskData") << "Unknown UUID.";
    }
}
