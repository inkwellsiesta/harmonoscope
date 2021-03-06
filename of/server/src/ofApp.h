#pragma once

#include "ofMain.h"
#include "ofxAubio.h"
#include "ofxGui.h"
#include "ofxNetwork.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void exit();
    
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void audioIn(float * input, int bufferSize, int nChannels);
    void audioOut();
    
private:
    bool noConnection = false;
    float avgConfidence, hzPitch, clientHzPitch;
    ofxAubioPitch pitch;
    
    ofxUDPManager udpConnection;
    
    ofxPanel pitchGui, clientPitchGui;
    ofxFloatSlider midiPitch, clientPitchSlider;
    ofxFloatSlider pitchConfidence, clientConfidenceSlider;
    
    deque<float> filteredPitchConfidence;
    deque<float> filteredPitchMidi;
    const int FILTER_SIZE = 10;
    
    char udpMessage[100];
    ofPolyline pitchPlot;
    
};