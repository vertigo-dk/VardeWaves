#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOscParameterSync.h"

#include "defines.h"
#include "soundWaveSystem.h"
#include "blink.h"
#include "boubble.h"
#include "dancingLine.h"
#include "waveSystem.h"
#include "flockSystem.h"






class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void ofRectGradient(int px, int py, int w, int h,const ofColor& start, const ofColor& end, ofGradientMode mode);
    
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    ofxPanel gui;
    ofParameterGroup WaveControl;
    
    ofParameter<float> inLeft;
    ofParameter<float> inRight;
    ofParameter<float> noiseAmt;
    
    ofParameter<float> posHWavePoles;
    ofParameter<float> posHWaveRamp;
    ofParameter<float> posHLinePoles;
    ofParameter<float> posHLineRamp;
    
    
    
    
    ofParameter<ofColor> colorLine;
    ofParameter<ofColor> colorTopTop;
    ofParameter<ofColor> colorTopBot;
    ofParameter<ofColor> colorBotTop;
    ofParameter<ofColor> colorBotBot;
    
    ofParameter<float> updateRequest;
    
    ofxOscParameterSync syncOSC;
    
    
    
    
    //ofFbo grafic_RGB_Pole;
    //ofFbo grafic_RGB_Ramp;
    
    ofFbo grafic_W_Pole;
    ofFbo grafic_W_Ramp;
    
    ofFbo render;
    ofxSyphonServer syphonRenderOut;
    ofTexture syphonTex;
    
    //WaveArray wave;
    
    WaveParticleSystem wave_poles;
    WaveParticleSystem wave_ramp;
    
    // Blinkende Lyger
    // GUI / Controlpanel
    ofParameter<float> blinkTempo;
    ofParameter<float> blinkIntensity;
    ofParameter<bool> hard_soft;
    ofParameter<ofColor> colorBlink;
    // BlinkendeLygter
    vector<Blink> blinks;
    ofFbo graficBlinkendeLygter;
    
    
    // Boubbles Lyger
    // GUI / Controlpanel
    ofParameter<float> boubblesIntensity;
    ofParameter<float> boubblesVelMin;
    ofParameter<float> boubblesVelMax;
    
    ofParameter<ofColor> colorBoubbles;
    // BlinkendeLygter
    vector<Boubble> boubbles;
    ofFbo graficBoubbles;
    
    
    // dancing Lines
    // GUI / Controlpanel
    ofParameter<float> dLinesSpeed;
    ofParameter<float> dLinesSync;

    ofParameter<ofColor> colorDLines;
    
    vector<DancingLine> dancingLinesPole;
    vector<DancingLine> dancingLinesRamp;
    
    float counterDLines;
    
    ofFbo fboTexPoles;
    ofFbo fboTexRamp;
    
    // drops
    ofParameter<float> dropsVelMin;
    ofParameter<float> dropsVelMax;
    ofParameter<float> dropsDragLimit;
    ofParameter<float> dropsExtraMass;
    ofParameter<ofColor> colorDrops;
    
    
    // soundWaves (mic input)
    ofxOscReceiver receiveOscTriggers;

    SoundWaveSystem sWSystem;

    // Flocking
    FlockSystem flockSystem;
    
    ofParameterGroup Flock;
    
    ofParameter<float> flockIntensity;
    ofParameter<float> flockIntensityDay;

    ofParameter<float> maxForce;
    ofParameter<float> maxSpeed;
    ofParameter<float> desiredSeparation;
    ofParameter<float> neighbourDist;
    ofParameter<float> weightSeparation;
    ofParameter<float> weightAlign;
    ofParameter<float> weightCohesion;
    ofParameter<float> characterUser;
    ofParameter<float> attractionUser;
    ofParameter<float> repulsionUser;
    ofParameter<float> minDistUser;
    
    ofParameter<int> numOfBoids;
    ofParameter<ofColor> colorFlock;

    int numOfBoidsLast;
    
    // Create Mask
    
    ofVec2f cursor;
    ofImage pixelMask;
    bool editMask;


};

