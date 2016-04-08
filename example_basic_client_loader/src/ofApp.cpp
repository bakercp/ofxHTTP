// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
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
    // Hack to make sure that the net sybsystem is initialized on windows.
    Poco::Net::initializeNetwork();
   
    // Register for client events.
    clientTaskQueue.registerAllEvents(this);

    // Lauch three large download tasks.
    for (int i = 0; i < 3; ++i)
    {
        std::string url = "http://peach.blender.org/wp-content/uploads/big_big_buck_bunny.jpg";

        // We can use this taskId to cancel the task or keep track of its
        // progress.  In this example, we register the taskId in the
        // taskStarted() callback.

        std::string uuid = clientTaskQueue.get(url);

        std::cout << "Getting: " << url << std::endl;
    }

}


void ofApp::draw()
{
    ofBackground(0);

    std::stringstream ps;

    ofx::HTTP::DefaultClientTaskQueue::ProgressMap progress = clientTaskQueue.getTaskProgress();

    ofx::HTTP::DefaultClientTaskQueue::ProgressMap::const_iterator progressIter = progress.begin();

    int height = 20;
    int y = height * 2;

    while (progressIter != progress.end() && y < ofGetHeight())
    {
        const ofx::TaskProgressEventArgs& progressInfo = progressIter->second;

        float progress = progressInfo.getProgress();

        std::string taskId = progressInfo.getTaskId();

        std::string name = progressInfo.getTaskName();

        ofColor color;

        std::string statusString;

        switch (progressInfo.getState())
        {
            case Poco::Task::TASK_IDLE:
                color = ofColor(127);
                statusString = "idle";
                break;
            case Poco::Task::TASK_STARTING:
                color = ofColor(255, 255, 0);
                statusString = "starting";
                break;
            case Poco::Task::TASK_RUNNING:
                color = ofColor(255, 127, 127);
                statusString = "running";
                break;
            case Poco::Task::TASK_CANCELLING:
                color = ofColor(255, 0, 0);
                statusString = "cancelling";
                break;
            case Poco::Task::TASK_FINISHED:
                color = ofColor(0, 255, 0);
                statusString = "finished";
                break;
        }

        std::stringstream ss;

        ss << taskId << ": " << statusString << " : " << name << std::endl;

        ofPushMatrix();
        ofTranslate(0, y);

        ofFill();
        ofSetColor(color, 127);
        ofDrawRectangle(0, 0, ofGetWidth() * progress, height - 2);

        ofNoFill();
        ofSetColor(color);
        ofDrawRectangle(0, 0, ofGetWidth() * progress, height - 2);

        ofFill();
        ofSetColor(255);
        ofDrawBitmapString(ss.str(), 4, 14);
        
        ofPopMatrix();
        
        y += height;
        
        ++progressIter;
    }

    ofDrawBitmapString(ps.str(), 10, ofGetHeight() / 2);

}


void ofApp::keyPressed(int key)
{
    if ('c' == key)
    {
        clientTaskQueue.cancelAll();
    }
}


void ofApp::onTaskQueued(const ofx::TaskQueueEventArgs& args)
{
}


void ofApp::onTaskStarted(const ofx::TaskQueueEventArgs& args)
{
}


void ofApp::onTaskCancelled(const ofx::TaskQueueEventArgs& args)
{
}


void ofApp::onTaskFinished(const ofx::TaskQueueEventArgs& args)
{
}


void ofApp::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{
    std::cout << "Task Failed: " << args.getException().displayText() << std::endl;
    std::cout << "\t" << args.getTaskName() << std::endl;
}


void ofApp::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
}


void ofApp::onClientBuffer(const ofx::HTTP::ClientBufferEventArgs& args)
{
    // Note: Saving to disk could / should also be done in the task's thread.

    // This is useful if you want to load the bytes into an openGL texture.
    const ofx::IO::ByteBuffer& buffer = args.getData().buffer();

    ofx::MediaTypeMap::SharedPtr mtm = ofx::MediaTypeMap::getDefault();

    Poco::Net::MediaType mediaType(args.getData().response().getContentType());

    std::string extension = mtm->getBestFileExtensionsForMediaType(mediaType);

    std::string path = ofToDataPath(args.getTaskId() + "." + extension);

    try
    {
        ofx::IO::ByteBufferUtils::saveToFile(buffer, path);
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("ofApp::onTaskData") << exc.displayText();
    }
}
