#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOscParameterSync.h"

#include "defines.h"

class Lygte{
public:
    ofVec2f location;
    ofColor lygteColor;
    float tempo;
    bool hard_soft = false;
    
    float lifespan;
    
    Lygte() {
        lifespan = 1.0;
        tempo = 0.01;
    }
    
    // Method to update location
    void update() {
        lifespan -= tempo;
        
    }
    // Method to display
    void draw() {
        
        float alpha;
        if(hard_soft){
            alpha = (sin(PI*(1-lifespan)+HALF_PI)+1)/2;
        }else{
            alpha = (cos(PI*(1-lifespan))+1)/2;
            
        }
        
        lygteColor.a = alpha*255;
        ofSetColor(lygteColor);
        ofFill();
        ofSetLineWidth(0.);
        
        ofDrawRectangle(location.x,location.y, 1,1);
        //ofLine(location.x,location.y, location.x+5, location.y);
        //ofCircle(location.x,location.y,8);
    }
    
    // Is the particle still useful?
    bool isDead() {
        if (lifespan < 0.0) {
            return true;
        } else {
            return false;
        }
    }
    
};

class Boubble{
public:
    ofVec2f location;
    ofVec2f velocity;
    ofColor boubbleColor;
    
    Boubble() {
        velocity = ofVec2f(0., - ofRandom(0.1, 2.));
    }
    
    // Method to update location
    void update() {
        location.operator+=(velocity);
        
    }
    // Method to display
    void draw() {
        
        
        ofSetColor(boubbleColor);
        ofFill();
        ofSetLineWidth(0.);
        ofDrawRectangle(location.x,location.y, 1,1);

    }
    
    // Is the particle still useful?
    bool isDead() {
        if(location.y < 0){
            return true;
        } else {
            return false;
        }
    }
    
};

class DancingLine{
public:
    ofVec2f location1;
    ofVec2f location2;
    float offset1, offset2;
    float   freedom;
    float speed;
    float seedOffset;

    ofColor lineColor;
    
    DancingLine() {
        speed = 4;
        lineColor = ofColor(0);
        seedOffset = ofRandom(1000);
        freedom = 100;
    }
    
    // Method to update location
    void update(float _timer) {
        offset1 = freedom * ((ofNoise(_timer*speed+seedOffset))*1.8-0.4);
        offset2 = freedom * ((ofNoise(_timer*1.5*speed+seedOffset))*1.8-0.4);

    }
    // Method to display
    void draw() {
        
        
        ofSetColor(lineColor);
        ofSetLineWidth(1.);
        ofDrawLine(location1.x, location1.y+offset1,location2.x, location2.y+offset2);
        
    }
};

class WaveParticle {
    
    // Inspired by http://vvvv.org/documentation/wave-simulation
public:
    float p;
    float v;
    float a;
    float attack;
    float damping;
    
    void setResponse(float _attack, float _damping){
        attack = _attack;
        damping = _damping;
    }
    
    void calcAccel(float p_left, float p_right);
    
    void update( float p_left, float p_right ){
        float delta_p = p_left + p_right - 2*p;
        a = attack * delta_p;
        v = (v + a) * damping;
        p = p + v;
    }
};


class WaveParticleSystem{
public:
    vector<WaveParticle> waveParticles;
    
    void setup();
    void updateResponse(float _attack, float _damping);
    void update(float _inLeft, float _inRight);
    void drawLine(int _x, int _y,int _w, int _h, ofColor colorLine, float _posHLine, int _lineWidth);
    void drawGradient(int _x, int _y, int _w, int _h, float _posHWave , ofTexture& _texture);
    
};




class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
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
    ofParameterGroup visualControl;
    ofParameter<float> inLeft;
    ofParameter<float> inRight;
    ofParameter<float> noiseAmt;
    
    ofParameter<float> posHWavePoles;
    ofParameter<float> posHWaveRamp;
    ofParameter<float> posHLinePoles;
    ofParameter<float> posHLineRamp;
    
    
    
    
    ofParameter<ofColor> colorBG;
    ofParameter<ofColor> colorLine;
    ofParameter<ofColor> colorBGTop;
    ofParameter<ofColor> colorBGBot;
    ofParameter<ofColor> colorWaveTop;
    ofParameter<ofColor> colorWaveBot;
    
    
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
    ofParameterGroup paramBlinkendeLyter;
    ofParameter<float> tempo;
    ofParameter<float> intensityBlink;
    ofParameter<bool> hard_soft;
    ofParameter<ofColor> colorLygter;
    // BlinkendeLygter
    vector<Lygte> lygter;
    ofFbo graficBlinkendeLygter;

    
    // Boubbles Lyger
    // GUI / Controlpanel
    ofParameterGroup paramBoubbles;
    ofParameter<float> intensityBoubbles;
    ofParameter<float> velMin;
    ofParameter<float> velMax;

    ofParameter<ofColor> colorBoubbles;
    // BlinkendeLygter
    vector<Boubble> boubbles;
    ofFbo graficBoubbles;

    
    // dancing Lines
    // GUI / Controlpanel
    ofParameterGroup paramLines;
    ofParameter<float> speed;
    
    ofParameter<ofColor> colorDLines;

    vector<DancingLine> dancingLinesPole;
    vector<DancingLine> dancingLinesRamp;
    
    ofFbo fboTexPoles;
    ofFbo fboTexRamp;
    
};

