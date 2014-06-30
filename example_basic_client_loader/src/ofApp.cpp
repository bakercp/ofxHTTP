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
        std::string url = "http://peach.blender.org/wp-content/uploads/big_big_buck_bunny.jpg";

        // We can use this taskId to cancel the task or keep track of its
        // progress.  In this example, we register the taskId in the
        // taskStarted() callback.
        Poco::UUID uuid = clientTaskQueue.get(url);
    }

}

void ofApp::exit()
{
    ofSSLManager::unregisterAllEvents(this);
}


void ofApp::update()
{
    TaskMap::iterator iter = tasks.begin();

    while (iter != tasks.end())
    {
        TaskProgress& t = iter->second;

        if (t.state != TaskProgress::PENDING)
        {
            t.fade--;
        }

        if (t.fade < 0)
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

    TaskMap::const_iterator iter = tasks.begin();

    int y = 0;
    int height = 20;
    int width = ofGetWidth();

    while (iter != tasks.end() && y < ofGetHeight())
    {
        const TaskProgress& t = iter->second;

        ++iter;

        ofPushMatrix();
        ofTranslate(0, y);
        ofFill();

        switch (t.state)
        {
            case TaskProgress::PENDING:
                ofSetColor(0, 255, 0, 50);
                break;
            case TaskProgress::SUCCESS:
                ofSetColor(255, 255, 0, 50);
                break;
            case TaskProgress::FAILURE:
                ofSetColor(255, 0, 0);
                break;
        }

        ofRect(0, 0, width, height);

        if (t.progress > 0)
        {
            ofFill();
            ofSetColor(255, 255, 0, 75);
            ofRect(0, 0, t.progress * width, height);
        }

        ofSetColor(255);

        std::stringstream ss;

        ss << t.name + " " << (t.progress * 100) << "%: " << t.message;

        ofDrawBitmapString(ss.str(), ofPoint(10, 14, 0));
        
        ofPopMatrix();

        y += (height + 1);
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


void ofApp::onTaskQueued(const ofx::TaskQueuedEventArgs& args)
{
    // Make a record of the task so we can keep track of its progress.
    TaskProgress task;
    task.name = args.getTaskName();
    task.uuid = args.getTaskId();
    tasks[task.uuid] = task;
}


void ofApp::onTaskStarted(const ofx::TaskStartedEventArgs& args)
{
}


void ofApp::onTaskCancelled(const ofx::TaskCancelledEventArgs& args)
{
    tasks[args.getTaskId()].state = TaskProgress::FAILURE;
}


void ofApp::onTaskFinished(const ofx::TaskFinishedEventArgs& args)
{
    if (tasks[args.getTaskId()].state == TaskProgress::PENDING)
    {
        tasks[args.getTaskId()].progress = 1;
        tasks[args.getTaskId()].state = TaskProgress::SUCCESS;
    }
}


void ofApp::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{
    tasks[args.getTaskId()].state = TaskProgress::FAILURE;
    tasks[args.getTaskId()].message = args.getException().displayText();
}


void ofApp::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
    tasks[args.getTaskId()].progress = args.getProgress();
}


void ofApp::onTaskData(const ofx::TaskDataEventArgs<ofx::HTTP::ClientResponseBufferEventArgs>& args)
{
    // Note: Saving to disk could / should also be done in the task's thread.

    // This is useful if you want to load the bytes into a GL texture.
    const ofx::IO::ByteBuffer& buffer = args.getData().getByteBuffer();

    std::string path = ofToDataPath(args.getTaskId().toString() + ".jpg");

    try
    {
        ofx::IO::ByteBufferUtils::saveToFile(buffer, path);
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofApp::onTaskData") << exc.displayText();
    }
}
