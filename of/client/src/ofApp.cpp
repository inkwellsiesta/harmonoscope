#include "ofApp.h"
#include <stdio.h>

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFrameRate(15);
    // set the size of the window
    // ofSetWindowShape(750, 750);
    // ofBackground(40);
    
    //ofSetVerticalSync(true);
    ofSetFrameRate(30);
    
    int nOutputs = 2;
    int nInputs = 2;
    int sampleRate = 44100;
    int bufferSize = 256;
    //int nBuffers = 4;
    
    // setup pitch object
    pitch.setup();
    //pitch.setup("schmitt", 8 * bufferSize, bufferSize, sampleRate); // yinfft
    
    ofSoundStreamSetup(nOutputs, nInputs, this);
    //ofSoundStreamSetup(nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
    ofSoundStreamListDevices();
    
    
    // setup UDP object    //create the socket and set to send to 127.0.0.1:11999
	udpConnection.Create();
	udpConnection.Connect("127.0.0.1",11999);
	udpConnection.SetNonBlocking(true);
    
}

void ofApp::exit(){
    ofSoundStreamStop();
    ofSoundStreamClose();
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    // compute pitch detection
    pitch.audioIn(input, bufferSize, nChannels);
}

void ofApp::audioOut(){
}

//--------------------------------------------------------------
void ofApp::update(){
    
    char udpMessage[100];
    string message;
    while (message.compare("sendpitch") != 0) {
        cout << message.compare("sendpitch");
        //cout << "looping\n";
        cout << message;
        udpConnection.Receive(udpMessage, 100);
        message = udpMessage;
    }
    
    // update pitch info
    pitchConfidence = pitch.pitchConfidence;
    if (pitch.latestPitch) midiPitch = pitch.latestPitch;
    
    filteredPitchConfidence.push_back(pitchConfidence);
    if (filteredPitchConfidence.size() > FILTER_SIZE) filteredPitchConfidence.pop_front();
    
    float avgConfidence = 0.;
    for (int i = 0; i < filteredPitchConfidence.size(); ++i) {
        avgConfidence+=filteredPitchConfidence[i];
    }
    avgConfidence/=filteredPitchConfidence.size();
    
    string acStr = ofToString(avgConfidence);
    acStr.append("\t");
    acStr.append(ofToString((float)midiPitch));
    acStr.append("\n");
    
    udpConnection.Send(acStr.c_str(), acStr.length());
}

//--------------------------------------------------------------
void ofApp::draw(){
    //  cout << "Framerate = " << ofGetFrameRate() << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}