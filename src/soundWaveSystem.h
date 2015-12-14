//
//  soundWaveSystem.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"

class SoundWave{
public:
    ofVec2f location;
    ofColor color;
    float lifeInc;
    float lifespan;
    float speed;
    float sizeRatio;
    
    SoundWave() {
        lifespan = 1.0;
        lifeInc = 0.001;
        sizeRatio = 0.;
        speed = 0;
        color = ofColor(255);
    }
    
    // Method to update location
    void update() {
        lifespan -= lifeInc;
        sizeRatio += speed;
    }
    // Method to display
    void draw(int _w) {
        
        float radius;
        float courve = 1-lifespan;//(1-cos(HALF_PI*(1-lifespan)));

        radius = _w * sizeRatio;

        color.a = 255-courve*255;
        ofSetColor(color);
        ofNoFill();
        ofSetLineWidth(10.);
        
        ofDrawCircle(location, radius);
    }
    
    // Is the particle still useful?
    bool isDead() {
        if (lifespan < 0.0 || sizeRatio > 1.5) {
            return true;
        } else {
            return false;
        }
    }
    
};

class SoundWaveSystem{
public:
    
    vector<SoundWave> soundWaves;
    int width;
    int height;
    
    ofFbo renderer;
    
    SoundWaveSystem(){
        
    }
    
    void setup(int _w, int _h){
        width = _w;
        height = _h;
        
        renderer.allocate(width, height);
    }
    
    void update(){
        for (vector<SoundWave>::iterator it=soundWaves.begin(); it!=soundWaves.end();)    {
            it->update();
            
            if(it->isDead())
                it = soundWaves.erase(it);
            else
                ++it;
        }
    }
    
    void draw(int _x, int _y, int _w, int _h){
        renderer.begin();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (int i = 0; i<soundWaves.size(); i++)    {
           soundWaves[i].draw(width);
        }
        renderer.end();
        
        renderer.draw(_x, _y, _w, _h);
    }
    
    void addSoundWave(ofVec2f _center, float _strength){
        SoundWave sW;
        sW.location = _center;
        sW.speed = _strength;
        soundWaves.push_back(sW);
    }
};

