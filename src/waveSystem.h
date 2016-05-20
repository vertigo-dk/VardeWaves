//
//  waveSystem.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"


class WaveParticle {
    
    // Inspired by http://vvvv.org/documentation/wave-simulation
public:
    float p;
    float v;
    float a;
    float attack;
    float damping;
    
    WaveParticle(){
        p = v = a = 0;
        attack = 0;
        damping = 0;
    }
    
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

class Drop{
public:
    float locX;
    float p;
    float v;
    float dragLimit;
    float extraMass;
    WaveParticle* waveParticleConnected;
    bool inWater;
    
    Drop() {
    }
    
    void init(float _locX, WaveParticle* _waveParticleConnected, float velMin, float velMax, float _dragLimit, float _extraMass){
        v = ofRandom(velMin, velMax);
        p = -1;
        dragLimit = _dragLimit;
        extraMass = _extraMass;
        locX = _locX;
        waveParticleConnected = _waveParticleConnected;
        inWater = false;
    }

    void init(float _locX, WaveParticle* _waveParticleConnected, float vel, float _dragLimit, float _extraMass){
        v = vel;
        p = -1;
        dragLimit = _dragLimit;
        extraMass = _extraMass;
        locX = _locX;
        waveParticleConnected = _waveParticleConnected;
        inWater = false;
    }
    
    // Method to update location
    void update() {
        p += v;
        if(p > waveParticleConnected->p){
            waveParticleConnected->p += v+extraMass;
            inWater = true;
        }
    }

    // Is the drop still there?
    bool isDead() {
        if(p > waveParticleConnected->p+dragLimit){
            return true;
        } else {
            return false;
        }
    }
    
};


class WaveParticleSystem{
public:
    vector<WaveParticle> waveParticles;
    vector<Drop> drops;
    

    float dropsDragLimit = 0.1;
    float dropsExtraMass  = 0.1;

    ofColor colorDrops;
    
    void setup(int _numArray){
        for(int i = 0; i<_numArray; i++){
            WaveParticle wP;
            wP.p = 0.;
            waveParticles.push_back(wP);
        }
    }
    
    void updateResponse(float _attack, float _damping){
        for(int i = 0; i<waveParticles.size(); i++){
            waveParticles[i].damping = _damping;
            waveParticles[i].attack  = _attack;
        }
    }
    
    void update(float _inLeft, float _inRight){
        waveParticles[0].p = _inLeft;
        waveParticles.back().p = _inRight;
        
        for(int i = 0; i < drops.size(); i++){
            drops[i].update();

            if(drops[i].isDead()){
                drops.erase(drops.begin() + i);
                i--;
            }
        }
        
        for(int i = 1; i<waveParticles.size()-1; i++){
            waveParticles[i].update(waveParticles[i-1].p, waveParticles[i+1].p);
            int i2 = waveParticles.size()-1-i;
            waveParticles[i2].update(waveParticles[i2-1].p, waveParticles[i2+1].p);
        }
    }
    
    
    void addDrop(float strength, float posX_from, float posX_to, float width){
        float locX = ofRandom(posX_from/width, posX_to/width);
        Drop drop;
        drop.init(locX, &waveParticles[int(waveParticles.size()*locX)], strength, dropsDragLimit, dropsExtraMass);
        drops.push_back(drop);
    }
    
    void drawDrops(int _x, int _y,int _w, int _h, ofColor colorDrop, float _posHLine){
        ofSetColor(colorDrop);
        ofFill();
        ofSetLineWidth(1.);
        for(auto & drop : drops){
            if(!drop.inWater){
                float x = drop.locX*_w;
                float y = (drop.p/2+1-_posHLine)*_h;
                ofDrawRectangle(x, y, 1,10);
            }
        }
    }
    
    void drawLine(int _x, int _y,int _w, int _h, ofColor colorLine, float _posHLine, int _lineWidth){
        ofPushMatrix();
        ofTranslate(_x, _y);
        ofSetLineWidth(_lineWidth);
        ofSetColor(colorLine);
        float inc = float(waveParticles.size())/float(_w+1);
        ofPath line;
        line.lineTo(0,_h); // Left under Corner

        for(int i = 0; i <= _w; i++){
            float x = i;
            float y = (waveParticles[i*inc].p/2+1-_posHLine)*_h;
            line.lineTo(ofVec2f(x,y));
        }
            line.lineTo(_w,_h); // right under corner
            line.lineTo(0,_h); // Left under Corner

           // line.close();
            line.isFilled();
        line.draw();
        ofPopMatrix();
    }
    
    void drawGradient(int _x, int _y, int _w, int _h, float _posHWave , ofTexture& _texture){
        ofPushMatrix();
        ofTranslate(_x, _y);
        
        float inc = float(waveParticles.size())/float(_w+1);
        float incTex = float(_texture.getWidth()) /float(_w+1);
        float texHeight = _texture.getHeight();
        float texHalfHeight = texHeight/2;
        
        ofMesh mesh;
        mesh.clear();
        mesh.enableTextures();
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        float min = 0;
        float max = 0;
        
        for(int i = 0; i <= _w; i++){
            float pValue = waveParticles[i*inc].p*-1;
            if(pValue < min) min = pValue;
            if(pValue > max) max = pValue;
            
        }
        
        for(int i = 0; i <= _w; i++){
            float x = i;
            float y = (waveParticles[i*inc].p/2+1-_posHWave)*_h;
            ofFloatColor pColor = ofFloatColor(1., ofMap( waveParticles[i*inc].p*-1, min, max, 0.8, 1, true));
            
            mesh.addVertex(ofVec3f(x, 0, 0));
            mesh.addTexCoord(ofVec2f(x,0));
            mesh.addColor(pColor);
            
            mesh.addVertex(ofVec3f(x, y, 0));
            mesh.addTexCoord(ofVec2f(x,texHalfHeight));
            mesh.addColor(pColor);
            
            mesh.addVertex(ofVec3f(x, _h, 0));
            mesh.addTexCoord(ofVec2f(x,texHeight));
            mesh.addColor(pColor);
        }
        
        for(int i = 0; i < mesh.getNumVertices()/3-1; i++){
            mesh.addIndex(i*3+1);
            mesh.addIndex(i*3+3);
            mesh.addIndex(i*3+0);
            
            mesh.addIndex(i*3+1);
            mesh.addIndex(i*3+4);
            mesh.addIndex(i*3+3);
            
            mesh.addIndex(i*3+2);
            mesh.addIndex(i*3+4);
            mesh.addIndex(i*3+1);
            
            mesh.addIndex(i*3+2);
            mesh.addIndex(i*3+5);
            mesh.addIndex(i*3+4);
        }
        ofSetColor( 255, 255, 255 );  //Set white color
        _texture.bind();
        mesh.draw();
        _texture.unbind();
        
        ofPopMatrix();
    }
    
};
