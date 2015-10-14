#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOscParameterSync.h"

#include "defines.h"



class WaveParticle {
    
    // Inspired by http://vvvv.org/documentation/wave-simulation
public:
    ofVec2f p;
    float v;
    float a;
    float attack;
    float damping;
    
    void setResponse(float _attack, float _damping){
        attack = _attack;
        damping = _damping;
    }
    
    void calcAccel(ofVec2f p_left, ofVec2f p_right);
    
    void update( ofVec2f p_left, ofVec2f p_right ){
        float delta_p = p_left.y + p_right.y - 2*p.y;
        a = attack * delta_p;
        v = (v + a) * damping;
        p.y = p.y + v;
    }
};


class WaveParticleSystem{
public:
    vector<WaveParticle> waveParticles;
    
    void setup();
    void updateResponse(float _attack, float _damping);
    void update(float _inLeft, float _inRight);
    void drawLine(int _x, int _y, ofColor colorLine,float _posHLine);
    void drawGradient(int _x, int _y, ofColor colorWaveTop, ofColor colorWaveBot, float _posHWave );
    
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
    ofParameter<float> damping;
    ofParameter<float> attack;
    ofParameter<float> noiseAmt;
    
    ofParameter<float> posHWave;
    ofParameter<float> posHLine;
    
    
    
    
    ofParameter<ofColor> colorBG;
    ofParameter<ofColor> colorLine;
    ofParameter<ofColor> colorBGTop;
    ofParameter<ofColor> colorBGBot;
    ofParameter<ofColor> colorWaveTop;
    ofParameter<ofColor> colorWaveBot;
    
    
    ofxOscParameterSync syncOSC;
    
    
    
    ofShader shaderBlurX;
    ofShader shaderBlurY;
    ofFbo grafic_RGB;
    ofFbo grafic_W;
    
    ofFbo render;
    ofxSyphonServer syphonRenderOut;
    ofTexture syphonTex;
    
    //WaveArray wave;
    
    WaveParticleSystem wave_poles;
    WaveParticleSystem wave_ramp;
    
    
};

