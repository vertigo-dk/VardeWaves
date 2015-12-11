//
//  lygte.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

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