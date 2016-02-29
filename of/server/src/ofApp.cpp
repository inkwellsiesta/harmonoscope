#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    noConnection = true;
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
    if (noConnection) {
        clientHzPitch = pow(2, (55. - 69.)/12.)*440.;
        clientConfidenceSlider = 1.f;
    }
    else  {
        char messageOut[20];
        sprintf(messageOut, "%llu\n", ofGetSystemTime());
        //cout << messageOut << endl;
        udpConnection.Send(messageOut, 20);
        
        string message;
        int pos = -1;
        udpConnection.Receive(udpMessage, 100);
        message = udpMessage;
        
        pos = message.find('\t');
        if (pos >= 0) {
            string confStr = message.substr(0, pos);
            string pitchStr = message.substr(pos+1);
            
            //  cout << confStr << ", " << pitchStr;
            clientPitchSlider = ofFromString<float>(pitchStr);
            clientHzPitch = pow(2, (clientPitchSlider - 69.)/12.)*440.;
            clientConfidenceSlider = ofFromString<float>(confStr);
        }
    }
    // update pitch info
    if (pitch.latestPitch) midiPitch = pitch.latestPitch;
    
    // filter using an average window
    filteredPitchMidi.push_back(midiPitch);
    if (filteredPitchMidi.size() > FILTER_SIZE) filteredPitchMidi.pop_front();
    float avgMidi = 0.;
    for (float eachMidi : filteredPitchMidi) {
        avgMidi+=eachMidi;
    }
    avgMidi/=filteredPitchMidi.size();
    cout << avgMidi << " vs " << midiPitch << endl;
    // Convert from midi to actual frequency
    hzPitch = pow(2, (avgMidi - 69.)/12.)*440.;
    
    
    pitchConfidence = pitch.pitchConfidence;
    // filter using an average window
    filteredPitchConfidence.push_back(pitchConfidence);
    if (filteredPitchConfidence.size() > FILTER_SIZE) filteredPitchConfidence.pop_front();
    avgConfidence = 0.;
    for (int i = 0; i < filteredPitchConfidence.size(); ++i) {
        avgConfidence+=filteredPitchConfidence[i];
    }
    avgConfidence = 1;// /=filteredPitchConfidence.size();
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    pitchPlot.clear();
    float confidenceThreshold = .3;
    float dt = .001;
    float decay = -.05;
    for (float i = HALF_PI; i < TWO_PI + HALF_PI; i+=dt) {
        float x = clientConfidenceSlider < confidenceThreshold ? ofGetWidth()/2. : (clientConfidenceSlider * sin(clientHzPitch / TWO_PI * i)*exp(i*decay) + 1.)*ofGetWidth()/2.;
        float y = avgConfidence < confidenceThreshold ? ofGetHeight()/2. : (avgConfidence * sin(hzPitch / TWO_PI * i)*exp(i*decay) + 1.)*ofGetHeight()/2.;
        
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