//
//  soundReactive.hpp
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"

class SoundWave{
public:
    ofVec2f location;
    ofColor sWaveColor;
    float tempo;
    float lifespan;
    float speed;
    
    SoundWave() {
        lifespan = 1.0;
        tempo = 0.01;
        speed = 0.1;
    }
    
    // Method to update location
    void update() {
        lifespan -= tempo;
    }
    // Method to display
    void draw(int _w) {
        
        float size;
        float courve = (1-cos(HALF_PI*(1-lifespan)))

        size = _w * courve;

        sWaveColor.a = 255-courve*255;
        ofSetColor(sWaveColor);
        ofNoFill();
        ofSetLineWidth(10.);
        
        ofDrawCircle(location.x,location.y,size);
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

class SoundWaveSystem{
    
    
    vector<SoundWave> soundWaves;
    int width;
    int height;
    
    SoundWaveSystem(){
        
    }
    
    void setup(int _x, int _y, int _w, int _h){
        
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
        for (vector<SoundWave>::iterator it=soundWaves.begin(); it!=soundWaves.end();)    {
            it->draw(_w);
        }
    }
    
    void addSoundWave(){
        SoundWave sW;
        soundWaves.push_back(sW);
    }
};

