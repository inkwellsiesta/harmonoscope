#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // set the size of the window
    ofSetWindowShape(750, 750);
    //ofSetVerticalSync(true);
    
    ofSetFrameRate(40);
    ofBackground(40);
    
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
    
    // setup the gui objects
    pitchGui.setup("ofxAubioPitch", "settings.xml", 510, 10);
    pitchGui.add(midiPitch.setup( "midi pitch", 0, 0, 128));
    pitchGui.add(pitchConfidence.setup( "confidence", 0, 0, 1));
    
    clientPitchGui.setup("clientPitch", "settings.xml", 10, 10);
    clientPitchGui.add(clientPitchSlider.setup("midi pitch", 0, 0, 128));
    clientPitchGui.add(clientConfidenceSlider.setup("confidence", 0, 0, 1));
    
    // setup UDP object
    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(11999);
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
}

//--------------------------------------------------------------
void ofApp::draw(){
    udpConnection.Send("sendpitch", 9);
    
    char udpMessage[100];
    while (udpConnection.Receive(udpMessage, 100) == 0) {}
    
    // update pitch info
    pitchConfidence = pitch.pitchConfidence;
    if (pitch.latestPitch) midiPitch = pitch.latestPitch;
    
    int hzPitch = pow(2, (midiPitch - 69.)/12.)*440.;
    
    string message=udpMessage;
    cout << message << endl;
    
    int pos = message.find('\t');
    if (pos >= 0) {
        string confStr = message.substr(0, pos);
        string pitchStr = message.substr(pos+1);
        //  cout << confStr << ", " << pitchStr;
        clientPitchSlider = ofFromString<float>(pitchStr);
        int clientHzPitch = pow(2, (clientPitchSlider - 69.)/12.)*440.;
        clientConfidenceSlider = ofFromString<float>(confStr);
        
        //      clientPitchSlider = pow(2, (55. - 69.)/12.)*440.;
        //     clientConfidenceSlider = 1.f;
        
        
        
        filteredPitchConfidence.push_back(pitchConfidence);
        if (filteredPitchConfidence.size() > FILTER_SIZE) filteredPitchConfidence.pop_front();
        
        float avgConfidence = 0.;
        for (int i = 0; i < filteredPitchConfidence.size(); ++i) {
            avgConfidence+=filteredPitchConfidence[i];
        }
        avgConfidence/=filteredPitchConfidence.size();
        
        pitchPlot.clear();
        float confidenceThreshold = .3;
        float dt = .005;
        for (float i = PI * PI / 69.; i < TWO_PI + PI * PI / 69.; i+=dt) {
            float x = clientConfidenceSlider < confidenceThreshold ? ofGetWidth()/2. : (clientConfidenceSlider * sin(clientHzPitch / TWO_PI * i) + 1.)*ofGetWidth()/2.;
            float y = avgConfidence < confidenceThreshold ? ofGetHeight()/2. : (avgConfidence * sin(hzPitch / TWO_PI * i) + 1.)*ofGetHeight()/2.;
            
            pitchPlot.curveTo(ofVec2f(x,y));
        }
        
        for (float width = 1; width < 50; width*=2)
        {
            ofSetLineWidth(avgConfidence*width);
            ofSetColor(255, 100/width);
            pitchPlot.draw();
        }
        
        // draw
        pitchGui.draw();
        clientPitchGui.draw();
    }
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